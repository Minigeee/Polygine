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
	Uint32 m_type;			//!< The particle type (optional)
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

	void setFields(const std::function<std::vector<Vector2u>()>& func);

private:
	void updateVertexArray();

private:
	Clock m_clock;
	Texture* m_texture;
	Shader* m_shader;

	Uint32 m_bufferSize;
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	std::vector<T> m_particles;

	std::function<std::vector<Vector2u>()> m_fieldsFunc;
};


///////////////////////////////////////////////////////////
/// \brief A system of particle effects where processing occurs on GPU
///
///////////////////////////////////////////////////////////
template <typename T>
class GpuParticles : public RenderSystem
{
public:
	GpuParticles();

	~GpuParticles();

	void init(Scene* scene) override;

	void render(Camera& camera, RenderPass pass) override;

	void addParticle(const T& particle);

	void update(const std::function<void(Shader*)>& func = std::function<void(Shader*)>());

	void setMaxParticles(Uint32 max);

	void setTexture(Texture* texture);

	void setUpdateShader(Shader* shader);

	void setRenderShader(Shader* shader);

	void setFields(const std::function<std::vector<Vector2u>()>& func);

private:
	void bindInputBuffer(VertexBuffer& buffer);

private:
	Clock m_clock;
	Texture* m_texture;
	Shader* m_updateShader;
	Shader* m_renderShader;

	Uint32 m_tfQuery;
	Uint32 m_numParticles;
	Uint32 m_bufferSize;
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffers[2];
	Uint32 m_currentBuffer;

	std::function<std::vector<Vector2u>()> m_fieldsFunc;
};


#include <poly/Core/Macros.h>


#ifndef DOXYGEN_SKIP

#define _GET_FIELD_PROPERTIES_EXPR(x) info.push_back(Vector2u((Uint32)((Uint8*)&p.x - (Uint8*)&p), ((Uint32)sizeof(p.x) + 3) / 4));

#define _GET_FIELD_PROPERTIES(...) LOOP(_GET_FIELD_PROPERTIES_EXPR, __VA_ARGS__)

#endif


///////////////////////////////////////////////////////////
/// \brief A macro used to define which particle fields should be sent to the shaders
///
/// Usage example:
/// \code
///
/// CpuParticles<Particle> particles;
/// particles.setFields(PARTICLE_FIELDS(Particle, m_position, m_rotation, m_size, m_color));
///
/// \endcode
///
///////////////////////////////////////////////////////////
#define PARTICLE_FIELDS(type, ...) \
	[]() -> std::vector<Vector2u> { std::vector<Vector2u> info; type p; _GET_FIELD_PROPERTIES(__VA_ARGS__) return info; }


}

#include <poly/Graphics/ParticleSystem.inl>

#endif