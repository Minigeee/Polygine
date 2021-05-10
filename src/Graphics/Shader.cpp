#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Texture.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
std::string readShaderFile(const std::string& fname, HashSet<std::string>& loadedFiles)
{
	// Return if the file has already been loaded
	if (loadedFiles.contains(fname)) return "";

	// Open file
	std::ifstream f(fname);
	if (!f.is_open())
	{
		LOG_ERROR("Could not open shader file: %s", fname.c_str());
		return "";
	}

	// Read shader code
	std::string code;

	f.seekg(0, std::ios::end);
	code.reserve((Uint32)f.tellg());
	f.seekg(0, std::ios::beg);

	code.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());

	// Close file
	f.close();

	// Add the file to the set of loaded files
	loadedFiles.insert(fname);


	// Process string
	const std::string verKeyword = "#version";
	const std::string inclKeyword = "#include";

	for (Uint32 i = 0; i < code.size() - inclKeyword.size(); ++i)
	{
		// Only handle lines with '#'
		if (code[i] != '#') continue;

		// Version
		if (i > 0 && loadedFiles.size() > 0 && code.substr(i, verKeyword.size()) == verKeyword)
		{
			// Remove the line
			Uint32 keywordEnd = i;
			for (; keywordEnd < code.size(); ++keywordEnd)
			{
				if (code[keywordEnd] == '\n')
					break;
			}

			code = code.substr(0, i) + code.substr(keywordEnd + 1);
		}

		// Includes
		else if (code.substr(i, inclKeyword.size()) == inclKeyword)
		{
			Uint32 fnameStart = i + inclKeyword.size() + 2;
			Uint32 fnameEnd = fnameStart;
			for (; fnameEnd < code.size(); ++fnameEnd)
			{
				if (code[fnameEnd] == '"')
					break;
			}

			std::string inclFname = code.substr(fnameStart, fnameEnd - fnameStart);
			std::string dirPath = fname.substr(0, fname.find_last_of("/\\"));
			if (dirPath.size() == fname.size())
				dirPath = "";

			// Get absolute path (relative to working directory) of new file
			Uint32 pathSeparatorPos = (Uint32)inclFname.find_first_of("/\\");
			while (pathSeparatorPos != std::string::npos)
			{
				std::string dir = inclFname.substr(0, pathSeparatorPos);
				inclFname = inclFname.substr(pathSeparatorPos + 1);

				if (dir == "..")
				{
					Uint32 index = (Uint32)dirPath.find_last_of("/\\");
					dirPath = (index == std::string::npos ? "" : dirPath.substr(0, index));
				}
				else
					dirPath += '/' + dir;

				pathSeparatorPos = (Uint32)inclFname.find_first_of("/\\");
			}
			inclFname = dirPath + '/' + inclFname;

			// Concatenate include data
			code = code.substr(0, i) + '\n' + readShaderFile(inclFname, loadedFiles) + '\n' + code.substr(fnameEnd + 1);
		}
	}

	return code;
}


}


///////////////////////////////////////////////////////////
Uint32 Shader::currentBound = 0;

///////////////////////////////////////////////////////////
HashMap<std::string, Uint32> Shader::loadedShaders;

