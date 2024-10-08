#ifndef POLY_MODEL_H
#define POLY_MODEL_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/BoundingBox.h>
#include <poly/Math/Sphere.h>
#include <poly/Math/Vector3.h>

#include <poly/Graphics/Material.h>
#include <poly/Graphics/Renderable.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <string>
#include <vector>

namespace poly
{

#ifndef DOXYGEN_SKIP

namespace priv
{


///////////////////////////////////////////////////////////
struct SkeletalData
{
	SkeletalData();

	Vector4f m_boneWeights;
	Vector4i m_boneIds;
};


}

#endif

class Texture;


///////////////////////////////////////////////////////////
/// \brief A struct containing model load settings
///
///////////////////////////////////////////////////////////
struct ModelLoadSettings
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	ModelLoadSettings();

	Vector3f m_scale;			//!< The scale to apply to model vertices
	float m_adjustForGamma;		//!< The gamma factor to adjust loaded textures for
	bool m_flatShading;			//!< Indicates whether the model should be loaded in a way that sets up for flat shading
	bool m_loadMaterials;		//!< Indicates whether model materials should be loaded (in case materials are shared)
};


///////////////////////////////////////////////////////////
/// \brief A collection of vertices that can be rendered with a single material
///
///////////////////////////////////////////////////////////
struct Mesh
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Mesh();

	VertexArray m_vertexArray;		//!< The vertex array containing the vertex data
	Material m_material;			//!< The mesh material
	Shader* m_shader;				//!< A pointer to the shader
	Uint32 m_offset;				//!< The vertex offset of the mesh
};


///////////////////////////////////////////////////////////
/// \brief A struct that contains vertex data
///
///////////////////////////////////////////////////////////
struct Vertex
{
	Vertex();
	Vertex(const Vector3f& pos, const Vector3f& normal);
	Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& texCoord);
	Vertex(const Vector3f& pos, const Vector3f& normal, const Colorf& color);
	Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& texCoord, const Colorf& color);

	Vector3f m_position;	//!< Vertex position
	Vector3f m_normal;		//!< Vertex normal vector
	Vector2f m_texCoord;	//!< Texture coordinate
	Colorf m_color;			//!< Vertex color
	Vector3f m_tangent;		//!< Vertex tangent vector
};


