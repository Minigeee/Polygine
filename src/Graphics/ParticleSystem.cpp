#include <poly/Graphics/ParticleSystem.h>


namespace poly
{


///////////////////////////////////////////////////////////
Shader& getDefaultParticleShader()
{
	static Shader shader;

	if (!shader.getId())
	{
		shader.load("shaders/particles/particle.vert", Shader::Vertex);
		shader.load("shaders/particles/particle.geom", Shader::Geometry);
		shader.load("shaders/particles/particle.frag", Shader::Fragment);
		shader.compile();
	}

	return shader;
}


///////////////////////////////////////////////////////////
Particle::Particle() :
	m_position		(0.0f),
	m_rotation		(0.0f),
	m_size			(0.1f),
	m_color			(1.0f),
	m_textureRect	(0.0f, 0.0f, 1.0f, 1.0f),
	m_velocity		(0.0f),
	m_age			(0.0f),
	m_type			(1)
{

}


}