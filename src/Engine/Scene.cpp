#include <poly/Core/Profiler.h>

#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Components.h>
#include <poly/Graphics/Lights.h>
#include <poly/Graphics/Model.h>
#include <poly/Graphics/Shader.h>

#include <poly/Math/Transform.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
struct RenderableEntity
{
	TransformComponent* m_transform;
	RenderComponent* m_render;
};


///////////////////////////////////////////////////////////
struct RenderData
{
	Model* m_model;
	Shader* m_shader;
	Uint32 m_offset;
	Uint32 m_instances;
};


///////////////////////////////////////////////////////////
void bindShader(Shader* shader, Camera* camera)
{
	shader->bind();
	shader->setUniform("u_projView", camera->getProjMatrix() * camera->getViewMatrix());
}


}


///////////////////////////////////////////////////////////
HandleArray<bool> Scene::idArray;


///////////////////////////////////////////////////////////
Scene::Scene() :
	m_handle				(idArray.add(true)),
	m_camera				(0),
	m_instanceBufferOffset	(0)
{
	// Create stream type instance buffer
	m_instanceBuffer.create<Matrix4f>(0, 65536, BufferUsage::Stream);
}


///////////////////////////////////////////////////////////
Scene::~Scene()
{
	// Clean up ECS
	{
		std::lock_guard<std::mutex> lock(m_entityMutex);
		priv::ComponentCleanup::cleanup(m_handle.m_index);
	}

	// Clean up event systems
	priv::SceneEventsCleanup::cleanup(m_handle.m_index);

	// Remove from list to free up id
	idArray.remove(m_handle);
}


///////////////////////////////////////////////////////////
Uint16 Scene::getId() const
{
	return m_handle.m_index;
}


///////////////////////////////////////////////////////////
void Scene::removeEntity(Entity::Id id)
{
	// Find the correct group
	Uint32 groupId = id.m_group;

	// Entity removal is protected by mutex
	std::lock_guard<std::mutex> lock(m_entityMutex);

	auto it = m_entityGroups.find(groupId);
	ASSERT(it != m_entityGroups.end(), "Can not remove entity from unknown entity group: %d", groupId);

	// This function adds it to a remove queue
	it.value().removeEntity(id);
}


///////////////////////////////////////////////////////////
void Scene::removeQueuedEntities()
{
	// Entity removal is protected by mutex
	std::lock_guard<std::mutex> lock(m_entityMutex);

	// Clear all queues
	for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); ++it)
		it.value().removeQueuedEntities();
}


///////////////////////////////////////////////////////////
void Scene::addTag(Entity::Id id, int tag)
{
	std::lock_guard<std::mutex> lock(m_tagMutex);

	// Insert id into correct tag group
	m_entityTags[tag].insert(id);
}


///////////////////////////////////////////////////////////
void Scene::removeTag(Entity::Id id, int tag)
{
	std::lock_guard<std::mutex> lock(m_tagMutex);

	// Remove entity from tag group
	m_entityTags[tag].erase(id);
}


///////////////////////////////////////////////////////////
bool Scene::hasTag(Entity::Id id, int tag)
{
	std::lock_guard<std::mutex> lock(m_tagMutex);

	// Get the tag group
	auto it = m_entityTags.find(tag);
	if (it == m_entityTags.end()) return false;

	// Return if the id has been found
	return it->second.find(id) != it->second.end();
}


///////////////////////////////////////////////////////////
const HashSet<Entity::Id>& Scene::getEntitiesWithTag(Uint32 tag)
{
	return m_entityTags[tag];
}


///////////////////////////////////////////////////////////
void Scene::addLight(Light* light)
{
	// Add the light to the correct subtype array

	if (dynamic_cast<DirectionLight*>(light))
		m_dirLights.push_back((DirectionLight*)light);
}


///////////////////////////////////////////////////////////
void Scene::removeLight(Light* light)
{
	// Remove the light from the correct array

	if (dynamic_cast<DirectionLight*>(light))
	{
		for (Uint32 i = 0; i < m_dirLights.size(); ++i)
		{
			if (m_dirLights[i] == light)
			{
				// Do a swap pop
				m_dirLights[i] = m_dirLights.back();
				m_dirLights.pop_back();
			}
		}
	}
}


///////////////////////////////////////////////////////////
void Scene::setCamera(Camera* camera)
{
	m_camera = camera;
}


///////////////////////////////////////////////////////////
Camera* Scene::getCamera() const
{
	return m_camera;
}


