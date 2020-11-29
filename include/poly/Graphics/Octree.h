#ifndef POLY_OCTREE_H
#define POLY_OCTREE_H

#include <poly/Core/ObjectPool.h>

#include <poly/Engine/Entity.h>

#include <poly/Math/BoundingBox.h>
#include <poly/Math/Matrix4.h>

#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/RenderState.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

class Camera;
class Model;
class Shader;

class Octree
{
public:
	Octree();

	void init(Scene* scene, Uint32 maxPerCell = 30);

	void add(Entity::Id entity);

	void update(Entity::Id entity);

	void remove(Entity::Id entity);

	///////////////////////////////////////////////////////////
	/// \brief Render to a specified framebuffer
	///
	/// Still in progress... TODO
	///
	///////////////////////////////////////////////////////////
	void render(
		Camera& camera,
		FrameBuffer& target = FrameBuffer::Default,
		const RenderState& state = RenderState::Default
	);

private:
	struct EntityData
	{
		Entity::Id m_entity;
		Uint32 m_group;
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

	void expand();

	void split(Node* node);

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

	static Vector3f nodeOffsets[8];
};

}

#endif