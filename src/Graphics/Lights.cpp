#include <poly/Graphics/Lights.h>
#include <poly/Graphics/Shader.h>

#include <string>

namespace poly
{


///////////////////////////////////////////////////////////
DirectionLight::DirectionLight() :
	m_direction		(0.0f, -1.0f, 0.0f)
{
	m_diffuse = Vector3f(1.0f);
	m_specular = Vector3f(1.0f);
}


///////////////////////////////////////////////////////////
DirectionLight::DirectionLight(const Vector3f& d, const Vector3f& s, const Vector3f& dir) :
	m_direction		(0.0f, -1.0f, 0.0f)
{
	m_diffuse = d;
	m_specular = s;
}


///////////////////////////////////////////////////////////
void DirectionLight::apply(Shader* shader, Uint32 id)
{
	std::string prefix = "u_dirLights[" + std::to_string(id) + "].";
	shader->setUniform(prefix + "diffuse", m_diffuse);
	shader->setUniform(prefix + "specular", m_specular);
	shader->setUniform(prefix + "direction", normalize(m_direction));
}


}