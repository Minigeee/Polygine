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

class Shader
{
public:
	enum Type
	{
		Vertex		= 0x8B31,
		Geometry	= 0x8DD9,
		Fragment	= 0x8B30
	};

public:
	Shader();
	~Shader();

	void bind();

	bool load(const std::string& fname, Type type);

	bool compile();

	void setUniform(const std::string& name, int value);

	void setUniform(const std::string& name, float value);

	void setUniform(const std::string& name, const Vector2f& value);

	void setUniform(const std::string& name, const Vector3f& value);

	void setUniform(const std::string& name, const Vector4f& value);

	void setUniform(const std::string& name, const Matrix2f& value);

	void setUniform(const std::string& name, const Matrix3f& value);

	void setUniform(const std::string& name, const Matrix4f& value);

	void setUniform(const std::string& name, const std::vector<int>& values);

	void setUniform(const std::string& name, const std::vector<float>& values);

	void setUniform(const std::string& name, const std::vector<Vector2f>& values);

	void setUniform(const std::string& name, const std::vector<Vector3f>& values);

	void setUniform(const std::string& name, const std::vector<Vector4f>& values);

	void setUniform(const std::string& name, const std::vector<Matrix2f>& values);

	void setUniform(const std::string& name, const std::vector<Matrix3f>& values);

	void setUniform(const std::string& name, const std::vector<Matrix4f>& values);

private:
	int getUniformLocation(const std::string& name);

private:
	Uint32 m_id;
	std::vector<Uint32> m_shaders;
	HashMap<std::string, int> m_uniforms;

	static Uint32 currentBound;
	static HashMap<std::string, Uint32> loadedShaders;
};

}

#endif