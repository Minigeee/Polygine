#ifndef POLY_MODEL_H
#define POLY_MODEL_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/BoundingBox.h>
#include <poly/Math/Sphere.h>
#include <poly/Math/Vector3.h>

#include <poly/Graphics/Material.h>
#include <poly/Graphics/Renderable.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <string>
#include <vector>

namespace poly
{

class Texture;


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
	int m_material;			//!< Material index
};


///////////////////////////////////////////////////////////
/// \brief A class that contains vertex and material data
///
///////////////////////////////////////////////////////////
class Model : public Renderable
{
public:
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
	/// \param fname The model file name to load
	///
	/// \return True if the file was successfully loaded
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Create a model from a list of vertices
	///
	/// \param vertices The list of vertices
	/// \param usage The vertex buffer usage pattern
	///
	///////////////////////////////////////////////////////////
	void create(const std::vector<Vertex>& vertices, BufferUsage usage = BufferUsage::Static);

	///////////////////////////////////////////////////////////
	/// \brief Set the model vertices
	///
	/// The internal vertex buffer will be updated every time
	/// this function is called.
	///
	/// \param vertices The list of vertices
	///
	///////////////////////////////////////////////////////////
	void setVertices(const std::vector<Vertex>& vertices);

	///////////////////////////////////////////////////////////
	/// \brief Set the material
	///
	/// Each model can have more than one material, so use
	/// \a index to specify which material to set.
	///
	/// \param material The material to set
	/// \param index The material index to set
	///
	///////////////////////////////////////////////////////////
	void setMaterial(const Material& material, Uint32 index = 0);

	///////////////////////////////////////////////////////////
	/// \brief Get the vertex array used to render the model
	///
	/// \return A vertex array
	///
	///////////////////////////////////////////////////////////
	VertexArray& getVertexArray();

	///////////////////////////////////////////////////////////
	/// \brief Get the list of vertices
	///
	/// \return The list of vertices
	///
	///////////////////////////////////////////////////////////
	const std::vector<Vertex>& getVertices() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the material at the specified index
	///
	/// \param index The index of the material to retrieve
	///
	/// \return The material
	///
	///////////////////////////////////////////////////////////
	const Material& getMaterial(Uint32 index = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the list of materials
	///
	/// \return The list of materials
	///
	///////////////////////////////////////////////////////////
	const std::vector<Material>& getMaterials() const;

private:
	VertexArray m_vertexArray;				//!< The vertex array used to render the model
	VertexBuffer m_vertexBuffer;			//!< The vertex buffer used to store the main vertex data
	VertexBuffer m_skeletalVertexBuffer;	//!< The vertex buffer used to store skeletal vertex data

	std::vector<Vertex> m_vertices;			//!< The list of vertex data excluding skeletal data
	std::vector<Material> m_materials;		//!< The list of materials
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