///////////////////////////////////////////////////////////
/// \brief A class that contains vertex data through collection of meshes
///
///////////////////////////////////////////////////////////
class Model : public Renderable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Model();

	///////////////////////////////////////////////////////////
	/// \brief Load a model from a file
	///
	/// \param fname The model file name to load
	/// \param settings The settings to use for loading the model
	///
	/// \see load
	///
	///////////////////////////////////////////////////////////
	Model(const std::string& fname, const ModelLoadSettings& settings = ModelLoadSettings());

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Remove all allocated meshes
	///
	///////////////////////////////////////////////////////////
	~Model();

	///////////////////////////////////////////////////////////
	/// \brief Load a model from a file
	///
	/// This will load all vertex positions, normals, texture
	/// coordinates, and colors from the model. If an armature
	/// is store in the model file, it will also load all bone
	/// weights and ids.
	///
	/// Then all material properties are loaded, including diffuse,
	/// specular, shininess, and a single diffuse map and a single
	/// specular map.
	///
	/// This function also gives the option to load models with
	/// the smooth shading option, by having each vertex share 
	/// normals for each face. Set \a flatShading to false to use
	/// smooth shading. The model file must also have vertex normals
	/// setup correctly as well.
	///
	/// If this function is not called from a thread with an active
	/// OpenGL context, the vertices and materials will be loaded into
	/// memory, but won't be pushed onto the GPU. The model will be
	/// partially loaded, so finish() must be called from a thread
	/// with an active OpenGL context.
	///
	/// \param fname The model file name to load
	/// \param settings The settings to use for loading the model
	///
	/// \note The \a gamma adjustment factor only affects images with RGB or RGBA color formats
	///
	/// \return True if the file was successfully loaded
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname, const ModelLoadSettings& settings = ModelLoadSettings());

	///////////////////////////////////////////////////////////
	/// \brief Finish loading a model from a file
	///
	/// This function should be used if the model is loaded from
	/// a thread that does not have an active OpenGL context. This
	/// function should be called from a thread with an active OpenGL
	/// context.
	///
	/// \return True if the model finished loading successfully
	///
	///////////////////////////////////////////////////////////
	bool finish();

	///////////////////////////////////////////////////////////
	/// \brief Create a model with a single mesh from a list of vertices
	///
	/// This function should be used to create a model with a single
	/// mesh. The single mesh will 
	///
	/// \param vertices The list of vertices
	/// \param usage The vertex buffer usage pattern
	///
	///////////////////////////////////////////////////////////
	void addMesh(
		const std::vector<Vertex>& vertices,
		const std::vector<Uint32>& indices = std::vector<Uint32>(),
		const Material& material = Material(),
		BufferUsage usage = BufferUsage::Static
	);

	///////////////////////////////////////////////////////////
	/// \brief Set the material for the specified mesh index
	///
	/// Each model can have more than one mesh, and each mesh
	/// has a single material.
	///
	/// \param material The material to set
	/// \param index The mesh index to assign the material to
	///
	///////////////////////////////////////////////////////////
	void setMaterial(const Material& material, Uint32 index = 0);

	///////////////////////////////////////////////////////////
	/// \brief Set the shader for the specified mesh index
	///
	/// Each model can have more than one mesh, and each mesh
	/// has a single shader it uses to render the mesh.
	///
	/// \param shader The shader to set
	/// \param index The mesh index to assign the shader to
	///
	///////////////////////////////////////////////////////////
	void setShader(Shader* material, Uint32 index = 0);

	///////////////////////////////////////////////////////////
	/// \brief Get the list of vertices
	///
	/// \return The list of vertices
	///
	///////////////////////////////////////////////////////////
	const std::vector<Vertex>& getVertices() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the list of vertex indices
	///
	/// \return The list of vertex indices
	///
	///////////////////////////////////////////////////////////
	const std::vector<Uint32>& getIndices() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of meshes contained within the model
	///
	/// \return The number of meshes in the model
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumMeshes() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the model mesh at the specified index
	///
	/// A mesh contains the vertex array containing the vertex
	/// data, a material, and a shader that should be used to
	/// render the mesh.
	///
	/// \param index The index of the mesh to retrieve
	///
	/// \return The mesh at the specified index
	///
	///////////////////////////////////////////////////////////
	Mesh* getMesh(Uint32 index = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the default model shader
	///
	/// \return The default model shader
	///
	///////////////////////////////////////////////////////////
	static Shader& getDefaultShader();

	///////////////////////////////////////////////////////////
	/// \brief Get the animated model shader
	///
	/// \return The default model shader
	///
	///////////////////////////////////////////////////////////
	static Shader& getAnimatedShader();

private:
	VertexBuffer m_vertexBuffer;					//!< The vertex buffer used to store the main vertex data
	VertexBuffer m_skeletalVertexBuffer;			//!< The vertex buffer used to store skeletal vertex data
	VertexBuffer m_indicesBuffer;					//!< The vertex buffer used to store vertex index data

	std::vector<Vertex> m_vertices;					//!< The list of vertex data excluding skeletal data
	std::vector<Uint32> m_indices;					//!< A list of vertex indices specifying the order of vertices (only for smooth shading)
	std::vector<Mesh*> m_meshes;					//!< The vertex array used to render the model

	std::vector<priv::SkeletalData> m_skeletalData;	//!< Stores skeletal data temporarily in the case data is loaded in a non-render thread
	std::vector<Uint32> m_meshVertexOffsets;		//!< Stores vertex offsets temporarily in the case data is loaded in a non-render thread

	static Shader s_defaultShader;					//!< The default model shader
	static Shader s_animatedShader;					//!< The animated model shader
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Model
/// \ingroup Graphics
///
/// The model class manages all the data needed to render 3D meshes
/// and their corresponding materials. A model consists of a
/// list of vertices that are rendered in triangles mode, and
/// one or more materials. Each vertex consists of the following
/// components:
///
/// \li Position
/// \li Normal
/// \li %Texture coordinate
/// \li Color
/// \li %Material index
///
/// And the following components if an armature was provided in
/// the loaded model file:
///
/// \li %Bone weights
/// \li %Bone ids
///
/// The easiest way to use a model is to use its load() function
/// to load a 3D model from a file. This function uses the Assimp
/// model loading library internally, so any file format that is
/// supported by Assimp will be able to be loaded. Some common formats
/// that are supported are:
///
/// \li DAE/Collada
/// \li STL
/// \li OBJ
/// \li FBX
/// \li 3DS
/// \li A lot more...
///
/// It is also possible to create a model without loading it from
/// a file using setVertices(), though this is generally not recommended
/// unless custom meshes are needed (i.e. random terrain). Creating
/// animated models without loading from a file is not yet supported but
/// be added in the future.
///
/// Models also support multiple materials, but each vertex may
/// only be rendered with a single material. The material each
/// vertex is rendered can be specified with each vertex
/// material index, and each material can be assigned a certain
/// index using setMaterial(). But in general, the model loader
/// will take care of assigning material indices. A situation
/// multiple materials could be useful is for a tree, where the
/// leaves and the bark would have separate materials.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Model model;
/// model.load("model.dae");
///
/// // Get vertices and bounding box
/// const std::vector<Vertex>& vertices = model.getVertices();
/// const BoundingBox& box = model.getBoundingBox();
///
/// \endcode
///
///////////////////////////////////////////////////////////