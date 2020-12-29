#ifndef POLY_TERRAIN_H
#define POLY_TERRAIN_H

#include <poly/Graphics/Shader.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Texture.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

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
	/// \brief Create the low-poly terrain with the specified parameters
	///
	/// The terrain can be thought of as a large square mesh, where each
	/// side of the square is \a size units long, and the height at each
	/// point in the mesh is sampled from a height map.
	///
	/// The terrain is made up of small tiles that slowly get larger
	/// as their distance to the camera increases (terrain lod). The default
	/// tile size is 2.0 units, but this can be scaled with /a tileScale.
	/// The distances at which the tiles start growing in size are defined
	/// by the lod distances. By default, these distances are [20.0, 100.0,
	/// 200.0, maxDist], but these distances can be scaled with \a lodScale.
	///
	/// \param size The size of a terrain edge, where the terrain is a large square mesh
	/// \param height The max height of the terrain
	/// \param tileScale The factor to scale a single tile scale at lod 0 (Default size is 2.0 units)
	/// \param lodScale The factor to scale terrain lod distances (Default lod distances are: [20, 100, 200, maxDist])
	/// \param maxDist The maximum distance the terrain should still be visible
	///
	///////////////////////////////////////////////////////////
	void create(float size, float height, float tileScale = 1.0f, float lodScale = 1.0f, float maxDist = 800.0f);

	///////////////////////////////////////////////////////////
	/// \brief Render the terrain from the perspective of the camera
	///
	/// This function will be called by the scene, so in most cases,
	/// this function will not be directly called.
	///
	/// \param camera The camera to render from the perspective of
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the terrain
	///
	/// The size of the terrain is the length of one of its sides
	///
	/// \param size The size
	///
	///////////////////////////////////////////////////////////
	void setSize(float size);

	///////////////////////////////////////////////////////////
	/// \brief Set the max height of the terrain
	///
	/// \param height The height
	///
	///////////////////////////////////////////////////////////
	void setHeight(float height);

	///////////////////////////////////////////////////////////
	/// \brief Set the scale of each tile
	///
	/// The default tile size is 2.0, this size can be scaled with
	/// this function.
	///
	/// \param height The tile scale
	///
	///////////////////////////////////////////////////////////
	void setTileScale(float scale);

	///////////////////////////////////////////////////////////
	/// \brief Set the scale of each lod (level of detail) distance
	///
	/// The default lod distances are [20.0, 100.0, 200.0, maxDist],
	/// and those distances can be scaled with this function.
	///
	/// \param height The tile scale
	///
	///////////////////////////////////////////////////////////
	void setLodScale(float scale);

	///////////////////////////////////////////////////////////
	/// \brief Set the maximum distance the terrain is rendered
	///
	/// Any terrain sections that are further from the camera than
	/// this distance will not be rendered.
	///
	/// \param maxDist The maximum distance
	///
	///////////////////////////////////////////////////////////
	void setMaxDist(float maxDist);

	///////////////////////////////////////////////////////////
	/// \brief Set the terrain height map
	///
	/// Everytime a new height map is set, the normals are recalculated
	/// and both the height map and the normal map are pushed
	/// to the GPU texture. This function may take some time to
	/// transfer data between the CPU and the GPU.
	///
	/// \param map The height map
	///
	///////////////////////////////////////////////////////////
	void setHeightMap(const Image& map);

	///////////////////////////////////////////////////////////
	/// \brief Set the terrain color map
	///
	/// The color map defines the color of the terrain at different
	/// areas of the terrain. This function may take some time to
	/// transfer data between the CPU and the GPU.
	///
	/// \param map The color map
	///
	///////////////////////////////////////////////////////////
	void setColorMap(const Image& map);

	///////////////////////////////////////////////////////////
	/// \brief Set the terrain ambient color
	///
	/// \param color The ambient color
	///
	///////////////////////////////////////////////////////////
	void setAmbientColor(const Vector3f& color);

	///////////////////////////////////////////////////////////
	/// \brief Get terrain size
	///
	/// \return Terrain size
	///
	///////////////////////////////////////////////////////////
	float getSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get terrain max height
	///
	/// \return Terrain max height
	///
	///////////////////////////////////////////////////////////
	float getHeight() const;

	///////////////////////////////////////////////////////////
	/// \brief Get terrain tile scale
	///
	/// \return Terrain tile scale
	///
	///////////////////////////////////////////////////////////
	float getTileScale() const;

	///////////////////////////////////////////////////////////
	/// \brief Get terrain lod scale
	///
	/// \return Terrain lod scale
	///
	///////////////////////////////////////////////////////////
	float getLodScale() const;

	///////////////////////////////////////////////////////////
	/// \brief Get max view distance
	///
	/// \return Max view distance
	///
	///////////////////////////////////////////////////////////
	float getMaxDist() const;

	///////////////////////////////////////////////////////////
	/// \brief Get height map texture
	///
	/// \return A reference to the height map texture
	///
	///////////////////////////////////////////////////////////
	const Texture& getHeightMap() const;

	///////////////////////////////////////////////////////////
	/// \brief Get color map texture
	///
	/// \return A reference to the color map texture
	///
	///////////////////////////////////////////////////////////
	const Texture& getColorMap() const;

	///////////////////////////////////////////////////////////
	/// \brief Get normal map texture
	///
	/// \return A reference to the normal map texture
	///
	///////////////////////////////////////////////////////////
	const Texture& getNormalMap() const;

	///////////////////////////////////////////////////////////
	/// \brief Get terrain ambient color
	///
	/// \return Terrain ambient color
	///
	///////////////////////////////////////////////////////////
	const Vector3f& getAmbientColor() const;

