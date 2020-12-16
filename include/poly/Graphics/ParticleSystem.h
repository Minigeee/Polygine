#ifndef POLY_PARTICLE_SYSTEM_H
#define POLY_PARTICLE_SYSTEM_H

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
/// \brief A struct representing a particle
///
///////////////////////////////////////////////////////////
struct Particle
{
	int m_type;					//!< The type of particle (standard or emitter)
	Vector3f m_position;		//!< The position of the particle
	Vector2f m_size;			//!< The size of the particle quad
	float m_rotation;			//!< The rotation of the particle in degrees (along the z-axis relative to the camera)
	Vector2f m_texCoords;		//!< The top left position of the texture rectangle (in texture coordinate space)
	Vector2f m_texSize;			//!< The size of the texture rectangle (in texture coordinate space)
	Colorf m_color;				//!< The color of the particle
	Vector3f m_velocity;		//!< The velocity of the particle
	float m_angularVelocity;	//!< The rotational velocity of the particle
	float m_elapsed;			//!< The amount of time elapsed since the particle was emitted
};


class ParticleSystem : public RenderSystem
{
public:
	ParticleSystem();

	~ParticleSystem();

	void init(Scene* scene) override;

	void update(float dt);

	void render(Camera& camera) override;

	void addParticleType(
		const std::string& shader,
		Uint32 maxParticles,
		Texture* texture = 0,
		std::function<void(Shader&)>&& update = std::function<void(Shader&)>()
	);

	void createEmitter(const std::string& shader, const Particle& emitter);

private:
	struct ParticleType
	{
		Shader m_shader;
		VertexArray m_vertexArray;
		VertexBuffer m_vertexBuffers[2];
		Texture* m_texture;
		Uint32 m_numInstances;
		Uint32 m_currentBuffer;

		std::function<void(Shader&)> m_updateFunc;
	};

	static VertexArray s_vertexArray;
	static Shader s_shader;

	VertexArray& getVertexArray();

	Shader& getShader();

private:
	ObjectPool m_typePool;
	HashMap<std::string, ParticleType*> m_particleTypes;
};

}

#endif