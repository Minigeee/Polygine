#ifndef POLY_TERRAIN_H
#define POLY_TERRAIN_H

#include <poly/Engine/Entity.h>

#include <poly/Graphics/Material.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Texture.h>
#include <poly/Graphics/UniformBuffer.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <poly/Physics/Collider.h>

#include <future>
#include <stack>

namespace poly
{


///////////////////////////////////////////////////////////
class TerrainBase : public RenderSystem
{
public:
	TerrainBase();

	virtual ~TerrainBase();

	virtual void init(Scene* scene) override;

	void render(Camera& camera, RenderPass pass, const RenderSettings& settings) override;

	void create(float size, float maxHeight, float maxBaseSize = 50.0f);

	void setShader(Shader* shader);

	Entity getEntity() const;

	float getSize() const;

	float getMaxHeight() const;

protected:
	///////////////////////////////////////////////////////////
	/// \brief This function is called before rendering
	///
	/// This function should be responsible for binding the shader,
	/// setting any uniforms and textures needed to render the terrain,
	/// and potentially any terrain updates that need to occur.
	///
	///////////////////////////////////////////////////////////
	virtual void onRender(Camera& camera) = 0;

protected:
	///////////////////////////////////////////////////////////
	/// \brief A struct containing data for terrain lod levels
	///
	///////////////////////////////////////////////////////////
	struct LodLevel
	{
		float m_dist;									//!< The distance this lod level ends at
		ImageBuffer<Vector2<Uint16>> m_heightBounds;	//!< An image buffer of height bounds values for each terrain tile
	};

	///////////////////////////////////////////////////////////
	/// \brief Make render list from quadtree nodes
	///
	///////////////////////////////////////////////////////////
	void makeRenderList(const Vector2u& node, Uint32 lod, const Frustum& frustum, std::vector<Vector4f>& renderList);

protected:
	Entity m_entity;					//!< The scene entity that will be used for terrain colliders
	float m_size;						//!< The size of each side of the terrain (world units)
	float m_maxHeight;					//!< The maximum height of the terrain (world units)

	Vector3f m_viewpoint;				//!< The player viewpoint (determins lod level of each tile)
	float m_baseScale;					//!< The scale of the base level tile

	Shader* m_shader;					//!< A pointer to the terrain shader
	VertexBuffer m_instanceBuffer;		//!< The tile instance buffer
	VertexBuffer m_vertexBuffer;		//!< The tile vertex buffer
	VertexBuffer m_indexBuffer;			//!< The tile index buffer
	VertexArray m_vertexArray;			//!< The render vertex array
	Uint32 m_instanceDataOffset;		//!< The offset of the instance buffer in bytes

	std::mutex m_mutex;					//!< Protect potentially multithreaded parts of terrain
	Uint32 m_numLevels;					//!< The number of quadtree levels
	std::vector<LodLevel> m_lodLevels;	//!< A list of terrain lod levels (where 0 is the largest level)

	bool m_viewpointChanged;			//!< True if viewpoint has changed (this is set in render loop, must be reset when used)
	bool m_lodDistsChanged;				//!< True if lod distances changed
};


///////////////////////////////////////////////////////////
/// \brief A render system that render low-poly style terrain
///
///////////////////////////////////////////////////////////
class Terrain : public TerrainBase
{
public:
	typedef ImageBuffer<float>				HeightMap;
	typedef ImageBuffer<Vector3<Uint16>>	NormalMap;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Terrain();

	///////////////////////////////////////////////////////////
	/// \brief Destructor that frees all memory allocated by the terrain
	///
	///////////////////////////////////////////////////////////
	~Terrain();

	void init(Scene* scene) override;

	void setHeightMap(const Image& hmap);

	void setBounciness(float bounciness);

	void setFrictionCoefficient(float coefficient);

	void setCollisionCategory(Uint16 category);

	void setCollisionMask(Uint16 mask);

	Texture& getHeightMap();

	Texture& getNormalMap();

	HeightMap& getHeightData();

	NormalMap& getNormalData();

	///////////////////////////////////////////////////////////
	/// \brief Get the collider bounciness value
	///
	/// \return The bounciness value
	///
	///////////////////////////////////////////////////////////
	float getBounciness() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the friction coefficient
	///
	/// \return The friction coefficient
	///
	///////////////////////////////////////////////////////////
	float getFrictionCoefficient() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the collision category bitfield
	///
	/// \return The collision category bitfield
	///
	///////////////////////////////////////////////////////////
	Uint16 getCollisionCategory() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the collision mask bitfield
	///
	/// \return The collision mask bitfield
	///
	///////////////////////////////////////////////////////////
	Uint16 getCollisionMask() const;

private:
	static Shader& getShader();

	static Shader s_shader;

	void onRender(Camera& camera) override;

	///////////////////////////////////////////////////////////
	/// \brief Update a subrect of the terrain height map
	///
	///////////////////////////////////////////////////////////
	void updateHeightMap(const Vector2u& pos, const Vector2u& size);

	void updateHeightBounds(Uint32 nr, Uint32 nc);

private:
	Texture m_heightMap;
	Texture m_normalMap;
	HeightMap m_heightMapImg;
	NormalMap m_normalMapImg;

	Collider m_collider;
	float m_bounciness;
	float m_friction;
	Uint16 m_collisionCategory;
	Uint16 m_collisionMask;
};


///////////////////////////////////////////////////////////
class LargeTerrain : public TerrainBase
{
public:
	typedef std::function<bool(const Vector2i&, Uint32, Image*)> LoadFunc;

public:
	LargeTerrain();