private:
	static Shader& getShader();

	struct TerrainTile
	{
		BoundingBox m_boundingBox;
		Vector2f m_position;
		float m_rotation;
		float m_scale;
		Uint32 m_lod;
	};

	struct InstanceData
	{
		float m_lodDist;
		Matrix4f m_transform;
	};

	///////////////////////////////////////////////////////////
	/// \brief Create ring layout of terrain, depends on tile scale, lod scale, and max distance
	///
	///////////////////////////////////////////////////////////
	void createTileLayout();

	///////////////////////////////////////////////////////////
	/// \brief Update normal map, depends on height and size
	///
	///////////////////////////////////////////////////////////
	void updateNormalMap(const Vector3f& scale);

private:
	Scene* m_scene;							//!< A pointer to a scene
	float m_size;							//!< Terrain size
	float m_height;							//!< Maximume terrain height
	float m_tileScale;						//!< Tile scale
	float m_lodScale;						//!< Lod distance scale
	float m_maxDist;						//!< Maximume view distance

	Texture m_heightMap;					//!< Height map texture
	Texture m_normalMap;					//!< Normal map texture
	Texture m_colorMap;						//!< Color map texture
	Vector3f* m_normalMapData;				//!< Normal map texture data

	VertexArray m_normalTile;				//!< The mesh for a normal tile
	VertexArray m_edgeTile;					//!< The mesh for an edge tile
	VertexBuffer m_normalBuffer;			//!< The data buffer for a normal tile
	VertexBuffer m_edgeBuffer;				//!< The data buffer for a normal tile
	VertexBuffer m_instanceBuffer;			//!< The instance buffer for tile transform matrices
	Uint32 m_instanceBufferOffset;			//!< The current offset in the instance buffer

	std::vector<TerrainTile> m_normalTiles;	//!< A list of normal tiles in their default position
	std::vector<TerrainTile> m_edgeTiles;	//!< A list of edge tiles in their default position
	std::vector<float> m_lodDists;			//!< A list of exact lod distances

	Vector3f m_ambientColor;				//!< The ambient color

	static Shader s_shader;
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