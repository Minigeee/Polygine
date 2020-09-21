#ifndef POLY_SHADER_H
#define POLY_SHADER_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Vector2.h>
#include <poly/Math/Vector3.h>
#include <poly/Math/Vector4.h>
#include <poly/Math/Matrix2.h>
#include <poly/Math/Matrix3.h>
#include <poly/Math/Matrix4.h>

namespace poly
{

class Texture;

///////////////////////////////////////////////////////////
/// \brief A shader class that controls render behavior
///
///////////////////////////////////////////////////////////
class Shader
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The type of shader being loaded
	///
	///////////////////////////////////////////////////////////
	enum Type
	{
		Vertex		= 0x8B31,	//!< For vertex shaders
		Geometry	= 0x8DD9,	//!< For geometry shaders
		Fragment	= 0x8B30	//!< For fragment shaders
	};

public:

#ifndef DOXYGEN_SKIP
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) = default;
	Shader& operator=(Shader&&) = default;
#endif

	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	/// This will not create the shader program
	///
	///////////////////////////////////////////////////////////
	Shader();

	///////////////////////////////////////////////////////////
	/// \brief The destructor
	///
	/// This will destroy the shader program, if it exists. It
	/// will not destry the shaders used to link the program however,
	/// as those may have been reused in other shader programs.
	///
	///////////////////////////////////////////////////////////
	~Shader();

	///////////////////////////////////////////////////////////
	/// \brief Bind the shader program
	///
	/// Binding the shader program will cause all further OpenGL
	/// operations, including rendering, to use this shader. Binding
	/// is also necessary in order for uniforms (shader variables) to be set.
	///
	///////////////////////////////////////////////////////////
	void bind();

	///////////////////////////////////////////////////////////
	/// \brief Load a shader of a specific type
	///
	/// This will load and compile the specified shader, but does
	/// not link the shader to the shader program. This shader program
	/// will still be invalid until compile() is successfully called.
	/// If the specified shader has been loaded before, the shader
	/// that has already been loaded will be used.
	///
	/// \param fname The path to the shader file
	/// \param type The type of shader being loaded
	///
	/// \return True if the shader was successfully loaded and compiled
	///
	/// \see compile
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname, Type type);

	///////////////////////////////////////////////////////////
	/// \brief Link all shaders that have been loaded
	///
	/// Linking the loaded shaders is necessary for this shader program
	/// to be useable. All shaders that have been loaded using load()
	/// will be linked into the final shader program.
	///
	/// \return True if the shaders successfully link
	///
	/// \see load
	///
	///////////////////////////////////////////////////////////
	bool compile();

	///////////////////////////////////////////////////////////
	/// \brief Set the value of a uniform (shader variable)
	///
	/// \param name The uniform name
	/// \param value The value to assign the uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, int value);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of a uniform (shader variable)
	///
	/// \param name The uniform name
	/// \param value The value to assign the uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, float value);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of a uniform (shader variable)
	///
	/// \param name The uniform name
	/// \param value The value to assign the uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const Vector2f& value);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of a uniform (shader variable)
	///
	/// \param name The uniform name
	/// \param value The value to assign the uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const Vector3f& value);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of a uniform (shader variable)
	///
	/// \param name The uniform name
	/// \param value The value to assign the uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const Vector4f& value);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of a uniform (shader variable)
	///
	/// \param name The uniform name
	/// \param value The value to assign the uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const Matrix2f& value);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of a uniform (shader variable)
	///
	/// \param name The uniform name
	/// \param value The value to assign the uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const Matrix3f& value);

	///////////////////////////////////////////////////////////
	/// \brief Set the value of a uniform (shader variable)
	///
	/// \param name The uniform name
	/// \param value The value to assign the uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const Matrix4f& value);

	///////////////////////////////////////////////////////////
	/// \brief Set the values of a uniform (shader variable) array
	///
	/// \param name The uniform array name
	/// \param values A list of values to assign the uniform array
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const std::vector<int>& values);

	///////////////////////////////////////////////////////////
	/// \brief Set the values of a uniform (shader variable) array
	///
	/// \param name The uniform array name
	/// \param values A list of values to assign the uniform array
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const std::vector<float>& values);

	///////////////////////////////////////////////////////////
	/// \brief Set the values of a uniform (shader variable) array
	///
	/// \param name The uniform array name
	/// \param values A list of values to assign the uniform array
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const std::vector<Vector2f>& values);

	///////////////////////////////////////////////////////////
	/// \brief Set the values of a uniform (shader variable) array
	///
	/// \param name The uniform array name
	/// \param values A list of values to assign the uniform array
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const std::vector<Vector3f>& values);

	///////////////////////////////////////////////////////////
	/// \brief Set the values of a uniform (shader variable) array
	///
	/// \param name The uniform array name
	/// \param values A list of values to assign the uniform array
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const std::vector<Vector4f>& values);

	///////////////////////////////////////////////////////////
	/// \brief Set the values of a uniform (shader variable) array
	///
	/// \param name The uniform array name
	/// \param values A list of values to assign the uniform array
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const std::vector<Matrix2f>& values);

	///////////////////////////////////////////////////////////
	/// \brief Set the values of a uniform (shader variable) array
	///
	/// \param name The uniform array name
	/// \param values A list of values to assign the uniform array
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const std::vector<Matrix3f>& values);

	///////////////////////////////////////////////////////////
	/// \brief Set the values of a uniform (shader variable) array
	///
	/// \param name The uniform array name
	/// \param values A list of values to assign the uniform array
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, const std::vector<Matrix4f>& values);

	///////////////////////////////////////////////////////////
	/// \brief Assign a texture to a sampler uniform
	///
	/// \param name The sampler uniform name
	/// \param texture The texture to assign to the sampler uniform
	///
	///////////////////////////////////////////////////////////
	void setUniform(const std::string& name, Texture& texture);

	///////////////////////////////////////////////////////////
	/// \brief Get the internal shader program id
	///
	/// This returns the id that is used by OpenGL
	///
	/// \return The internal shader program id
	///
	///////////////////////////////////////////////////////////
	Uint32 getId() const;

