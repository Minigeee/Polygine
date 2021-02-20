#include <poly/Graphics/GLCheck.h>

namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline CpuParticles<T>::CpuParticles() :
	m_texture		(0),
	m_shader		(0),
	m_bufferSize	(0)
{

}


///////////////////////////////////////////////////////////
template <typename T>
inline void CpuParticles<T>::init(Scene* scene)
{
	// Create vertex buffer with default size
	m_vertexBuffer.create((T*)NULL, 256, BufferUsage::Stream);
	m_bufferSize = 256;

	// Create dummy particle to get property offsets
	T particle;

	// Create vertex array and add required properties
	m_vertexArray.addBuffer(m_vertexBuffer, 0, 3, sizeof(T), (Uint32)((Uint8*)&particle.m_position - (Uint8*)&particle));
	m_vertexArray.addBuffer(m_vertexBuffer, 1, 1, sizeof(T), (Uint32)((Uint8*)&particle.m_rotation - (Uint8*)&particle));
	m_vertexArray.addBuffer(m_vertexBuffer, 2, 2, sizeof(T), (Uint32)((Uint8*)&particle.m_size - (Uint8*)&particle));
	m_vertexArray.addBuffer(m_vertexBuffer, 3, 4, sizeof(T), (Uint32)((Uint8*)&particle.m_color - (Uint8*)&particle));
	m_vertexArray.addBuffer(m_vertexBuffer, 4, 4, sizeof(T), (Uint32)((Uint8*)&particle.m_textureRect - (Uint8*)&particle));

	// Render as points
	m_vertexArray.setDrawMode(DrawMode::Points);

	// Start with zero particles
	m_vertexArray.setNumVertices(0);

	// Use default shader if one isn't provided
	if (!m_shader)
		m_shader = &getDefaultParticleShader();
}


///////////////////////////////////////////////////////////
template <typename T>
inline void CpuParticles<T>::render(Camera& camera, RenderPass pass)
{
	// Only render for default pass
	if (pass != RenderPass::Default) return;

	// Bind shader
	m_shader->bind();
	m_shader->setUniform("u_cameraPos", camera.getPosition());
	m_shader->setUniform("u_projView", camera.getProjMatrix() * camera.getViewMatrix());

	// Bind texture
	if (m_texture)
	{
		m_shader->setUniform("u_texture", *m_texture);
		m_shader->setUniform("u_hasTexture", true);
	}
	else
		m_shader->setUniform("u_hasTexture", false);

	// Enable depth testing
	glCheck(glEnable(GL_DEPTH_TEST));

	// Draw particles
	m_vertexArray.draw();
}


///////////////////////////////////////////////////////////
template <typename T>
inline void CpuParticles<T>::addParticle(const T& particle)
{
	// Add the particle
	m_particles.push_back(particle);

	// Check if the vertex buffer needs to be expanded
	if (m_particles.size() > m_bufferSize)
	{
		// Recreate the buffer with the particle array's capacity
		m_vertexBuffer.create((T*)NULL, m_particles.capacity(), BufferUsage::Stream);

		// Update data
		m_vertexBuffer.update(m_particles);

		// Update size
		m_bufferSize = m_particles.capacity();
	}

	// Update number of particles
	m_vertexArray.setNumVertices(m_particles.size());
}


///////////////////////////////////////////////////////////
template <typename T>
template <typename Func>
inline void CpuParticles<T>::update(Func&& func)
{
	// Keep track of the list of particles to remove
	std::vector<Uint32> removeIndices;

	// Get elapsed time
	float dt = m_clock.restart().toSeconds();

	// Iterate through particles and update each
	for (Uint32 i = 0; i < m_particles.size(); ++i)
	{
		Particle& particle = m_particles[i];

		if (!func(dt, particle))
			// Remove particles that shouldn't be kept
			removeIndices.push_back(i);

		// Update particle age after (so particles that were just added have an age of zero)
		particle.m_age += dt;
	}

	// Remove particles from back to front, using swap-pop
	for (int i = (int)removeIndices.size() - 1; i >= 0; --i)
	{
		m_particles[removeIndices[i]] = m_particles.back();
		m_particles.pop_back();
	}

	// Push new particles to vertex buffer
	m_vertexBuffer.update(m_particles);

	// Update the number of particles
	m_vertexArray.setNumVertices(m_particles.size());
}


///////////////////////////////////////////////////////////
template <typename T>
inline void CpuParticles<T>::setTexture(Texture* texture)
{
	m_texture = texture;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void CpuParticles<T>::setShader(Shader* shader)
{
	m_shader = shader;
}


}