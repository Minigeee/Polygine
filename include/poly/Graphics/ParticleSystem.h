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

class ParticleSystem;


class ParticleType
{
	friend ParticleSystem;

public:
	ParticleType();

	void setUpdateShader(const std::string& fname);

	void setUpdateFunc(const std::function<void(Shader&)>& func);

	void setTexture(Texture* texture);

	template <typename T>
	void addField(const std::string& name);

private:
	struct ParticleField
	{
		std::string m_uniform;
		Uint32 m_offset;
		Uint32 m_size;
		bool m_isFloatType;
	};

private:
	Shader m_shader;
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffers[2];
	Texture* m_texture;

	std::vector<ParticleField> m_fields;
	std::function<void(Shader&)> m_updateFunc;

	Uint32 m_particleSize;
	Uint32 m_numParticles;
	Uint32 m_currentBuffer;
};


class ParticleSystem : public RenderSystem
{
public:
	ParticleSystem();

	~ParticleSystem();

	void init(Scene* scene) override;

	void render(Camera& camera) override;

	void update(float dt);

	template <typename T>
	void addParticleType();

	template <typename T>
	void addParticle(const T& particle);

	template <typename T>
	void setUpdateShader(const std::string& fname);

	template <typename T>
	void setUpdateFunc(const std::function<void(Shader&)>& func);

	template <typename T>
	void setTexture(Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Add a particle field to a particle type
	///
	/// These fields can represent particle position, size, rotation,
	/// color, or any custom per-particle data that is needed for
	/// the particle shader to work correctly. The order the fields
	/// are added must match the order they are defined in the particle
	/// struct, and this order must match the order of the vertex
	/// shader inputs. There are six fields that are required for
	/// every particle type, and these fields must be added first before
	/// any other custom fields are added:
	///
	/// \li Position
	/// \li Size
	/// \li Rotation
	/// \li Color
	/// \li Texture Coord
	/// \li Texture Size
	///
	/// The output variable names of the previous fields must be defined as:
	///
	/// \li g_position
	/// \li g_size
	/// \li g_rotation
	/// \li g_color
	/// \li g_texCoord
	/// \li g_texSize
	///
	/// Where the "g_" prefix indicates that the variable is an output variable
	/// that comes from the geometry shader.
	///
	/// Also, all particle fields must be added before setting the particle
	/// update shader, so the shader compiler knows which output variables
	/// to capture.
	///
	/// Usage example:
	/// \code
	///
	/// using namespace poly;
	///
	/// struct FireParticle
	/// {
	///		// The following 6 fields must be defined in this order
	///		Vector3f m_position;
	///		Vector2f m_size;
	///		float m_rotation;
	///		Vector4f m_color;
	///		Vector2f m_texCoord;
	///		Vector2f m_texSize;
	///
	///		// Extra particle fields
	///		int m_type;
	///		Vector3f m_velocity;
	///		float m_angularVelocity;
	///		float m_elapsed;
	/// };
	///
	/// ParticleSystem system;
	///
	/// // The fields must be added in the order they are defined
	/// system.addField<FireParticle, Vector3f>("g_position");
	/// system.addField<FireParticle, Vector2f>("g_size");
	/// system.addField<FireParticle, float>("g_rotation");
	/// system.addField<FireParticle, Vector4f>("g_color");
	/// system.addField<FireParticle, Vector2f>("g_texCoord");
	/// system.addField<FireParticle, Vector2f>("g_texSize");
	/// system.addField<FireParticle, int>("g_type");
	/// system.addField<FireParticle, Vector3f>("g_velocity");
	/// system.addField<FireParticle, float>("g_angularVelocity");
	/// system.addField<FireParticle, float>("g_elapsed");
	///
	/// // After adding all the particle fields, set the update shader
	/// system.setUpdateUniform<FireParticle>("shaders/particles/fire.geom");
	///
	///
	/// \endcode
	///
	/// \tparam T The particle type
	/// \tparam F The particle field type
	///
	/// \param name The output variable name of the field
	///
	///////////////////////////////////////////////////////////
	template <typename T, typename F>
	void addField(const std::string& name);

	template <typename T>
	Uint32 getNumParticles() const;

private:
	static Shader s_shader;

	Shader& getShader();

private:
	ObjectPool m_typePool;
	HashMap<Uint32, ParticleType*> m_particleTypes;
};

}

#include <poly/Graphics/ParticleSystem.inl>

#endif