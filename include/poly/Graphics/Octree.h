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

///////////////////////////////////////////////////////////
/// \brief A class that spatially organizes entities into a dynamic octree structure
///        and manages the rendering of these entitites.
///
///////////////////////////////////////////////////////////
class Octree : public RenderSystem
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Octree();

	///////////////////////////////////////////////////////////
	/// \brief Initialize the octree with a scene
	///
	/// This function must be called after create() is called
	/// and is used to attach event listeners and to add all
	/// currently renderable entities. This functions is automatically
	/// called by when the octree is added to the scene with
	/// Scene::addRenderSystem().
	///
	/// \param scene A pointer to the scene
	///
	///////////////////////////////////////////////////////////
	void init(Scene* scene) override;

	///////////////////////////////////////////////////////////
	/// \brief Create the octree with the specified parameters
	///
	/// This function creates the root node and sets up the 
	/// instance buffer for rendering. The max number of entities
	/// per cell can be specified, but the default is 30. When
	/// the limit per cell is reached, the cell will be split into
	/// 8 subcells, and the entities in that cell will be sorted
	/// into the subcells unless the entity is too big to fit into
	/// the subcell.
	///
	/// \param maxPerCell The max number of entities allowed per entity
	///
	///////////////////////////////////////////////////////////
	void create(Uint32 maxPerCell = 30);

	///////////////////////////////////////////////////////////
	/// \brief Add an entity to the octree
	///
	/// The entity must have both a TransformComponent and a
	/// RenderComponent. The entity is put into the smallest subcell
	/// that it can fit into, and if adding the entity causes the cell
	/// to go over the max entities allowed per cell, the cell will
	/// be split. The entities in that cell will be sorted
	/// into the subcells unless the entity is too big to fit into
	/// the subcell.
	///
	/// If the entity does not have the DynamicTag, then the entity
	/// transform will be cached and will not be updated until
	/// the entity is updated with update().
	///
	/// \param entity The entity to add
	///
	///////////////////////////////////////////////////////////
	void add(Entity entity);

	///////////////////////////////////////////////////////////
	/// \brief Add an entity to the octree
	///
	/// The entity must have both a TransformComponent and a
	/// RenderComponent. The entity is put into the smallest subcell
	/// that it can fit into, and if adding the entity causes the cell
	/// to go over the max entities allowed per cell, the cell will
	/// be split. The entities in that cell will be sorted
	/// into the subcells unless the entity is too big to fit into
	/// the subcell.
	///
	/// If the entity does not have the DynamicTag, then the entity
	/// transform will be cached and will not be updated until
	/// the entity is updated with update().
	///
	/// \param entity The id of the entity to add
	///
	///////////////////////////////////////////////////////////
	void add(Entity::Id entity);

	///////////////////////////////////////////////////////////
	/// \brief Update all entities with the dynamic tag
	///
	/// Any entity with the DynamicTag will be updated with the
	/// update() function.
	///
	///////////////////////////////////////////////////////////
	void update();

	///////////////////////////////////////////////////////////
	/// \brief Update the transform and containing cell of an entity
	///
	/// The entity will have its transform matrix updated, and if
	/// its containing cell changed, the entity will be removed from
	/// its previous cell and re-inserted into the tree.
	///
	/// \param entity The entity to update
	///
	///////////////////////////////////////////////////////////
	void update(Entity::Id entity);

	///////////////////////////////////////////////////////////
	/// \brief Remove an entity from the octree
	///
	/// \param entity The entity to remove
	///
	///////////////////////////////////////////////////////////
	void remove(Entity::Id entity);

	///////////////////////////////////////////////////////////
	/// \brief Render from the perspective of the camera
	///
	/// The octree uses a stream instance buffer to store the transform
	/// matrices. The transform matrices are update every single time
	/// render() is called, meaning that the matrices aren't reused
	/// across frames. This is for more consistent performance, and
	/// the matrices would have to be updated if a different camera
	/// is used, or if the view matrix of the camera is changed.
	///
	/// \param camera The camera to render from the perspective of
	/// \param pass The render pass that is being executed
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera, RenderPass pass) override;

private:
	struct Node;

	struct EntityData
	{
		Uint32 m_group;
		Node* m_node;
		bool m_castsShadows;
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
		Renderable* m_renderable;
		std::vector<Uint32> m_lodLevels;
		Skeleton* m_skeleton;
	};

	struct RenderData
	{
		VertexArray* m_vertexArray;
		Material* m_material;
		Shader* m_shader;
		Skeleton* m_skeleton;
		Uint32 m_offset;
		Uint32 m_instances;
	};

	void expand();

	void split(Node* node);

	void insert(EntityData* data);

	void update(const Entity::Id& id, RenderComponent& r, TransformComponent& t);

	void getRenderData(Node* node, const Frustum& frustum, std::vector<std::vector<EntityData*>>& entityData, const Vector3f& cameraPos, RenderPass pass);

	Uint32 getRenderGroup(Renderable* renderable, Skeleton* skeleton);

private:
	TypePool<Node> m_nodePool;							//!< The object pool that holds node data
	ObjectPool m_dataPool;								//!< The object pool that holds render data

	Node* m_root;										//!< A pointer to the root node
	float m_size;										//!< The size of the highest octree level
	Uint32 m_maxPerCell;								//!< The max number of entities allowed per cell
	HashMap<Entity::Id, EntityData*> m_dataMap;			//!< A map of entity id to its cached data

	VertexBuffer m_instanceBuffer;						//!< The instance buffer that stores instance transform data
	Uint32 m_instanceBufferOffset;						//!< The offset of the valid range of the instance buffer
	std::vector<RenderGroup> m_renderGroups;			//!< A list of render groups

	static Vector3f nodeOffsets[8];
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Octree
/// \ingroup Graphics
///
/// An octree is a spatial structure that is used to divide
/// entities into smaller groups. It is used to make culling
/// and spatial lookup faster as lookup has a log(N) time
/// complexity and culling requires less frustum tests.
///
/// To use the octree, call create() to create the initial octree,
/// then add it to a scene using Scene::addRenderSystem(). Adding
/// the octree to a scene requires a pointer to the octree, so
/// after adding it to the scene, it is important that the octree
/// remains in the same location in memory until the scene is
/// destroyed. Upon adding it to the scene, all existing entities
/// with both a TransformComponent and a RenderComponent will
/// be added to the octree, and all future entities that match
/// the requirement will be added as well.
///
/// If an entity has the DynamicTag component, its transform matrix
/// and containing cell will be updated every time update() is called.
/// Entities that don't have this tag, or static entities, can also be
/// updated by calling update() with ths static entity id. To render
/// the entities in the octree, simply call render() with the desired
/// camera. However, rendering is handled by the scene in most cases.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a scene with a single entity
/// Scene scene;
/// scene.createEntity(TransformComponent(), RenderComponent());
///
/// // Create the octree
/// Octree octree;
/// octree.create();
/// scene.addRenderSystem(&octree);
///
/// // Create a camera to render from the perspective of
/// Camera camera;
///
/// // Game loop
/// while (true)
/// {
///		// Update all dynamic entities
///		octree.update();
///
///		// Rendering the scene will render all render systems, including the octree
///		scene.render(camera);
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////