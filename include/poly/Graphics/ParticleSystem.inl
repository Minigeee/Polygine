#include <poly/Core/TypeInfo.h>

namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline void ParticleType::addField(const std::string& name)
{
	// Add field
	ParticleField field;
	field.m_uniform = name;
	field.m_offset = m_particleSize;
	field.m_size = sizeof(T);
	field.m_isFloatType = false;
	m_fields.push_back(field);

	// Update particle size
	m_particleSize += sizeof(T);
}


///////////////////////////////////////////////////////////
template <>
inline void ParticleType::addField<float>(const std::string& name)
{
	// Add field
	ParticleField field;
	field.m_uniform = name;
	field.m_offset = m_particleSize;
	field.m_size = sizeof(float);
	field.m_isFloatType = true;
	m_fields.push_back(field);

	// Update particle size
	m_particleSize += sizeof(float);
}


///////////////////////////////////////////////////////////
template <>
inline void ParticleType::addField<Vector2f>(const std::string& name)
{
	// Add field
	ParticleField field;
	field.m_uniform = name;
	field.m_offset = m_particleSize;
	field.m_size = sizeof(Vector2f);
	field.m_isFloatType = true;
	m_fields.push_back(field);

	// Update particle size
	m_particleSize += sizeof(Vector2f);
}


///////////////////////////////////////////////////////////
template <>
inline void ParticleType::addField<Vector3f>(const std::string& name)
{
	// Add field
	ParticleField field;
	field.m_uniform = name;
	field.m_offset = m_particleSize;
	field.m_size = sizeof(Vector3f);
	field.m_isFloatType = true;
	m_fields.push_back(field);

	// Update particle size
	m_particleSize += sizeof(Vector3f);
}


///////////////////////////////////////////////////////////
template <>
inline void ParticleType::addField<Vector4f>(const std::string& name)
{
	// Add field
	ParticleField field;
	field.m_uniform = name;
	field.m_offset = m_particleSize;
	field.m_size = sizeof(Vector4f);
	field.m_isFloatType = true;
	m_fields.push_back(field);

	// Update particle size
	m_particleSize += sizeof(Vector4f);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void ParticleSystem::addParticleType()
{
	// Don't add particle types twice
	if (m_particleTypes.find(TypeInfo::getId<T>()) != m_particleTypes.end()) return;

	// Create particle type object
	ParticleType* type = (ParticleType*)m_typePool.alloc();
	new(type)ParticleType();

	// Create vertex buffers, with initial size 500
	type->m_vertexBuffers[0].create((T*)0, 500, BufferUsage::Dynamic);
	type->m_vertexBuffers[1].create((T*)0, 500, BufferUsage::Dynamic);

	// Create vertex array
	type->m_vertexArray.setDrawMode(DrawMode::Points);
	type->m_vertexArray.setNumVertices(1);

	// Add to type map
	m_particleTypes[TypeInfo::getId<T>()];
}


///////////////////////////////////////////////////////////
template <typename T>
inline void ParticleSystem::addParticle(const T& particle)
{
	// Get the type struct
	auto it = m_particleTypes.find(TypeInfo::getId<T>());
	if (it == m_particleTypes.end()) return;

	ParticleType* type = it.value();
	type->m_vertexBuffers[type->m_currentBuffer].update(&particle, 1, type->m_numInstances++);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void ParticleSystem::setUpdateShader(const std::string& fname)
{
	// Get the type struct
	auto it = m_particleTypes.find(TypeInfo::getId<T>());
	if (it == m_particleTypes.end())
	{
		// If it dosn't exist, add it
		addParticleType<T>();
		it = m_particleTypes.find(TypeInfo::getId<T>());
	}

	it.value().setUpdateShader(fname);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void ParticleSystem::setUpdateFunc(const std::function<void(Shader&)>& func)
{
	// Get the type struct
	auto it = m_particleTypes.find(TypeInfo::getId<T>());
	if (it == m_particleTypes.end())
	{
		// If it dosn't exist, add it
		addParticleType<T>();
		it = m_particleTypes.find(TypeInfo::getId<T>());
	}

	it.value().setUpdateFunc(func);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void ParticleSystem::setTexture(Texture* texture)
{
	// Get the type struct
	auto it = m_particleTypes.find(TypeInfo::getId<T>());
	if (it == m_particleTypes.end())
	{
		// If it dosn't exist, add it
		addParticleType<T>();
		it = m_particleTypes.find(TypeInfo::getId<T>());
	}

	it.value().setTexture(texture);
}


///////////////////////////////////////////////////////////
template <typename T, typename F>
inline void ParticleSystem::addField(const std::string& name)
{
	// Get the type struct
	auto it = m_particleTypes.find(TypeInfo::getId<T>());
	if (it == m_particleTypes.end())
	{
		// If it dosn't exist, add it
		addParticleType<T>();
		it = m_particleTypes.find(TypeInfo::getId<T>());
	}

	it.value().addField<F>(uniform);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 ParticleSystem::getNumParticles() const
{
	// Get the type struct
	auto it = m_particleTypes.find(TypeInfo::getId<T>());
	if (it == m_particleTypes.end()) return 0;

	return it.value().m_numParticles;
}


}