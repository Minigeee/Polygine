#include <poly/Graphics/ParticleSystem.h>

#include <poly/Graphics/Shaders/particles/particle.vert.h>
#include <poly/Graphics/Shaders/particles/particle.geom.h>
#include <poly/Graphics/Shaders/particles/particle.frag.h>


namespace poly
{


///////////////////////////////////////////////////////////
Shader& getDefaultParticleShader()
{
	static Shader shader;

	if (!shader.getId())
	{
		shader.load("poly/particles/particle.vert", SHADER_PARTICLES_PARTICLE_VERT, Shader::Vertex);
		shader.load("poly/particles/particle.geom", SHADER_PARTICLES_PARTICLE_GEOM, Shader::Geometry);
		shader.load("poly/particles/particle.frag", SHADER_PARTICLES_PARTICLE_VERT, Shader::Fragment);
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
	m_type			(0)
{

}


}