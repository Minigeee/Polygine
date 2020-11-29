#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/Model.h>
#include <poly/Graphics/Octree.h>

#include <poly/Math/Transform.h>

#define BASE_SIZE 16.0f

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
bool updateBoundingBox(BoundingBox& a, const BoundingBox& b)
{
	bool changed = false;

	if (b.m_min.x < a.m_min.x)
	{
		a.m_min.x = b.m_min.x;
		changed = true;
	}

	if (b.m_max.x > a.m_max.x)
	{
		a.m_max.x = b.m_max.x;
		changed = true;
	}

	if (b.m_min.y < a.m_min.y)
	{
		a.m_min.y = b.m_min.y;
		changed = true;
	}

	if (b.m_max.y > a.m_max.y)
	{
		a.m_max.y = b.m_max.y;
		changed = true;
	}

	if (b.m_min.z < a.m_min.z)
	{
		a.m_min.z = b.m_min.z;
		changed = true;
	}

	if (b.m_max.z > a.m_max.z)
	{
		a.m_max.z = b.m_max.z;
		changed = true;
	}

	return changed;
}


}


///////////////////////////////////////////////////////////
Vector3f Octree::nodeOffsets[8] =
{
	Vector3f(0.0f, 0.0f, 0.0f),
	Vector3f(0.5f, 0.0f, 0.0f),
	Vector3f(0.0f, 0.0f, 0.5f),
	Vector3f(0.5f, 0.0f, 0.5f),
	Vector3f(0.0f, 0.5f, 0.0f),
	Vector3f(0.5f, 0.5f, 0.0f),
	Vector3f(0.0f, 0.5f, 0.5f),
	Vector3f(0.5f, 0.5f, 0.5f)
};


///////////////////////////////////////////////////////////
Octree::Node::Node() :
	m_level		(0),
	m_parent	(0),
	m_children	{ 0, 0, 0, 0, 0, 0, 0, 0 }
{
	
}


///////////////////////////////////////////////////////////
Octree::Octree() :
	m_nodePool				(sizeof(Node)),
	m_dataPool				(sizeof(EntityData)),
	m_scene					(0),
	m_root					(0),
	m_size					(0.0f),
	m_maxPerCell			(0),
	m_instanceBufferOffset	(0)
{

}


///////////////////////////////////////////////////////////
void Octree::init(Scene* scene, Uint32 maxPerCell)
{
	m_scene = scene;
	m_size = BASE_SIZE;
	m_maxPerCell = maxPerCell;

	// Create the root node
	m_root = (Node*)m_nodePool.alloc();
	*m_root = Node();
	m_root->m_boundingBox.m_min = Vector3f(-m_size * 0.5f);
	m_root->m_boundingBox.m_max = Vector3f(m_size * 0.5f);
}


///////////////////////////////////////////////////////////
void Octree::expand()
{
	// Calculate new size
	m_size = BASE_SIZE * powf(2.0f, (float)++m_root->m_level);

	// Update bounding box
	m_root->m_boundingBox.m_min = Vector3f(-m_size * 0.5f);
	m_root->m_boundingBox.m_max = Vector3f(m_size * 0.5f);

	// Create new subnodes for every previous node
	for (Uint32 i = 0; i < 8; ++i)
	{
		// If there are no entities in a subnode, then the node will not exist
		if (!m_root->m_children[i]) continue;

		// If the node exists, then it is guaranteed to have less than the max objects per node
		// So a new node must be created
		Vector3f minPoint = m_root->m_boundingBox.m_min + nodeOffsets[i] * m_size;
		Vector3f maxPoint = minPoint + Vector3f(m_size * 0.5f);

		Node* node = (Node*)m_nodePool.alloc();
		*node = Node();
		node->m_boundingBox.m_min = minPoint;
		node->m_boundingBox.m_max = maxPoint;
		node->m_level = m_root->m_level - 1;
		m_root->m_children[i]->m_parent = node;
		node->m_children[7 - i] = m_root->m_children[i];

		// Assign new node to previous node
		node->m_parent = m_root;
		m_root->m_children[i] = node;

		// Update bounding box
		Node* gchild = node->m_children[7 - i];
		priv::updateBoundingBox(node->m_boundingBox, gchild->m_boundingBox);
	}
}


