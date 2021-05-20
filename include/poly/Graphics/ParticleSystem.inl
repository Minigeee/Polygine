#include <poly/Graphics/GLCheck.h>

#include <poly/Core/Profiler.h>

namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline CpuParticles<T>::CpuParticles() :
	m_texture		(0),
	m_shader		(0),
	m_bufferSize	(0)
{
	m_fieldsFunc = PARTICLE_FIELDS(T, m_position, m_rotation, m_size, m_color, m_textureRect);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void CpuParticles<T>::init(Scene* scene)
{
	// Create vertex buffer with default size
	m_vertexBuffer.create((T*)NULL, 256, BufferUsage::Stream);
	m_bufferSize = 256;

	// Create vertex array and add required properties
	updateVertexArray();

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

	// Camera
	camera.apply(m_shader);

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
		T& particle = m_particles[i];

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


///////////////////////////////////////////////////////////
template <typename T>
inline void CpuParticles<T>::setFields(const std::function<std::vector<Vector2u>()>& func)
{
	m_fieldsFunc = func;

	if (m_vertexArray.getId())
		updateVertexArray();
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 CpuParticles<T>::getNumParticles() const
{
	return m_particles.size();
}


///////////////////////////////////////////////////////////
template <typename T>
inline Texture* CpuParticles<T>::getTexture() const
{
	return m_texture;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Shader* CpuParticles<T>::getShader() const
{
	return m_shader;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void CpuParticles<T>::updateVertexArray()
{
	// Get particle field properties
	std::vector<Vector2u> info = m_fieldsFunc();
	Uint32 currentSlot = 0;

	for (Uint32 i = 0; i < info.size(); ++i)
	{
		Uint32 offset = info[i].x;
		Uint32 size = info[i].y;

		// If the property is larger than 4 floats, then multiple slots should be used
		Uint32 numSlots = (size + 3) / 4;
		for (Uint32 slot = 0; slot < numSlots; ++slot, ++currentSlot)
		{
			m_vertexArray.addBuffer(m_vertexBuffer, currentSlot, size < 4 ? size : 4, sizeof(T), offset);
			if (size > 4)
				size -= 4;
		}
	}
}


///////////////////////////////////////////////////////////
template <typename T>
inline GpuParticles<T>::GpuParticles() :
	m_texture		(0),
	m_updateShader	(0),
	m_renderShader	(0),
	m_tfQuery		(0),
	m_numParticles	(0),
	m_bufferSize	(256),
	m_currentBuffer	(0),
	m_queryFinished	(true)
{
	m_fieldsFunc = PARTICLE_FIELDS(T, m_position, m_rotation, m_size, m_color, m_textureRect, m_velocity, m_age, m_type);
}


///////////////////////////////////////////////////////////
template <typename T>
inline GpuParticles<T>::~GpuParticles()
{
	if (m_tfQuery)
		glCheck(glDeleteQueries(1, &m_tfQuery));
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::init(Scene* scene)
{
	// Create vertex buffer with default size
	m_vertexBuffers[0].create((T*)NULL, m_bufferSize, BufferUsage::Stream);
	m_vertexBuffers[1].create((T*)NULL, m_bufferSize, BufferUsage::Stream);

	// Render as points
	m_vertexArray.setDrawMode(DrawMode::Points);

	// Start with zero particles
	m_vertexArray.setNumVertices(0);

	// Use default shader if one isn't provided
	if (!m_renderShader)
		m_renderShader = &getDefaultParticleShader();

	// Generate a query object to keep track of how many particles are output
	glCheck(glGenQueries(1, &m_tfQuery));
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::render(Camera& camera, RenderPass pass)
{
	// Only render for default pass
	if (pass != RenderPass::Default) return;

	// Check if the query is ready
	Uint32 available = GL_FALSE;
	glGetQueryObjectuiv(m_tfQuery, GL_QUERY_RESULT_AVAILABLE, &available);

	if (available)
	{
		// Check number of particles before rendering to give the update function
		// as much time as possible to finish the query
		glCheck(glGetQueryObjectuiv(m_tfQuery, GL_QUERY_RESULT, &m_numParticles));
		m_vertexArray.setNumVertices(m_numParticles);

		// Update current buffer
		m_currentBuffer = (m_currentBuffer + 1) % 2;

		// Indicate query finished
		m_queryFinished = true;
	}

	if (!m_numParticles) return;

	// Bind current buffer
	bindInputBuffer(m_vertexBuffers[m_currentBuffer]);

	// Bind shader
	m_renderShader->bind();

	// Camera
	camera.apply(m_renderShader);

	// Bind texture
	if (m_texture)
	{
		m_renderShader->setUniform("u_texture", *m_texture);
		m_renderShader->setUniform("u_hasTexture", true);
	}
	else
		m_renderShader->setUniform("u_hasTexture", false);

	// Enable depth testing
	glCheck(glEnable(GL_DEPTH_TEST));

	// Draw particles
	m_vertexArray.draw();
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::addParticle(const T& particle)
{
	// Quit if can't add any more particles
	if (m_numParticles >= m_bufferSize) return;

	// Append to end of the current buffer
	VertexBuffer& buffer = m_vertexBuffers[m_currentBuffer];
	buffer.update(&particle, 1, m_numParticles);

	// Update number of particles
	m_vertexArray.setNumVertices(++m_numParticles);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::update(const std::function<void(Shader*)>& func)
{
	// Can't update without an update shader
	if (!m_updateShader || !m_numParticles || !m_queryFinished) return;

	// Get elapsed time
	float dt = m_clock.restart().toSeconds();

	// Disable fragment shader
	glCheck(glEnable(GL_RASTERIZER_DISCARD));

	// Bind shader and set uniforms
	m_updateShader->bind();
	m_updateShader->setUniform("u_elapsed", dt);

	// Call update function
	if (func)
		func(m_updateShader);

	// Bind vertex buffers
	Uint32 nextBuffer = (m_currentBuffer + 1) % 2;
	VertexBuffer& inputBuffer = m_vertexBuffers[m_currentBuffer];
	VertexBuffer& outputBuffer = m_vertexBuffers[nextBuffer];
	bindInputBuffer(inputBuffer);

	// Output buffer
	outputBuffer.bind(BufferTarget::TransformFeedback, 0);

	// Begin transform feedback
	glCheck(glBeginTransformFeedback(GL_POINTS));

	// Update particles
	glCheck(glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_tfQuery));
	m_vertexArray.draw();
	glCheck(glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN));

	// End transform feedback
	glCheck(glEndTransformFeedback());

	// Reenable fragment shader
	glCheck(glDisable(GL_RASTERIZER_DISCARD));

	// Indicate that particles are still updating
	m_queryFinished = false;

	// Don't update the current buffer immediately after calling
	// the update because it will take time to finish. Keep rendering
	// the old buffer until the next one finishes
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::setMaxParticles(Uint32 max)
{
	m_bufferSize = max;

	// Recreate buffers
	m_vertexBuffers[0].create((T*)NULL, m_bufferSize, BufferUsage::Stream);
	m_vertexBuffers[1].create((T*)NULL, m_bufferSize, BufferUsage::Stream);
}

///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::setTexture(Texture* texture)
{
	m_texture = texture;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::setUpdateShader(Shader* shader)
{
	m_updateShader = shader;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::setRenderShader(Shader* shader)
{
	m_renderShader = shader;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::setFields(const std::function<std::vector<Vector2u>()>& func)
{
	m_fieldsFunc = func;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 GpuParticles<T>::getNumParticles()
{
	// Update the number of particles
	glCheck(glGetQueryObjectuiv(m_tfQuery, GL_QUERY_RESULT, &m_numParticles));
	m_vertexArray.setNumVertices(m_numParticles);

	return m_numParticles;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 GpuParticles<T>::getMaxParticles() const
{
	return m_bufferSize;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Texture* GpuParticles<T>::getTexture() const
{
	return m_texture;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Shader* GpuParticles<T>::getUpdateShader() const
{
	return m_updateShader;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Shader* GpuParticles<T>::getRenderShader() const
{
	return m_renderShader;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void GpuParticles<T>::bindInputBuffer(VertexBuffer& buffer)
{
	// Get particle field properties
	std::vector<Vector2u> info = m_fieldsFunc();
	Uint32 currentSlot = 0;

	// Bind buffer to standard array target
	buffer.bind(BufferTarget::Array);

	// Bind buffer to array
	for (Uint32 i = 0; i < info.size(); ++i)
	{
		Uint32 offset = info[i].x;
		Uint32 size = info[i].y;

		// If the property is larger than 4 floats, then multiple slots should be used
		Uint32 numSlots = (size + 3) / 4;
		for (Uint32 slot = 0; slot < numSlots; ++slot, ++currentSlot)
		{
			m_vertexArray.addBuffer(buffer, currentSlot, size < 4 ? size : 4, sizeof(T), offset);
			if (size > 4)
				size -= 4;
		}
	}
}


}