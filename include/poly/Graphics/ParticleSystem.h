#ifndef POLY_PARTICLE_SYSTEM_H
#define POLY_PARTICLE_SYSTEM_H

#include <poly/Core/Clock.h>
#include <poly/Core/DataTypes.h>
#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <functional>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief Get a reference to the default particle shader
///
///////////////////////////////////////////////////////////
Shader& getDefaultParticleShader();


///////////////////////////////////////////////////////////
/// \brief A particle type with its most basic properties
///
///////////////////////////////////////////////////////////
struct Particle
{
	Particle();

	Vector3f m_position;	//!< The particle position in 3D space
	float m_rotation;		//!< The rotation of the particle in degrees
	Vector2f m_size;		//!< The 2D size of the particle in world space units
	Vector4f m_color;		//!< The color of the particle
	Vector4f m_textureRect;	//!< The subrectangle of the texture to display on the particle

	Vector3f m_velocity;	//!< The velocity of the particle
	float m_age;			//!< The age of the particle in seconds
};


///////////////////////////////////////////////////////////
/// \brief A system of particle effects where processing occurs on CPU
///
///////////////////////////////////////////////////////////
template <typename T>
class CpuParticles : public RenderSystem
{
public:
	CpuParticles();

	void init(Scene* scene) override;

	void render(Camera& camera, RenderPass pass) override;

	void addParticle(const T& particle);

	template <typename Func>
	void update(Func&& func);

	void setTexture(Texture* texture);

	void setShader(Shader* shader);

private:
	Clock m_clock;
	Texture* m_texture;
	Shader* m_shader;

	Uint32 m_bufferSize;
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	std::vector<T> m_particles;
};

}

#include <poly/Graphics/ParticleSystem.inl>

#endif