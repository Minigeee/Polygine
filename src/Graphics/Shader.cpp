#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Shader.h>

namespace poly
{


///////////////////////////////////////////////////////////
Uint32 Shader::currentBound = 0;

///////////////////////////////////////////////////////////
HashMap<std::string, Uint32> Shader::loadedShaders;

///////////////////////////////////////////////////////////
#ifdef USE_COLUMN_MAJOR
GLboolean transpose = GL_FALSE;
#else
GLboolean shouldTranspose = GL_TRUE;
#endif


///////////////////////////////////////////////////////////
Shader::Shader() :
	m_id		(0)
{

}


///////////////////////////////////////////////////////////
Shader::~Shader()
{
	if (m_id)
		glCheck(glDeleteProgram(m_id));

	m_id = 0;
}


///////////////////////////////////////////////////////////
void Shader::bind()
{
	ASSERT(m_id, "Attempted to bind an uncompiled shader");

	// Only bind if it isn't bound
	if (currentBound != m_id)
	{
		glCheck(glUseProgram(m_id));
		currentBound = m_id;
	}
}


///////////////////////////////////////////////////////////
bool Shader::load(const std::string& fname, Shader::Type type)
{
	// First check if the file has already been loaded
	{
		auto it = loadedShaders.find(fname);
		if (it != loadedShaders.end())
		{
			// Add the shader object
			m_shaders.push_back(it->second);

			return true;
		}
	}

	// Open file
	std::ifstream f(fname);
	if (!f.is_open())
	{
		LOG_ERROR("Could not open shader file: %s", fname.c_str());
		return false;
	}

	// Read shader code
	std::string code;

	f.seekg(0, std::ios::end);
	code.reserve((Uint32)f.tellg());
	f.seekg(0, std::ios::beg);

	code.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());

	// Close file
	f.close();

	// Create shader
	Uint32 shader = 0;
	const char* src = code.c_str();
	glCheck(shader = glCreateShader(type));
	glCheck(glShaderSource(shader, 1, &src, NULL));
	glCheck(glCompileShader(shader));

	// Check status
	int success;
	char infoLog[512];
	glCheck(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		glCheck(glGetShaderInfoLog(shader, 512, NULL, infoLog));
		LOG_ERROR("Failed to compile %s:\n%s", fname.c_str(), infoLog);
		glCheck(glDeleteShader(shader));
		return false;
	}

	// Add shader
	m_shaders.push_back(shader);

	// Add to loaded map
	loadedShaders[fname] = shader;

	LOG("Loaded shader: %s", fname.c_str());
	return true;
}


///////////////////////////////////////////////////////////
bool Shader::compile()
{
	// Don't compile if already compiled before
	if (m_id) return true;

	// Create program
	glCheck(m_id = glCreateProgram());

	// Attach shaders
	for (Uint32 i = 0; i < m_shaders.size(); ++i)
		glAttachShader(m_id, m_shaders[i]);

	// Link the program
	glCheck(glLinkProgram(m_id));

	// Check status
	int success;
	char infoLog[512];
	glCheck(glGetProgramiv(m_id, GL_LINK_STATUS, &success));
	if (!success)
	{
		glCheck(glGetProgramInfoLog(m_id, 512, NULL, infoLog));
		LOG_ERROR("Failed to link program:\n%s", infoLog);
		glCheck(glDeleteProgram(m_id));
		return false;
	}

	return true;
}


///////////////////////////////////////////////////////////
int Shader::getUniformLocation(const std::string& name)
{
	// First check if the uniform has been found
	auto it = m_uniforms.find(name);

	if (it != m_uniforms.end())
		// Return the location
		return it->second;

	else
	{
		// Find the location
		int location = -1;
		glCheck(location = glGetUniformLocation(m_id, name.c_str()));

		// Check if it was found
		if (location == -1)
			LOG_WARNING("Could not find shader uniform: %s", name.c_str());

		// Add it to the map
		return (m_uniforms[name] = location);
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, int value)
{
	int location = getUniformLocation(name);
	if (location != -1)
		glCheck(glUniform1iv(location, 1, &value));
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, float value)
{
	int location = getUniformLocation(name);
	if (location != -1)
		glCheck(glUniform1fv(location, 1, &value));
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Vector2f& value)
{
	int location = getUniformLocation(name);
	if (location != -1)
		glCheck(glUniform2fv(location, 1, &value.x));
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Vector3f& value)
{
	int location = getUniformLocation(name);
	if (location != -1)
		glCheck(glUniform3fv(location, 1, &value.x));
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Vector4f& value)
{
	int location = getUniformLocation(name);
	if (location != -1)
		glCheck(glUniform4fv(location, 1, &value.x));
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Matrix2f& value)
{
	int location = getUniformLocation(name);
	if (location != -1)
		glCheck(glUniformMatrix2fv(location, 1, shouldTranspose, &value.x.x));
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Matrix3f& value)
{
	int location = getUniformLocation(name);
	if (location != -1)
		glCheck(glUniformMatrix3fv(location, 1, shouldTranspose, &value.x.x));
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Matrix4f& value)
{
	int location = getUniformLocation(name);
	if (location != -1)
		glCheck(glUniformMatrix4fv(location, 1, shouldTranspose, &value.x.x));
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<int>& values)
{
	if (values.size())
	{
		int location = getUniformLocation(name);
		if (location != -1)
			glCheck(glUniform1iv(location, values.size(), &values[0]));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<float>& values)
{
	if (values.size())
	{
		int location = getUniformLocation(name);
		if (location != -1)
			glCheck(glUniform1fv(location, values.size(), &values[0]));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Vector2f>& values)
{
	if (values.size())
	{
		int location = getUniformLocation(name);
		if (location != -1)
			glCheck(glUniform2fv(location, values.size(), &values[0].x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Vector3f>& values)
{
	if (values.size())
	{
		int location = getUniformLocation(name);
		if (location != -1)
			glCheck(glUniform3fv(location, values.size(), &values[0].x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Vector4f>& values)
{
	if (values.size())
	{
		int location = getUniformLocation(name);
		if (location != -1)
			glCheck(glUniform4fv(location, values.size(), &values[0].x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Matrix2f>& values)
{
	if (values.size())
	{
		int location = getUniformLocation(name);
		if (location != -1)
			glCheck(glUniformMatrix2fv(location, values.size(), shouldTranspose, &values[0].x.x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Matrix3f>& values)
{
	if (values.size())
	{
		int location = getUniformLocation(name);
		if (location != -1)
			glCheck(glUniformMatrix3fv(location, values.size(), shouldTranspose, &values[0].x.x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Matrix4f>& values)
{
	if (values.size())
	{
		int location = getUniformLocation(name);
		if (location != -1)
			glCheck(glUniformMatrix4fv(location, values.size(), shouldTranspose, &values[0].x.x));
	}
}


}