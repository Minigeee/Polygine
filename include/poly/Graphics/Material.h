#ifndef POLY_MATERIAL_H
#define POLY_MATERIAL_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Vector3.h>

namespace poly
{

class Shader;
class Texture;

class Material
{
public:
	Material();

	void setDiffuse(const Vector3f& color);

	void setDiffuse(float r, float g, float b);

	void setSpecular(const Vector3f& color);

	void setSpecular(float r, float g, float b);

	void setShininess(float shininess);

	void addTexture(const std::string& uniform, Texture* texture);

	void removeTexture(const std::string& uniform);

	Vector3f& getDiffuse();

	Vector3f& getSpecular();

	float getShininess() const;

	Texture* getTexture(const std::string& uniform) const;

	void apply(Shader* shader, int index = 0) const;

private:
	Vector3f m_diffuse;
	Vector3f m_specular;
	float m_shininess;

	HashMap<std::string, Texture*> m_textures;
};

}

#endif