///////////////////////////////////////////////////////////
void Scene::render(const RenderState& state)
{
	START_PROFILING_FUNC;

	// Apply render state
	state.apply();

	HashMap<Model*, std::vector<priv::RenderableEntity>> visibleEntities;

	// Keep track of the total number of visible entities
	Uint32 numVisible = 0;

	// Get the camera frustum
	const Frustum& frustum = m_camera->getFrustum();

	// Sort and cull all renderable entities
	system<TransformComponent, RenderComponent>(
		[&](const Entity::Id& id, TransformComponent& t, RenderComponent& r)
		{
			// Do culling here
			bool culled = false;

			// Get the scale
			float scale = t.m_scale.x;
			if (t.m_scale.y > scale)
				scale = t.m_scale.y;
			if (t.m_scale.z > scale)
				scale = t.m_scale.z;

			// Get the bounding sphere
			Sphere sphere = r.m_model->getBoundingSphere();
			sphere.m_position += t.m_position;
			sphere.m_radius *= scale;

			// Test if the sphere is in the frustum
			culled |= !frustum.contains(sphere);

			if (!culled)
			{
				// Add it to the list of visible entities
				visibleEntities[r.m_model].push_back(priv::RenderableEntity{ &t, &r });

				// Increment number of visible
				++numVisible;
			}

			// Update culled status
			r.m_culled = culled;
		}
	);

	// Do nothing if there is nothing to render
	if (!numVisible) return;


	// Stream instance data
	Uint32 size = numVisible * sizeof(Matrix4f);
	int flags = (int)MapBufferFlags::Write;

	// Choose different flags based on how much space is left
	if (m_instanceBufferOffset + size > m_instanceBuffer.getSize())
	{
		flags |= (int)MapBufferFlags::InvalidateBuffer;
		m_instanceBufferOffset = 0;
	}
	else
	{
		flags |= (int)MapBufferFlags::Unsynchronized;
	}

	// Map the buffer
	Matrix4f* buffer = (Matrix4f*)m_instanceBuffer.map(m_instanceBufferOffset, size, flags);

	Uint32 numEntitiesMapped = 0;
	std::vector<priv::RenderData> renderData;
	renderData.reserve(visibleEntities.size());

	// Iterate through visible entities and stream data
	for (auto it = visibleEntities.begin(); it != visibleEntities.end(); ++it)
	{
		const std::vector<priv::RenderableEntity>& entities = it.value();

		// Create render data
		priv::RenderData data;
		data.m_model = entities.front().m_render->m_model;
		data.m_shader = entities.front().m_render->m_shader;
		data.m_offset = m_instanceBufferOffset;
		data.m_instances = entities.size();
		renderData.push_back(data);

		// Push instance data
		for (Uint32 i = 0; i < entities.size(); ++i)
		{
			const TransformComponent& t = *entities[i].m_transform;
			buffer[numEntitiesMapped + i] = toTransformMatrix(t.m_position, t.m_rotation, t.m_scale);
		}

		// Update instance buffer offset
		m_instanceBufferOffset += entities.size() * sizeof(Matrix4f);
		numEntitiesMapped += entities.size();
	}

	// After pushing all instance data, unmap the buffer
	m_instanceBuffer.unmap();

	// Sort by shader to minimize shader changes
	std::sort(renderData.begin(), renderData.end(),
		[](const priv::RenderData& a, const priv::RenderData& b) -> bool
		{
			if (a.m_shader == b.m_shader)
				return a.m_offset < b.m_offset;
			else
				return a.m_shader < b.m_shader;
		}
	);

	// Bind the first shader
	Shader* shader = renderData.front().m_shader;
	priv::bindShader(shader, m_camera);

	// Iterate through render data and render everything
	for (Uint32 i = 0; i < renderData.size(); ++i)
	{
		const priv::RenderData& data = renderData[i];

		// If the shader changed, update to the new one
		if (data.m_shader != shader)
		{
			shader = data.m_shader;
			priv::bindShader(shader, m_camera);
		}

		// Apply the materials
		const std::vector<Material>& materials = data.m_model->getMaterials();
		for (Uint32 i = 0; i < materials.size(); ++i)
			materials[i].apply(shader, i);

		// Get vertex array and do an instanced render
		VertexArray& vao = data.m_model->getVertexArray();

		// Bind instance data
		vao.bind();
		vao.addBuffer(m_instanceBuffer, 5, 4, sizeof(Matrix4f), data.m_offset + 0 * sizeof(Vector4f), 1);
		vao.addBuffer(m_instanceBuffer, 6, 4, sizeof(Matrix4f), data.m_offset + 1 * sizeof(Vector4f), 1);
		vao.addBuffer(m_instanceBuffer, 7, 4, sizeof(Matrix4f), data.m_offset + 2 * sizeof(Vector4f), 1);
		vao.addBuffer(m_instanceBuffer, 8, 4, sizeof(Matrix4f), data.m_offset + 3 * sizeof(Vector4f), 1);

		// Draw
		vao.draw(data.m_instances);
	}
}

}