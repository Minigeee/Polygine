#ifndef POLY_TERRAIN_H
#define POLY_TERRAIN_H

#include <poly/Graphics/Shader.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Texture.h>
#include <poly/Graphics/UniformBuffer.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{


#ifndef DOXYGEN_SKIP

///////////////////////////////////////////////////////////
struct UniformBlock_Terrain
{
	UniformBufferType<Vector4f> m_clipPlanes[4];

	UniformBufferType<float> m_size;
	UniformBufferType<float> m_height;
	UniformBufferType<float> m_tileScale;
	UniformBufferType<float> m_blendLodDist;
	UniformBufferType<bool> m_useFlatShading;
};

#endif


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
	/// \param pass The render pass that is being executed
	/// \param deferred Whether terrain should use a deferred render
	///
	///////////////////////////////////////////////////////////
	virtual void render(Camera& camera, RenderPass pass, bool deferred) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the shader used to render the terrain
	///
	/// \param A pointer to the terrain shader
	///
	///////////////////////////////////////////////////////////
	void setShader(Shader* shader);

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
	/// \brief Set whether the terrain should use flat shading (low-poly style)
	///
	/// \param use The boolean controlling whether to use flat shading or not
	///
	///////////////////////////////////////////////////////////
	void setUseFlatShading(bool use);

	///////////////////////////////////////////////////////////
	/// \brief Set the terrain height map
	///
	/// Everytime a new height map is set, the normals are recalculated
	/// and both the height map and the normal map are pushed
	/// to the GPU texture. This function may take some time to
	/// transfer data between the CPU and the GPU. The image data
	/// pointer is used to retrieve height values, so make sure the
	/// data exists while the terrain is active.
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
	/// \brief Updates a subregion of the height map
	///
	/// The provided image containing the new height map data must
	/// have a size that matches the terrain height map.
	///
	/// \param map An image containing the new data (has to be the same size as the current height map)
	/// \param pos The top-left corner of the rectangle to update (in pixels)
	/// \param size The size of the rectangle to update (in pixels)
	///
	///////////////////////////////////////////////////////////
	void updateHeightMap(const Image& map, const Vector2i& pos = Vector2i(0), const Vector2u& size = Vector2u(0));

	///////////////////////////////////////////////////////////
	/// \brief Updates a subregion of the color map
	///
	/// The provided image containing the new color map data must
	/// have a size that matches the terrain color map.
	///
	/// \param map An image containing the new data (has to be the same size as the current height map)
	/// \param pos The top-left corner of the rectangle to update (in pixels)
	/// \param size The size of the rectangle to update (in pixels)
	///
	///////////////////////////////////////////////////////////
	void updateColorMap(const Image& map, const Vector2i& pos = Vector2i(0), const Vector2u& size = Vector2u(0));

	///////////////////////////////////////////////////////////
	/// \brief Get the terrain shader
	///
	/// \return A pointer to the terrain shader
	///
	///////////////////////////////////////////////////////////
	Shader* getShader() const;

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
	/// \brief Check if this terrain is rendered using flat shading
	///
	/// \return True if the terrain uses flat shading
	///
	///////////////////////////////////////////////////////////
	bool usesFlatShading() const;

	///////////////////////////////////////////////////////////
	/// \brief Get height map texture
	///
	/// \return A reference to the height map texture
	///
	///////////////////////////////////////////////////////////
	Texture& getHeightMap();

	///////////////////////////////////////////////////////////
	/// \brief Get color map texture
	///
	/// \return A reference to the color map texture
	///
	///////////////////////////////////////////////////////////
	Texture& getColorMap();

	///////////////////////////////////////////////////////////
	/// \brief Get normal map texture
	///
	/// \return A reference to the normal map texture
	///
	///////////////////////////////////////////////////////////
	Texture& getNormalMap();

	///////////////////////////////////////////////////////////
	/// \brief Get the interpolated height at the given 2d coordinates in world space
	///
	/// \param x The x-coordinate of the point of height to retrieve
	/// \param z The z-coordinate of the point of height to retrieve
	///
	/// \param height The height of the given point in world space units
	///
	///////////////////////////////////////////////////////////
	float getHeightAt(float x, float z) const;

protected:
	static Shader& getDefaultShader();

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
	/// \brief Apply terrain setting uniforms to shader
	///
	/// This is called before every terrain render to bind textures
	/// to the shader. The terrain uniform block is binded outside
	/// this function.
	///
	/// \param shader A pointer to the terrain shader
	///
	///////////////////////////////////////////////////////////
	virtual void applyTextures(Shader* shader);

	///////////////////////////////////////////////////////////
	/// \brief Create ring layout of terrain, depends on tile scale, lod scale, and max distance
	///
	///////////////////////////////////////////////////////////
	void createTileLayout();

	///////////////////////////////////////////////////////////
	/// \brief Calculate normals from height map given a subregion
	///
	///////////////////////////////////////////////////////////
	void calcNormals(const Image& hmap, const Vector2i& pos, const Vector2u& size);

	///////////////////////////////////////////////////////////
	/// \brief Update normal map, depends on height and size
	///
	///////////////////////////////////////////////////////////
	void updateNormalMap(const Vector3f& scale);

protected:
	float m_size;							//!< Terrain size
	float m_height;							//!< Maximume terrain height
	float m_tileScale;						//!< Tile scale
	float m_lodScale;						//!< Lod distance scale
	float m_maxDist;						//!< Maximum view distance
	bool m_useFlatShading;					//!< Controls if the terrain should be rendered using flat shading

	Shader* m_shader;						//!< The shader used to render the terrain
	Texture m_heightMap;					//!< Height map texture
	Texture m_normalMap;					//!< Normal map texture
	Texture m_colorMap;						//!< Color map texture
	float* m_heightMapData;
	Vector3f* m_normalMapData;				//!< Normal map texture data

	UniformBuffer m_uniformBuffer;			//!< The uniform buffer used to store terrain uniform data
	VertexArray m_normalTile;				//!< The mesh for a normal tile
	VertexArray m_edgeTile;					//!< The mesh for an edge tile
	VertexBuffer m_normalBuffer;			//!< The data buffer for a normal tile
	VertexBuffer m_edgeBuffer;				//!< The data buffer for a normal tile
	VertexBuffer m_instanceBuffer;			//!< The instance buffer for tile transform matrices
	Uint32 m_instanceBufferOffset;			//!< The current offset in the instance buffer

	std::vector<TerrainTile> m_normalTiles;	//!< A list of normal tiles in their default position
	std::vector<TerrainTile> m_edgeTiles;	//!< A list of edge tiles in their default position
	std::vector<float> m_lodDists;			//!< A list of exact lod distances

	bool m_isUniformDirty;					//!< This is true when one of the uniform parameters has changed

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