	~LargeTerrain();

	void create(float size, float maxHeight, float maxBaseSize = 50.0f, float tileSize = 512.0f);

	void setHeightLoader(const LoadFunc& func);

	void setSplatLoader(const LoadFunc& func);

	void addCustomLoader(const LoadFunc& func);

	void onUnloadTile(const std::function<void(const Vector2i&, Uint32)>& func);

	void setSplatTexture(Texture* texture, Uint32 index);

	void applyRedirectMap(Shader* shader);

	void setBounciness(float bounciness);

	void setFrictionCoefficient(float coefficient);

	void setCollisionCategory(Uint16 category);

	void setCollisionMask(Uint16 mask);

	Texture& getRedirectMap();

	Texture& getHeightMap();

	Texture& getNormalMap();

	Texture& getSplatMap();

	Texture* getCustomMap(Uint32 index) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the collider bounciness value
	///
	/// \return The bounciness value
	///
	///////////////////////////////////////////////////////////
	float getBounciness() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the friction coefficient
	///
	/// \return The friction coefficient
	///
	///////////////////////////////////////////////////////////
	float getFrictionCoefficient() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the collision category bitfield
	///
	/// \return The collision category bitfield
	///
	///////////////////////////////////////////////////////////
	Uint16 getCollisionCategory() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the collision mask bitfield
	///
	/// \return The collision mask bitfield
	///
	///////////////////////////////////////////////////////////
	Uint16 getCollisionMask() const;

private:
	enum class EdgeRow
	{
		Left,
		Right,
		Top,
		Bottom,
		LMid,
		RMid,
		TMid,
		BMid
	};

	struct MapData
	{
		enum Type
		{
			Height,
			Normal,
			Splat,
			Custom
		};

		Texture* m_texture;
		Image* m_fullImg;
		Image* m_edgeImg;
		Uint8 m_edgeResL;
		Uint8 m_edgeResR;
		Uint8 m_edgeResT;
		Uint8 m_edgeResB;
	};

	struct LoadTask
	{
		Task<bool> m_task;
		Image* m_image;
		Texture* m_texture;
		MapData::Type m_mapType;
		Vector3<Uint16> m_tileData;
	};

	struct Tile
	{
		std::vector<MapData> m_mapData;
		Vector2<Uint8> m_cachePos;
		Vector3<Uint16> m_tileData;
		Uint32 m_isLoaded;
	};

	struct ColliderInfo
	{
		Collider m_collider;
		Vector2<Uint16> m_tile;
	};

	static Shader& getShader();

	static Shader s_shader;

	void onRender(Camera& camera) override;

	void updateTileMaps(const Vector2u& node, Uint32 lod);

	void updateLoadTasks();

	Tile* getAdjTile(const Vector3<Uint16>& tileData);

	bool processHeightTile(Image* hmap, Image* nmap, const Vector3<Uint16>& tile);

private:
	float m_tileSize;		//!< The size of the area that each tile map covers (per side in world units)

	Texture m_heightMap;
	Texture m_normalMap;
	Texture m_splatMap;
	Texture m_redirectMap;
	ImageBuffer<Vector3<Uint8>> m_redirectMapImg;
	std::vector<Texture*> m_customMaps;

	LoadFunc m_heightLoadFunc;
	LoadFunc m_splatLoadFunc;
	std::vector<LoadFunc> m_customLoadFuncs;
	std::function<void(const Vector2i&, Uint32)> m_unloadFunc;

	std::vector<Texture*> m_splatTextures;

	std::vector<ColliderInfo> m_colliders;
	float m_bounciness;
	float m_friction;
	Uint16 m_collisionCategory;
	Uint16 m_collisionMask;

	Uint32 m_baseTileLevel;
	Vector2u m_cacheMapSize;
	std::stack<Vector2<Uint8>> m_freeList;
	HashMap<Vector3<Uint16>, Tile> m_tileMap;
	std::vector<LoadTask*> m_loadTasks;

	Uint32 m_tileLoadedBitfield;
	bool m_redirectMapChanged;
};


}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Terrain
/// \ingroup Graphics
///
/// This class creates and renders low-poly style terrain.
/// To create and render terrain, call create(), then add the
/// terrain to a scene using Scene::addRenderSystem(). Adding the
/// terrain as a render system requires taking a pointer to the
/// terrain object, so after adding it to the scene, the terrain
/// must be kept in the same memory location until the scene is
/// destroyed. The terrain will then be rendered any time
/// Scene::render() is called, and it will alwyas be centered on
/// on the origin.
///
/// Usage example:
/// \code
///
/// Scene scene;
///
/// // Load a height map
/// Image hmap;
/// hmap.load("images/hmap.png", GLType::Float);
///
/// // Load a color map
/// Image cmap;
/// cmap.load("images/cmap.png");
///
/// // Create the terrain with a size of 4000 units, a height of 200 units, and a tile size of 3.0
/// Terrain terrain;
/// terrain.create(4000.0f, 200.0f, 1.5f);
/// terrain.setHeightMap(hmap);
/// terrain.setColorMap(cmap);
/// scene.addRenderSystem(&terrain);
///
/// // Create a camera to render from the perspective of
/// Camera camera;
///
/// // Game loop
/// while (true)
/// {
///		// Rendering the scene will render all render systems, including the terrain
///		scene.render(camera);
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////