#ifndef POLY_TERRAIN_H
#define POLY_TERRAIN_H

#include <poly/Graphics/Material.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Texture.h>
#include <poly/Graphics/UniformBuffer.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <future>
#include <stack>

namespace poly
{


class Terrain;


///////////////////////////////////////////////////////////
/// \brief A texture wrapper class for large terrain maps that are too large to hold in memory
///
///////////////////////////////////////////////////////////
class TerrainMap
{
	friend Terrain;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	TerrainMap();

	void create(Uint32 size, Uint32 tileSize = 256, PixelFormat fmt = PixelFormat::Rgb, GLType dtype = GLType::Uint8);

	Texture& getTexture();

	Uint32 getSize() const;

	Uint32 getTileSize() const;

	PixelFormat getFormat() const;

	GLType getDataType() const;

	void onRequestTile(const std::function<bool(const Vector2i&, Uint32, Image&)>& func);

private:
	Image* load(const Vector2i& pos, Uint32 level);

	void setCacheSize(Uint32 w, Uint32 h);

private:
	Texture m_texture;			//!< The cache texture
	Uint32 m_size;				//!< The true size of the texture
	Uint32 m_tileSize;			//!< The tile size of the texture
	PixelFormat m_format;		//!< The pixel format of the texture
	GLType m_dataType;			//!< The data type of the pixel data

	std::function<bool(const Vector2i&, Uint32, Image&)> m_loadFunc;
};


///////////////////////////////////////////////////////////
/// \brief A render system that render low-poly style terrain
///
///////////////////////////////////////////////////////////
class Terrain : public RenderSystem
{
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

	///////////////////////////////////////////////////////////
	/// \brief Initialize the terrain for a scene
	///
	/// This stores a pointer to the scene, and is automatically called
	/// when the terrain is added to a scene with Scene::addRenderSystem().
	///
	/// \param scene A pointer to the scene to initialize with
	///
	///////////////////////////////////////////////////////////
	void init(Scene* scene) override;

	///////////////////////////////////////////////////////////
	/// \brief Render the terrain from the perspective of the camera
	///
	/// This function will be called by the scene, so in most cases,
	/// this function will not be directly called.
	///
	/// \param camera The camera to render from the perspective of
	/// \param pass The render pass that is being executed
	/// \param settings The render settings to apply
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera, RenderPass pass, const RenderSettings& settings) override;

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	void create(float chunkSize, float maxHeight, float maxNodeSize = 50.0f, float viewDist = -1.0f);

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	void setViewpoint(const Vector3f& point);

	void setHeightMap(const Image& hmap);

	void setHeightMap(TerrainMap* hmap);

	void setHeightBounds(const Vector2i& tile, const Vector2f& bounds);

protected:
	struct LodLevel
	{
		float m_range;
		std::vector<Vector2<Uint16>> m_heightBounds;
	};

	struct AsyncTileObj
	{
		std::future<Image*> m_future;
		Texture* m_cacheTexture;
		Vector3<Uint8> m_nodeData;
		Vector2<Uint8> m_cachePos;
	};

	static Shader& getDefaultShader();

	static Shader& getTerrainMapShader();

	static Shader s_shader;
	static Shader s_tmapShader;

	void updateData(const Vector2u& pos, const Vector2u& size);

	void updateHeightBounds(Uint32 r, Uint32 c);

	void addLodNodes(const Vector2u& node, Uint32 lod, const Frustum& frustum, std::vector<Vector4f>& renderList);

	void loadMapTiles(const Vector2u& node, Uint32 lod);

	void createIndexMap(Uint32 size);

	Image* createNormalTile(Image* hmap, float chunkSize);

protected:
	float m_size;
	float m_maxHeight;
	Uint32 m_numLevels;

	void* m_heightMap;
	void* m_normalMap;
	Image m_heightMapImg;
	Image m_normalMapImg;
	std::vector<TerrainMap*> m_customMaps;
	Uint32 m_tileSize;
	bool m_usesTerrainMaps;

	Shader* m_shader;
	VertexBuffer m_instanceBuffer;
	VertexBuffer m_vertexBuffer;
	VertexBuffer m_indexBuffer;
	VertexArray m_vertexArray;
	Uint32 m_instanceDataOffset;

	Texture m_indexMap;
	ImageBuffer<Vector2<Uint8>> m_indexMapImg;
	std::stack<Vector2<Uint8>> m_indexFreeList;
	Uint32 m_highestMapLevel;
	HashMap<Vector3<Uint8>, Vector2<Uint8>> m_loadedMapTiles;
	std::vector<AsyncTileObj> m_loadThreads;
	bool m_indexMapChanged;

	Vector3f m_viewpoint;
	float m_baseScale;
	std::vector<LodLevel> m_lodLevels;
	bool m_viewpointChanged;
	bool m_useCustomViewpoint;
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