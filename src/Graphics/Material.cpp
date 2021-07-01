#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Material.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Texture.h>

#include <string>


namespace poly
{


///////////////////////////////////////////////////////////
Material::Material() :
	m_ambient		(1.0f),
	m_diffuse		(1.0f),
	m_specular		(0.1f),
	m_shininess		(16.0f),
	m_isTransparent	(false),
	m_cullFace		(true),
	m_diffTexture	(0),
	m_specTexture	(0),
	m_normalTexture	(0),
	m_renderMask	(RenderPass::All)
{ }


///////////////////////////////////////////////////////////
void Material::setAmbientFactor(float factor)
{
	m_ambient = factor;
}


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
void Material::setTransparent(bool transparent)
{
	m_isTransparent = transparent;
}


///////////////////////////////////////////////////////////
void Material::setCullFace(bool cull)
{
	m_cullFace = cull;
}


///////////////////////////////////////////////////////////
void Material::setDiffTexture(Texture* texture)
{
	m_diffTexture = texture;
}


///////////////////////////////////////////////////////////
void Material::setSpecTexture(Texture* texture)
{
	m_specTexture = texture;
}


///////////////////////////////////////////////////////////
void Material::setNormalTexture(Texture* texture)
{
	m_normalTexture = texture;
}


///////////////////////////////////////////////////////////
void Material::setRenderMask(RenderPass mask)
{
	m_renderMask = mask;
}


///////////////////////////////////////////////////////////
void Material::setApplyFunc(const std::function<void(Shader*)>& func)
{
	m_applyFunc = func;
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
float Material::getAmbientFactor()
{
	return m_ambient;
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
bool Material::isTransparent() const
{
	return m_isTransparent;
}


///////////////////////////////////////////////////////////
bool Material::getCullFace() const
{
	return m_cullFace;
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
RenderPass Material::getRenderMask() const
{
	return m_renderMask;
}


///////////////////////////////////////////////////////////
void Material::apply(Shader* shader) const
{
	std::string prefix = "u_material.";
	shader->setUniform(prefix + "ambient", m_ambient);
	shader->setUniform(prefix + "diffuse", m_diffuse);
	shader->setUniform(prefix + "specular", m_specular);
	shader->setUniform(prefix + "shininess", m_shininess);
	shader->setUniform(prefix + "hasDiffTexture", (int)m_diffTexture);
	shader->setUniform(prefix + "hasSpecTexture", (int)m_specTexture);
	shader->setUniform(prefix + "hasNormalTexture", (int)m_normalTexture);

	// Bind diffuse texture
	if (m_diffTexture)
		shader->setUniform("u_diffuseMap", *m_diffTexture);

	// Bind specular texture
	if (m_specTexture)
		shader->setUniform("u_specularMap", *m_specTexture);

	// Bind normal texture
	if (m_normalTexture)
		shader->setUniform("u_normalMap", *m_normalTexture);

	// Apply all custom textures
	auto it = m_textures.begin();
	for (int i = 2; it != m_textures.end(); ++it, ++i)
	{
		// Bind texture
		Texture* texture = it->second;
		texture->bind(i);

		// Set sampler index
		shader->setUniform(it->first, i);
	}

	// Cull face
	if (m_cullFace)
	{
		glCheck(glEnable(GL_CULL_FACE));
		glCheck(glCullFace(GL_BACK));
	}
	else
		glCheck(glDisable(GL_CULL_FACE));

	// Apply function callback
	if (m_applyFunc)
		m_applyFunc(shader);
}


}