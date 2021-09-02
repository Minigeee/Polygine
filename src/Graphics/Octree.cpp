#include <poly/Core/Profiler.h>

#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

#include <poly/Graphics/Billboard.h>
#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Lighting.h>
#include <poly/Graphics/LodSystem.h>
#include <poly/Graphics/Model.h>
#include <poly/Graphics/Octree.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Shadows.h>
#include <poly/Graphics/Skeleton.h>

#include <poly/Math/Transform.h>

#include <iostream>

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


///////////////////////////////////////////////////////////
void bindShader(Shader* shader, Camera& camera, Scene* scene, RenderPass pass)
{
	shader->bind();

	// Camera
	camera.apply(shader);
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
	m_root					(0),
	m_size					(0.0f),
	m_maxPerCell			(0),
	m_numEntitites			(0),
	m_instanceBufferOffset	(0)
{

}


///////////////////////////////////////////////////////////
void Octree::init(Scene* scene)
{
	ASSERT(m_root, "Call Octree::create() before calling Octree::init()");

	m_scene = scene;

	// Add all renderables upon initialization
	m_scene->system<TransformComponent, RenderComponent>(
		[&](const Entity::Id& id, TransformComponent& t, RenderComponent& r)
		{
			add(id);
		}
	);

	// Add any new renderables from scene automatically
	m_scene->addListener<E_EntitiesCreated>(
		[&](const E_EntitiesCreated& e)
		{
			if (e.m_entities->has<RenderComponent>())
			{
				for (Uint32 i = 0; i < e.m_numEntities; ++i)
					add(e.m_entities[i]);
			}
		}
	);
}


///////////////////////////////////////////////////////////
void Octree::create(Uint32 maxPerCell)
{
	m_size = BASE_SIZE;
	m_maxPerCell = maxPerCell;

	// Create the root node
	m_root = (Node*)m_nodePool.alloc();
	*m_root = Node();
	m_root->m_boundingBox.m_min = Vector3f(-m_size * 0.5f);
	m_root->m_boundingBox.m_max = Vector3f(m_size * 0.5f);

	// Instance buffer
	m_instanceBuffer.create<Matrix4f>(NULL, 65536, BufferUsage::Stream);
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

			// Also update the data's containing node reference
			data->m_node = child;
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
void Octree::add(Entity entity)
{
	add(entity.getId());
}


///////////////////////////////////////////////////////////
void Octree::add(Entity::Id entity)
{
	ASSERT(m_scene, "The octree must be initialized before using, by calling the init() function");

	// Get component data
	auto components = m_scene->getComponents<TransformComponent, RenderComponent, AnimationComponent>(entity);
	RenderComponent& r = *components.get<RenderComponent*>();
	TransformComponent& t = *components.get<TransformComponent*>();
	AnimationComponent* a = components.get<AnimationComponent*>();

	// Get skeleton pointer
	Skeleton* skeleton = a ? a->m_skeleton : 0;

	// Get transform matrix
	Matrix4f transform = toTransformMatrix(t.m_position, t.m_rotation, t.m_scale);

	// Get bounding box
	BoundingBox bbox = r.m_renderable->getBoundingBox();
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

	// Create entity data
	EntityData* data = (EntityData*)m_dataPool.alloc();
	data->m_boundingBox = bbox;
	data->m_transform = transform;
	data->m_group = getRenderGroup(r.m_renderable, skeleton);
	data->m_castsShadows = r.m_castsShadows;

	std::unique_lock<std::mutex> lock(m_mutex);

	// Add to map
	m_dataMap[entity] = data;

	// Increment number of entities
	++m_numEntitites;


	// Insert into the octree
	insert(data);
}


///////////////////////////////////////////////////////////
void Octree::insert(EntityData* data)
{
	const BoundingBox& bbox = data->m_boundingBox;

	// Get box size (max)
	Vector3f dims = bbox.getDimensions();
	float boxSize = dims.x;
	if (dims.y > boxSize)
		boxSize = dims.y;
	if (dims.z > boxSize)
		boxSize = dims.z;

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
				bbox.m_max.x < rootMax.x && bbox.m_max.y < rootMax.y && bbox.m_max.z < rootMax.z;
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
			data->m_node = current;

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


///////////////////////////////////////////////////////////
void Octree::merge(Node* node)
{
	// Count the number of entities in children nodes
	Uint32 numEntities = 0;
	bool hasChildren = false;
	bool hasGrandchildren = false;

	for (Uint32 i = 0; i < 8; ++i)
	{
		Node* child = node->m_children[i];
		if (child)
		{
			hasChildren = true;
			numEntities += child->m_data.size();

			for (Uint32 j = 0; j < 8; ++j)
				hasGrandchildren |= (bool)child->m_children[j];
		}
	}

	// Can't merge if there is nothing to merge or if the node has grandchildren
	if (!hasChildren || hasGrandchildren) return;

	// Only merge if the number of entitites is less than a certain margin
	if (numEntities < (Uint32)(0.7f * m_maxPerCell))
	{
		std::vector<EntityData*>& data = node->m_data;

		for (Uint32 i = 0; i < 8; ++i)
		{
			if (node->m_children[i])
			{
				// Append all child node data
				std::vector<EntityData*>& childData = node->m_children[i]->m_data;
				data.insert(data.end(), childData.begin(), childData.end());
			}
		}

		// Update the containing node of each entity
		for (Uint32 i = 0; i < data.size(); ++i)
			data[i]->m_node = node;

		// Remove the children node
		for (Uint32 i = 0; i < 8; ++i)
		{
			if (node->m_children[i])
				m_nodePool.free(node->m_children[i]);

			node->m_children[i] = 0;
		}
	}
}


///////////////////////////////////////////////////////////
void Octree::update()
{
	ASSERT(m_scene, "The octree must be initialized before using, by calling the init() function");

	// Use a system update for entities with the dynamic tag
	m_scene->system<TransformComponent, RenderComponent, DynamicTag>(
		[&](const Entity::Id& id, TransformComponent& t, RenderComponent& r, DynamicTag&)
		{
			// Call update for each entity
			update(id, r, t);
		}
	);
}


///////////////////////////////////////////////////////////
void Octree::update(Entity::Id entity)
{
	ASSERT(m_scene, "The octree must be initialized before using, by calling the init() function");

	// Get component data
	auto components = m_scene->getComponents<TransformComponent, RenderComponent>(entity);
	RenderComponent& r = *components.get<RenderComponent*>();
	TransformComponent& t = *components.get<TransformComponent*>();

	// Update entity
	update(entity, r, t);
}


///////////////////////////////////////////////////////////
void Octree::update(const Entity::Id& entity, RenderComponent& r, TransformComponent& t)
{
	// Get transform matrix
	Matrix4f transform = toTransformMatrix(t.m_position, t.m_rotation, t.m_scale);

	// Get bounding box
	BoundingBox bbox = r.m_renderable->getBoundingBox();
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


	// Get node
	EntityData* data = m_dataMap[entity];
	data->m_boundingBox = bbox;
	data->m_transform = transform;
	data->m_castsShadows = r.m_castsShadows;

	Node* node = data->m_node;

	// Lock
	std::unique_lock<std::mutex> lock(m_mutex);

	// Get cell info
	float cellSize = BASE_SIZE * powf(2.0f, (float)node->m_level - 1.0f);
	Vector3f cellMin = node->m_boundingBox.m_min / cellSize;
	cellMin = Vector3f(roundf(cellMin.x), roundf(cellMin.y), roundf(cellMin.z)) * cellSize;
	Vector3f cellMax = cellMin + Vector3f(cellSize);

	// Check if the bounding box is still inside correct area
	Vector3f pos = bbox.getCenter();
	bool inside =
		pos.x > cellMin.x && pos.x < cellMax.x&&
		pos.y > cellMin.y && pos.y < cellMax.y&&
		pos.z > cellMin.z && pos.z < cellMax.z;

	// If the entity is not in the box anymore, update it
	if (!inside)
	{
		// Search for data in node
		std::vector<EntityData*>& nodeData = node->m_data;
		for (Uint32 i = 0; i < nodeData.size(); ++i)
		{
			if (nodeData[i] == data)
			{
				// Remove it
				nodeData[i] = nodeData.back();
				nodeData.pop_back();
			}
		}

		// Insert again
		insert(data);

		// Attempt to merge the parent cell (after inserting in case it was inserted into one of sibling nodes)
		if (node->m_parent)
			merge(node->m_parent);
	}
}


///////////////////////////////////////////////////////////
void Octree::remove(Entity::Id entity)
{
	ASSERT(m_scene, "The octree must be initialized before using, by calling the init() function");

	// Lock
	std::unique_lock<std::mutex> lock(m_mutex);

	// Get node
	EntityData* data = m_dataMap[entity];
	Node* node = data->m_node;

	// Search for data in node
	std::vector<EntityData*>& nodeData = node->m_data;
	for (Uint32 i = 0; i < nodeData.size(); ++i)
	{
		if (nodeData[i] == data)
		{
			// Remove it
			nodeData[i] = nodeData.back();
			nodeData.pop_back();
		}
	}

	// Attempt to merge the parent cell
	if (node->m_parent)
		merge(node->m_parent);

	// Remove from pool
	m_dataPool.free(data);

	// Decrement the number of entities
	--m_numEntitites;
}


///////////////////////////////////////////////////////////
void Octree::render(Camera& camera, RenderPass pass, const RenderSettings& settings)
{
	// Anything in the octree should be rendered for all passes

	ASSERT(m_scene, "The octree must be initialized before using, by calling the init() function");

	if (!settings.m_deferred)
	{
		// TODO : Forward render for transparent objects

		return;
	}

	START_PROFILING_FUNC;

	// Reset transparent render data
	m_transparentData.clear();

	// Get entity data
	std::vector<std::vector<EntityData*>> entityData(m_renderGroups.size());
	const Frustum& frustum = camera.getFrustum();
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		getRenderData(m_root, frustum, entityData, camera.getPosition(), pass);
	}
	
	// Get number of visible entities
	Uint32 numVisible = 0;
	for (Uint32 i = 0; i < entityData.size(); ++i)
		numVisible += entityData[i].size();

	if (!numVisible) return;


	// Stream instance data
	Uint32 size = numVisible * sizeof(Matrix4f);
	MapBufferFlags flags = MapBufferFlags::Write | MapBufferFlags::Unsynchronized;

	// Choose different flags based on how much space is left
	if (m_instanceBufferOffset + size > m_instanceBuffer.getSize())
	{
		flags |= MapBufferFlags::InvalidateBuffer;
		m_instanceBufferOffset = 0;
	}

	// Map the buffer
	Matrix4f* buffer = (Matrix4f*)m_instanceBuffer.map(m_instanceBufferOffset, size, flags);

	Uint32 numEntitiesMapped = 0;
	std::vector<RenderData> renderData;
	renderData.reserve(m_renderGroups.size());

	// Iterate through visible entities and stream data
	for (Uint32 i = 0; i < entityData.size(); ++i)
	{
		RenderGroup& group = m_renderGroups[i];
		const std::vector<EntityData*>& entities = entityData[i];

		// Skip the group if no entities or if lod
		if (!entities.size() || group.m_lodLevels.size())
			continue;

		// Create render data
		RenderData data;
		data.m_skeleton = group.m_skeleton;
		data.m_offset = m_instanceBufferOffset;
		data.m_instances = entities.size();

		// Take different actions based on what type of renderable being dealt with
		Model* model = 0;
		Billboard* billboard = 0;
		if ((model = dynamic_cast<Model*>(group.m_renderable)) != 0)
		{
			// Number of meshes prevented from rendering because of render mask
			Uint32 numMasked = 0;

			// Add data for every mesh in the model
			for (Uint32 j = 0; j < model->getNumMeshes(); ++j)
			{
				Mesh* mesh = model->getMesh(j);

				data.m_vertexArray = &mesh->m_vertexArray;
				data.m_material = &mesh->m_material;
				data.m_shader = mesh->m_shader;
				data.m_isTransparent = data.m_material->isTransparent();

				// Only add the data if it isn't masked
				if (data.m_material && !(Uint32)(data.m_material->getRenderMask() & pass))
					++numMasked;
				else if (data.m_isTransparent)
					m_transparentData.push_back(data);
				else
					renderData.push_back(data);
			}

			// If the number of masked is equal to the number of meshes, skip this group
			if (numMasked == model->getNumMeshes())
				continue;
		}
		else if ((billboard = dynamic_cast<Billboard*>(group.m_renderable)) != 0)
		{
			data.m_vertexArray = &billboard->getVertexArray();
			data.m_material = billboard->getMaterial();
			data.m_shader = billboard->getShader();
			data.m_isTransparent = data.m_material->isTransparent();

			// Only add the data if it isn't masked
			if (data.m_material && !(Uint32)(data.m_material->getRenderMask() & pass))
				continue;
			else if (data.m_isTransparent)
				m_transparentData.push_back(data);
			else
				renderData.push_back(data);
		}
		else
			// Otherwise, the renderable is not a valid type, and should be skipped
			continue;

		// Push instance data
		for (Uint32 j = 0; j < entities.size(); ++j)
			buffer[numEntitiesMapped + j] = entities[j]->m_transform;

		// Update instance buffer offset
		m_instanceBufferOffset += entities.size() * sizeof(Matrix4f);
		numEntitiesMapped += entities.size();
	}

	// After pushing all instance data, unmap the buffer
	m_instanceBuffer.unmap();

	// Sort by shader to minimize shader changes
	std::sort(renderData.begin(), renderData.end(),
		[](const RenderData& a, const RenderData& b) -> bool
		{
			if (a.m_shader == b.m_shader)
				return a.m_offset < b.m_offset;
			else
				return a.m_shader < b.m_shader;
		}
	);


	// Enable depth testing
	glCheck(glEnable(GL_DEPTH_TEST));

	// Disable alpha blending
	glCheck(glDisable(GL_BLEND));

	// Bind the first shader
	Shader* shader = renderData.front().m_shader;
	priv::bindShader(shader, camera, m_scene, pass);

	// Apply render settings
	applyRenderSettings(shader, settings);

	// Iterate through render data and render everything
	for (Uint32 i = 0; i < renderData.size(); ++i)
	{
		const RenderData& data = renderData[i];

		// If the shader changed, update to the new one
		if (data.m_shader != shader)
		{
			shader = data.m_shader;
			priv::bindShader(shader, camera, m_scene, pass);
		}

		Model* model = 0;
		Billboard* billboard = 0;

		// Apply the material to the shader
		if (data.m_material)
			data.m_material->apply(shader);

		// Get vertex array and do an instanced render
		VertexArray& vao = *data.m_vertexArray;

		// Different rendering behavior based on if the model is animated or not
		if (!data.m_skeleton)
		{
			// Bind instance data
			vao.bind();
			vao.addBuffer(m_instanceBuffer, 5, 4, sizeof(Matrix4f), data.m_offset + 0 * sizeof(Vector4f), 1);
			vao.addBuffer(m_instanceBuffer, 6, 4, sizeof(Matrix4f), data.m_offset + 1 * sizeof(Vector4f), 1);
			vao.addBuffer(m_instanceBuffer, 7, 4, sizeof(Matrix4f), data.m_offset + 2 * sizeof(Vector4f), 1);
			vao.addBuffer(m_instanceBuffer, 8, 4, sizeof(Matrix4f), data.m_offset + 3 * sizeof(Vector4f), 1);

			// Draw
			vao.draw(data.m_instances);
		}
		else
		{
			vao.bind();

			// Have to render each animated model individually
			for (Uint32 e = 0; e < data.m_instances; ++e)
			{
				// Bind transform data
				Uint32 offset = data.m_offset + e * sizeof(Matrix4f);
				vao.addBuffer(m_instanceBuffer, 5, 4, sizeof(Matrix4f), offset + 0 * sizeof(Vector4f), 1);
				vao.addBuffer(m_instanceBuffer, 6, 4, sizeof(Matrix4f), offset + 1 * sizeof(Vector4f), 1);
				vao.addBuffer(m_instanceBuffer, 7, 4, sizeof(Matrix4f), offset + 2 * sizeof(Vector4f), 1);
				vao.addBuffer(m_instanceBuffer, 8, 4, sizeof(Matrix4f), offset + 3 * sizeof(Vector4f), 1);

				// Apply skeleton
				data.m_skeleton->apply(shader);

				// Render model
				vao.draw();
			}
		}
	}

	// Reset render settings
	resetRenderSettings(settings);
}


///////////////////////////////////////////////////////////
bool Octree::hasDeferredPass() const
{
	return true;
}


///////////////////////////////////////////////////////////
bool Octree::hasForwardPass() const
{
	return true;
}


///////////////////////////////////////////////////////////
Uint32 Octree::getNumEntities() const
{
	return m_numEntitites;
}


///////////////////////////////////////////////////////////
void Octree::getRenderData(Node* node, const Frustum& frustum, std::vector<std::vector<EntityData*>>& entityData, const Vector3f& cameraPos, RenderPass pass)
{
	// Add all data inside the frustum
	for (Uint32 i = 0; i < node->m_data.size(); ++i)
	{
		EntityData* data = node->m_data[i];

		// Skip this object if the render pass is shadow and shadow casting is disabled
		if (pass == RenderPass::Shadow && !data->m_castsShadows)
			continue;

		// Frustum culling
		if (frustum.contains(data->m_boundingBox))
		{
			Uint32 groupId = data->m_group;

			// If the renderable is an lod system, add the correct group
			RenderGroup& group = m_renderGroups[groupId];
			if (group.m_lodLevels.size())
			{
				LodSystem* lod = (LodSystem*)group.m_renderable;

				Vector3f objectOffset = cameraPos - data->m_boundingBox.getCenter();
				float distSquared = dot(objectOffset, objectOffset);

				// Find the correct lod level
				Uint32 level = 0;
				for (; level < lod->getNumLevels() && distSquared > lod->getDistance(level) * lod->getDistance(level); ++level);

				groupId = level < lod->getNumLevels() ? group.m_lodLevels[level] : groupId;
			}

			entityData[groupId].push_back(data);
		}
	}

	// Call for children
	for (Uint32 i = 0; i < 8; ++i)
	{
		Node* child = node->m_children[i];
		if (child && frustum.contains(child->m_boundingBox))
			getRenderData(child, frustum, entityData, cameraPos, pass);
	}
}


///////////////////////////////////////////////////////////
Uint32 Octree::getRenderGroup(Renderable* renderable, Skeleton* skeleton)
{
	Uint32 groupId = 0;

	// Add render group
	bool groupExists = false;
	for (Uint32 i = 0; i < m_renderGroups.size(); ++i)
	{
		const RenderGroup& group = m_renderGroups[i];
		if (
			group.m_renderable == renderable &&
			group.m_skeleton == skeleton)
		{
			groupId = i;
			groupExists = true;
			break;
		}
	}

	if (!groupExists)
	{
		groupId = m_renderGroups.size();

		RenderGroup group;
		group.m_renderable = renderable;
		group.m_skeleton = skeleton;

		// If the renderable is an lod system, add lod levels
		LodSystem* lod = 0;
		if (lod = dynamic_cast<LodSystem*>(renderable))
		{
			Uint32 numLevels = lod->getNumLevels();

			// Get or create render groups for lod levels
			for (Uint32 i = 0; i < numLevels; ++i)
				group.m_lodLevels.push_back(getRenderGroup(lod->getRenderable(i), skeleton));
		}

		m_renderGroups.push_back(group);
	}

	return groupId;
}


}