///////////////////////////////////////////////////////////
#ifdef USE_COLUMN_MAJOR
GLboolean shouldTranspose = GL_FALSE;
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

	// Load file (with includes)
	HashSet<std::string> loadedFiles;
	std::string code = priv::readShaderFile(fname, loadedFiles);


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
bool Shader::compile(const std::vector<const char*>& feedback)
{
	// Don't compile if already compiled before
	if (m_id) return true;

	// Create program
	glCheck(m_id = glCreateProgram());

	// Attach shaders
	for (Uint32 i = 0; i < m_shaders.size(); ++i)
		glCheck(glAttachShader(m_id, m_shaders[i]));

	// Add feedback variables
	if (feedback.size())
		glCheck(glTransformFeedbackVaryings(m_id, feedback.size(), &feedback[0], GL_INTERLEAVED_ATTRIBS));

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
Shader::UniformData& Shader::getUniformData(const std::string& name)
{
	// First check if the uniform has been found
	auto it = m_uniforms.find(name);

	if (it != m_uniforms.end())
		// Return the location
		return it.value();

	else
	{
		// Find the location
		int location = -1;
		glCheck(location = glGetUniformLocation(m_id, name.c_str()));

		// Check if it was found
		if (location == -1)
			LOG_WARNING("Could not find shader uniform: %s", name.c_str());

		// Add it to the map
		UniformData data;
		data.m_location = location;

		return (m_uniforms[name] = data);
	}
}


///////////////////////////////////////////////////////////
Uint32 Shader::getUniformBlockIndex(const std::string& name)
{
	// First check if the uniform has been found
	auto it = m_uniformBlocks.find(name);

	if (it != m_uniformBlocks.end())
		// Return the location
		return it->second;

	else
	{
		// Find the location
		Uint32 location = GL_INVALID_INDEX, index = GL_INVALID_INDEX;
		glCheck(location = glGetUniformBlockIndex(m_id, name.c_str()));

		// Check if it was found
		if (location == GL_INVALID_INDEX)
			LOG_WARNING("Could not find shader uniform block: %s", name.c_str());
		else
			glCheck(glUniformBlockBinding(m_id, location, index = m_uniformBlocks.size()));

		// Add it to the map
		return (m_uniformBlocks[name] = index);
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, int value)
{
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(int*)data.m_data != value)
	{
		glCheck(glUniform1iv(data.m_location, 1, &value));
		*(int*)data.m_data = value;
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, float value)
{
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(float*)data.m_data != value)
	{
		glCheck(glUniform1fv(data.m_location, 1, &value));
		*(float*)data.m_data = value;
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Vector2f& value)
{
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(Vector2f*)data.m_data != value)
	{
		glCheck(glUniform2fv(data.m_location, 1, &value.x));
		*(Vector2f*)data.m_data = value;
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Vector3f& value)
{
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(Vector3f*)data.m_data != value)
	{
		glCheck(glUniform3fv(data.m_location, 1, &value.x));
		*(Vector3f*)data.m_data = value;
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Vector4f& value)
{
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(Vector4f*)data.m_data != value)
	{
		glCheck(glUniform4fv(data.m_location, 1, &value.x));
		*(Vector4f*)data.m_data = value;
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Matrix2f& value)
{
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(Matrix2f*)data.m_data != value)
	{
		glCheck(glUniformMatrix2fv(data.m_location, 1, shouldTranspose, &value.x.x));
		*(Matrix2f*)data.m_data = value;
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Matrix3f& value)
{
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(Matrix3f*)data.m_data != value)
	{
		glCheck(glUniformMatrix3fv(data.m_location, 1, shouldTranspose, &value.x.x));
		*(Matrix3f*)data.m_data = value;
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const Matrix4f& value)
{
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(Matrix4f*)data.m_data != value)
	{
		glCheck(glUniformMatrix4fv(data.m_location, 1, shouldTranspose, &value.x.x));
		*(Matrix4f*)data.m_data = value;
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<int>& values)
{
	if (values.size())
	{
		UniformData& data = getUniformData(name);
		if (data.m_location != -1)
			glCheck(glUniform1iv(data.m_location, values.size(), &values[0]));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<float>& values)
{
	if (values.size())
	{
		UniformData& data = getUniformData(name);
		if (data.m_location != -1)
			glCheck(glUniform1fv(data.m_location, values.size(), &values[0]));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Vector2f>& values)
{
	if (values.size())
	{
		UniformData& data = getUniformData(name);
		if (data.m_location != -1)
			glCheck(glUniform2fv(data.m_location, values.size(), &values[0].x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Vector3f>& values)
{
	if (values.size())
	{
		UniformData& data = getUniformData(name);
		if (data.m_location != -1)
			glCheck(glUniform3fv(data.m_location, values.size(), &values[0].x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Vector4f>& values)
{
	if (values.size())
	{
		UniformData& data = getUniformData(name);
		if (data.m_location != -1)
			glCheck(glUniform4fv(data.m_location, values.size(), &values[0].x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Matrix2f>& values)
{
	if (values.size())
	{
		UniformData& data = getUniformData(name);
		if (data.m_location != -1)
			glCheck(glUniformMatrix2fv(data.m_location, values.size(), shouldTranspose, &values[0].x.x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Matrix3f>& values)
{
	if (values.size())
	{
		UniformData& data = getUniformData(name);
		if (data.m_location != -1)
			glCheck(glUniformMatrix3fv(data.m_location, values.size(), shouldTranspose, &values[0].x.x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, const std::vector<Matrix4f>& values)
{
	if (values.size())
	{
		UniformData& data = getUniformData(name);
		if (data.m_location != -1)
			glCheck(glUniformMatrix4fv(data.m_location, values.size(), shouldTranspose, &values[0].x.x));
	}
}


///////////////////////////////////////////////////////////
void Shader::setUniform(const std::string& name, Texture& texture)
{
	int slot = 0;

	// Get texture slot
	auto it = m_textures.find(texture.getId());
	if (it == m_textures.end())
		slot = (int)(m_textures[texture.getId()] = m_textures.size());

	else
		slot = (int)it->second;

	// Bind texture
	texture.bind(slot);

	// Set uniform
	UniformData& data = getUniformData(name);
	if (data.m_location != -1 && *(int*)data.m_data != slot)
	{
		glCheck(glUniform1iv(data.m_location, 1, &slot));
		*(int*)data.m_data = slot;
	}
}


///////////////////////////////////////////////////////////
void Shader::bindUniformBlock(const std::string& name, UniformBuffer& block, Uint32 offset, Uint32 size)
{
	// Get uniform block index
	Uint32 index = getUniformBlockIndex(name);

	// Bind uniform block to the index
	if (index != GL_INVALID_INDEX)
		block.bind(index, offset, size);
}


///////////////////////////////////////////////////////////
Uint32 Shader::getId() const
{
	return m_id;
}


}