#include <poly/Graphics/Material.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Texture.h>

#include <string>


namespace poly
{


///////////////////////////////////////////////////////////
Material::Material() :
	m_diffuse	(1.0f),
	m_specular	(0.1f),
	m_shininess	(16.0f)
{ }


///////////////////////////////////////////////////////////
void Material::setDiffuse(const Vector3f& color)
{
	m_diffuse = color;
}


///////////////////////////////////////////////////////////
void Material::setDiffuse(float r, float g, float b)
{
	m_diffuse = Vector3f(r, g, b);
}


///////////////////////////////////////////////////////////
void Material::setSpecular(const Vector3f& color)
{
	m_specular = color;
}


///////////////////////////////////////////////////////////
void Material::setSpecular(float r, float g, float b)
{
	m_specular = Vector3f(r, g, b);
}


///////////////////////////////////////////////////////////
void Material::setShininess(float shininess)
{
	m_shininess = shininess;
}


///////////////////////////////////////////////////////////
void Material::addTexture(const std::string& uniform, Texture* texture)
{
	if (texture)
		m_textures[uniform] = texture;
}


///////////////////////////////////////////////////////////
void Material::removeTexture(const std::string& uniform)
{
	m_textures.erase(uniform);
}


///////////////////////////////////////////////////////////
Vector3f& Material::getDiffuse()
{
	return m_diffuse;
}


///////////////////////////////////////////////////////////
Vector3f& Material::getSpecular()
{
	return m_specular;
}


///////////////////////////////////////////////////////////
float Material::getShininess() const
{
	return m_shininess;
}


///////////////////////////////////////////////////////////
Texture* Material::getTexture(const std::string& uniform) const
{
	// Find texture
	Texture* texture = 0;
	auto it = m_textures.find(uniform);
	if (it != m_textures.end())
		texture = it->second;

	return texture;
}


///////////////////////////////////////////////////////////
void Material::apply(Shader* shader, int index) const
{
	shader->setUniform("u_materials[" + std::to_string(index) + "].diffuse", m_diffuse);
	shader->setUniform("u_materials[" + std::to_string(index) + "].specular", m_specular);
	shader->setUniform("u_materials[" + std::to_string(index) + "].shininess", m_shininess);

	auto it = m_textures.begin();
	for (int i = 0; it != m_textures.end(); ++it, ++i)
	{
		// Bind texture
		Texture* texture = it->second;
		texture->bind(i);

		// Set sampler index
		shader->setUniform(it->first, i);
	}
}


}