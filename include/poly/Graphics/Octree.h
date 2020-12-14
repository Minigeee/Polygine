#ifndef POLY_OCTREE_H
#define POLY_OCTREE_H

#include <poly/Core/ObjectPool.h>

#include <poly/Engine/Entity.h>

#include <poly/Math/BoundingBox.h>
#include <poly/Math/Matrix4.h>

#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

class Camera;
class Model;
class Shader;

class Octree : public RenderSystem
{
public:
	Octree();

	void init(Scene* scene) override;

	void create(Uint32 maxPerCell = 30);

	void add(Entity entity);

	void add(Entity::Id entity);

	void update();

	void update(Entity::Id entity);

	void remove(Entity::Id entity);

	///////////////////////////////////////////////////////////
	/// \brief Render to a specified framebuffer
	///
	/// Still in progress... TODO
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera) override;

	void setAmbientColor(const Vector3f& color);

	const Vector3f& getAmbientColor() const;

private:
	struct Node;

	struct EntityData
	{
		Uint32 m_group;
		Node* m_node;
		BoundingBox m_boundingBox;
		Matrix4f m_transform;
	};

	struct Node
	{
		Node();

		int m_level;
		Node* m_parent;
		Node* m_children[8];
		BoundingBox m_boundingBox;
		std::vector<EntityData*> m_data;
	};

	struct RenderGroup
	{
		Model* m_model;
		Shader* m_shader;
		Skeleton* m_skeleton;
	};

	struct RenderData
	{
		Model* m_model;
		Shader* m_shader;
		Skeleton* m_skeleton;
		Uint32 m_offset;
		Uint32 m_instances;
	};

	void expand();

	void split(Node* node);

	void insert(EntityData* data);

	void update(const Entity::Id& id, RenderComponent& r, TransformComponent& t);

	void getRenderData(Node* node, const Frustum& frustum, std::vector<std::vector<EntityData*>>& entityData);

private:
	ObjectPool m_nodePool;
	ObjectPool m_dataPool;

	Scene* m_scene;
	Node* m_root;
	float m_size;
	Uint32 m_maxPerCell;
	HashMap<Entity::Id, EntityData*> m_dataMap;

	VertexBuffer m_instanceBuffer;						//!< The instance buffer that stores instance transform data
	Uint32 m_instanceBufferOffset;						//!< The offset of the valid range of the instance buffer
	std::vector<RenderGroup> m_renderGroups;			//!< A list of render groups

	Vector3f m_ambientColor;

	static Vector3f nodeOffsets[8];
};

}

#endif