///////////////////////////////////////////////////////////
void Octree::split(Node* node)
{
	std::vector<EntityData*> keep;
	std::vector<EntityData*> children[8];

	// Get current node spatial info
	float cellSize = BASE_SIZE * powf(2.0f, (float)node->m_level);
	Vector3f cellMin = node->m_boundingBox.m_min / (cellSize * 0.5f);
	cellMin = Vector3f(roundf(cellMin.x), roundf(cellMin.y), roundf(cellMin.z)) * cellSize * 0.5f;

	// Iterate through data and decide where each should go
	std::vector<EntityData*>& data = node->m_data;
	for (Uint32 i = 0; i < data.size(); ++i)
	{
		const BoundingBox& bbox = data[i]->m_boundingBox;

		// Get box size (max)
		Vector3f dims = bbox.getDimensions();
		float boxSize = dims.x;
		if (dims.y > boxSize)
			boxSize = dims.y;
		if (dims.z > boxSize)
			boxSize = dims.z;

		// Keep the entity if the size is too big for children
		if (boxSize >= cellSize * 0.5f)
			keep.push_back(data[i]);

		else
		{
			// Find which subchild the entity belongs to
			Vector3f pos = bbox.getCenter();
			Uint32 index = 0;
			if (pos.y > cellMin.y + 0.5f * cellSize)
				index += 4;
			if (pos.z > cellMin.z + 0.5f * cellSize)
				index += 2;
			if (pos.x > cellMin.x + 0.5f * cellSize)
				index += 1;

			// Add it to the list
			children[index].push_back(data[i]);
		}
	}

	// Update current node data
	node->m_data = std::move(keep);

	// Keep track of bounding box changes
	bool changed = false;

	// Create a new node for each child that has data
	for (Uint32 i = 0; i < 8; ++i)
	{
		// Skip if child has no data
		if (!children[i].size()) continue;

		// Create a new node
		Node* child = (Node*)m_nodePool.alloc();
		*child = Node();
		child->m_boundingBox.m_min = cellMin + nodeOffsets[i] * cellSize;
		child->m_boundingBox.m_max = child->m_boundingBox.m_min + Vector3f(cellSize * 0.5f);
		child->m_level = node->m_level - 1;
		child->m_data = std::move(children[i]);

		// Set child node
		child->m_parent = node;
		node->m_children[i] = child;

		// Update bounding box based on entity
		for (Uint32 j = 0; j < child->m_data.size(); ++j)
		{
			EntityData* data = child->m_data[j];
			priv::updateBoundingBox(child->m_boundingBox, data->m_boundingBox);
		}

		// Update parent bounding box
		changed |= priv::updateBoundingBox(node->m_boundingBox, child->m_boundingBox);
	}

	// Update all bounding boxes to the root
	Node* current = node;
	while (changed && current != m_root)
	{
		changed = priv::updateBoundingBox(current->m_parent->m_boundingBox, current->m_boundingBox);
		current = current->m_parent;
	}
}