private:
	int getUniformLocation(const std::string& name);

private:
	Uint32 m_id;							//!< The program id
	std::vector<Uint32> m_shaders;			//!< A list of shader ids
	HashMap<std::string, int> m_uniforms;	//!< A map of uniform names to uniform locations
	HashMap<Uint32, Uint32> m_textures;		//!< A map of texture ids to texture slot

	static Uint32 currentBound;
	static HashMap<std::string, Uint32> loadedShaders;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Shader
/// \ingroup Graphics
///
/// A Shader program controls the way a mesh is rendered, from
/// any transformations applied to the mesh vertices, to the way
/// mesh materials, colros, and lighting are displayed.
///
/// This is a wrapper around OpenGL shaders, and GLSL is used to
/// create vertex, geometry, and fragment shaders. These shaders
/// must be stored in seperate files and loaded into the final
/// shader program using load(). After loading all the desired
/// shaders, the loaded shader must be linked together into the
/// final program using compile().
///
/// After compiling the shader program, the program can now be used
/// to determine the behavior of how meshes are rendered. To
/// use the shader program, use bind(). This will cause all
/// following meshes being rendered to use this shader, until
/// another shader program is bound.
///
/// Communication from the main program to the shader program
/// is achieved through the use of uniforms, or shader variables.
/// It is possible to set the value of a uniform from the main
/// program using setUniform().
///
/// Knowledge of GLSL is recommended before using shaders.
///
/// Usage example:
///
/// \code
///
/// // default.vert
///
/// #version 330 core
///
/// layout(location = 0) in vec3 aPos;
/// 
/// void main()
/// {
///		// Don't perform any transofrmations on the vertices
/// 	gl_Position = vec4(aPos, 1.0);
/// }
///
/// \endcode
///
/// \code
///
/// // default.frag
///
/// #version 330 core
///
/// out vec4 FragColor;
/// 
/// void main()
/// {
///		// Set each pixel color
/// 	FragColor = vec4(0.2, 0.2, 0.3, 1.0);
/// }
///
/// \endcode
///
/// \code
/// 
/// using namespace poly;
///
/// // Load the shader
/// Shader shader;
/// shader.load("default.vert", Shader::Vertex);
/// shader.load("default.frag", Shader::Fragment);
/// shader.compile();
///
/// // Use the shader
/// shader.bind();
///
/// \endcode
///
///////////////////////////////////////////////////////////