///////////////////////////////////////////////////////////
void Octree::add(Entity::Id entity)
{
	// Get component data
	RenderComponent& r = *m_scene->getComponent<RenderComponent>(entity);
	TransformComponent& t = *m_scene->getComponent<TransformComponent>(entity);

	// Get transform matrix
	Matrix4f transform = toTransformMatrix(t.m_position, t.m_rotation, t.m_scale);

	// Get bounding box
	BoundingBox bbox = r.m_model->getBoundingBox();
	Vector3f vertices[] =
	{
		Vector3f(bbox.m_min.x, bbox.m_min.y, bbox.m_min.z),
		Vector3f(bbox.m_max.x, bbox.m_min.y, bbox.m_min.z),
		Vector3f(bbox.m_min.x, bbox.m_max.y, bbox.m_min.z),
		Vector3f(bbox.m_max.x, bbox.m_max.y, bbox.m_min.z),
		Vector3f(bbox.m_min.x, bbox.m_min.y, bbox.m_max.z),
		Vector3f(bbox.m_max.x, bbox.m_min.y, bbox.m_max.z),
		Vector3f(bbox.m_min.x, bbox.m_max.y, bbox.m_max.z),
		Vector3f(bbox.m_max.x, bbox.m_max.y, bbox.m_max.z)
	};

	// Transform bounding box
	bbox.m_min = Vector3f(transform * Vector4f(vertices[0], 1.0f));
	bbox.m_max = bbox.m_min;
	for (Uint32 i = 1; i < 8; ++i)
	{
		Vector4f v = transform * Vector4f(vertices[i], 1.0f);

		if (v.x < bbox.m_min.x)
			bbox.m_min.x = v.x;
		else if (v.x > bbox.m_max.x)
			bbox.m_max.x = v.x;

		if (v.y < bbox.m_min.y)
			bbox.m_min.y = v.y;
		else if (v.y > bbox.m_max.y)
			bbox.m_max.y = v.y;

		if (v.z < bbox.m_min.z)
			bbox.m_min.z = v.z;
		else if (v.z > bbox.m_max.z)
			bbox.m_max.z = v.z;
	}

	// Get box size (max)
	Vector3f dims = bbox.getDimensions();
	float boxSize = dims.x;
	if (dims.y > boxSize)
		boxSize = dims.y;
	if (dims.z > boxSize)
		boxSize = dims.z;

	// Create entity data
	EntityData* data = (EntityData*)m_dataPool.alloc();
	data->m_entity = entity;
	data->m_boundingBox = bbox;
	data->m_transform = transform;
	data->m_group = 0;

	// Add to map
	m_dataMap[entity] = data;


	// Determine the min level for entity
	int minLevel = 0;
	float ratio = boxSize / BASE_SIZE;

	while (ratio > 1.0f || ratio < 0.5f)
	{
		// If the ratio is above 1, then the entity is too big for current level
		// So the min level should be increased
		if (ratio > 1.0f)
			ratio = boxSize / (BASE_SIZE * powf(2.0f, (float)++minLevel));
		else if (ratio < 0.5f)
			ratio = boxSize / (BASE_SIZE * powf(2.0f, (float)--minLevel));
	}

	{
		Vector3f& rootMin = m_root->m_boundingBox.m_min;
		Vector3f& rootMax = m_root->m_boundingBox.m_max;
		bool inside =
			bbox.m_min.x > rootMin.x && bbox.m_min.y > rootMin.y && bbox.m_min.z > rootMin.z &&
			bbox.m_max.x < rootMax.x && bbox.m_max.y < rootMax.y && bbox.m_max.z < rootMax.z;

		// Expand the octree until the root has a level equal to minLevel + 1
		while (m_root->m_level < minLevel + 1 || !inside)
		{
			expand();

			inside =
				bbox.m_min.x > rootMin.x && bbox.m_min.y > rootMin.y && bbox.m_min.z > rootMin.z &&
				bbox.m_max.x < rootMax.x&& bbox.m_max.y < rootMax.y&& bbox.m_max.z < rootMax.z;
		}
	}

	// Start inserting at the root
	Node* current = m_root;
	bool added = false;

	while (!added)
	{
		// Check if the node has children
		bool hasChildren = false;
		for (Uint32 i = 0; i < 8; ++i)
			hasChildren |= (bool)current->m_children[i];

		// If the current cell is at the entity min level, add to the current cell,
		// or if there are no children, add the entity into the current cell
		if (current->m_level == minLevel || !hasChildren)
		{
			current->m_data.push_back(data);

			// If at or over max, attempt to split the node
			if (current->m_data.size() >= m_maxPerCell)
				split(current);

			// Otherwise, need to update bounding box (this is done automatically with split)
			else
			{
				bool changed = priv::updateBoundingBox(current->m_boundingBox, data->m_boundingBox);

				// Update all bounding boxes to the root
				while (changed && current != m_root)
				{
					changed = priv::updateBoundingBox(current->m_parent->m_boundingBox, current->m_boundingBox);
					current = current->m_parent;
				}
			}

			// Quit the loop
			added = true;
		}
		else
		{
			float cellSize = BASE_SIZE * powf(2.0f, (float)current->m_level);
			Vector3f cellMin = current->m_boundingBox.m_min / (cellSize * 0.5f);
			cellMin = Vector3f(roundf(cellMin.x), roundf(cellMin.y), roundf(cellMin.z)) * cellSize * 0.5f;

			// Find which subchild the entity belongs to
			Vector3f pos = bbox.getCenter();

			Uint32 index = 0;
			if (pos.y > cellMin.y + 0.5f * cellSize)
				index += 4;
			if (pos.z > cellMin.z + 0.5f * cellSize)
				index += 2;
			if (pos.x > cellMin.x + 0.5f * cellSize)
				index += 1;

			// Try to make the child node the current
			// But if the node does not yet exist, create it
			if (!current->m_children[index])
			{
				Node* child = (Node*)m_nodePool.alloc();
				*child = Node();
				child->m_boundingBox.m_min = current->m_boundingBox.m_min + nodeOffsets[index] * cellSize;
				child->m_boundingBox.m_max = child->m_boundingBox.m_min + Vector3f(cellSize * 0.5f);
				child->m_level = current->m_level - 1;

				child->m_parent = current;
				current->m_children[index] = child;
			}

			current = current->m_children[index];
		}
	}
}


}