#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/ParticleSystem.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
void bindParticleBuffer(VertexArray& vao, VertexBuffer& vbo)
{
	vao.addBuffer(vbo, 0, 1, sizeof(Particle), 0 * sizeof(float), 1, GLType::Int32);	// Type
	vao.addBuffer(vbo, 1, 3, sizeof(Particle), 1 * sizeof(float), 1);					// Position
	vao.addBuffer(vbo, 2, 2, sizeof(Particle), 4 * sizeof(float), 1);					// Size
	vao.addBuffer(vbo, 3, 1, sizeof(Particle), 6 * sizeof(float), 1);					// Rotation
	vao.addBuffer(vbo, 4, 2, sizeof(Particle), 7 * sizeof(float), 1);					// Tex coords
	vao.addBuffer(vbo, 5, 2, sizeof(Particle), 8 * sizeof(float), 1);					// Tex size
	vao.addBuffer(vbo, 6, 4, sizeof(Particle), 11 * sizeof(float), 1);					// Color
	vao.addBuffer(vbo, 7, 3, sizeof(Particle), 15 * sizeof(float), 1);					// Velocity
	vao.addBuffer(vbo, 8, 1, sizeof(Particle), 18 * sizeof(float), 1);					// Angular velocity
	vao.addBuffer(vbo, 9, 1, sizeof(Particle), 19 * sizeof(float), 1);					// Time elapsed
}


}


///////////////////////////////////////////////////////////
ParticleSystem::ParticleSystem() :
	m_typePool		(sizeof(ParticleType), 32)
{

}


///////////////////////////////////////////////////////////
ParticleSystem::~ParticleSystem()
{
	for (auto it = m_particleTypes.begin(); it != m_particleTypes.end(); ++it)
	{
		// Call emitter destructor
		it.value()->~ParticleType();

		// Free memory
		m_typePool.free(it.value());
	}
}


///////////////////////////////////////////////////////////
void ParticleSystem::init(Scene* scene)
{
	// Nothing to do here
}


///////////////////////////////////////////////////////////
void ParticleSystem::update(float dt)
{
	// Disable fragment shader
	glCheck(glEnable(GL_RASTERIZER_DISCARD));

	// Begin transform feedback
	glCheck(glBeginTransformFeedback(GL_POINTS));

	// Iterate through particle types
	for (auto it = m_particleTypes.begin(); it != m_particleTypes.end(); ++it)
	{
		// Get particle type struct
		ParticleType* type = it.value();

		// Bind shader and set uniforms
		type->m_shader.bind();
		type->m_shader.setUniform("u_elapsed", dt);

		// Call update function
		if (type->m_updateFunc)
			type->m_updateFunc(type->m_shader);

		// Bind vertex buffers
		Uint32 nextBuffer = (type->m_currentBuffer + 1) % 2;
		VertexBuffer& inputBuffer = type->m_vertexBuffers[type->m_currentBuffer];
		VertexBuffer& outputBuffer = type->m_vertexBuffers[nextBuffer];

		// Input buffer
		priv::bindParticleBuffer(type->m_vertexArray, inputBuffer);

		// Output buffer
		outputBuffer.bind(BufferTarget::TransformFeedback, 0);

		// Need to keep track of how many particles are output
		Uint32 query;
		glCheck(glGenQueries(1, &query));

		// Update particles
		glCheck(glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query));
		type->m_vertexArray.draw(type->m_numInstances);
		glCheck(glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN));

		// Update the number of particles
		glCheck(glGetQueryObjectuiv(query, GL_QUERY_RESULT, &type->m_numInstances));

		// Update the type's current buffer
		type->m_currentBuffer = nextBuffer;
	}

	// End transform feedback
	glCheck(glEndTransformFeedback());

	// Enable fragment shader
	glCheck(glDisable(GL_RASTERIZER_DISCARD));
}


///////////////////////////////////////////////////////////
void ParticleSystem::render(Camera& camera)
{
	// Iterate through particle types
	for (auto it = m_particleTypes.begin(); it != m_particleTypes.end(); ++it)
	{
		// Get particle type struct
		ParticleType* type = it.value();

		// Bind shader and set uniforms
		Shader& shader = getShader();
		type->m_shader.bind();
		type->m_shader.setUniform("u_projView", camera.getProjMatrix() * camera.getViewMatrix());
		// Bind the particle texture
		if (type->m_texture)
			type->m_shader.setUniform("u_texture", *type->m_texture);

		// Bind particle buffer
		VertexArray& vertexArray = getVertexArray();
		VertexBuffer& inputBuffer = type->m_vertexBuffers[type->m_currentBuffer];
		priv::bindParticleBuffer(vertexArray, inputBuffer);

		// Draw the particles
		vertexArray.draw(type->m_numInstances);
	}
}


///////////////////////////////////////////////////////////
void ParticleSystem::addParticleType(const std::string& shader, Uint32 maxParticles, Texture* texture, std::function<void(Shader&)>&& update)
{
	// Check if the shader has already been added
	if (m_particleTypes.find(shader) != m_particleTypes.end()) return;

	// Allocate particle type
	ParticleType* type = (ParticleType*)m_typePool.alloc();

	// Load update shader
	type->m_shader.load("particles/update.vert", Shader::Vertex);
	type->m_shader.load(shader, Shader::Geometry);
	type->m_shader.compile();

	// Create vertex buffer
	type->m_vertexBuffers[0].create((ParticleType*)0, maxParticles, BufferUsage::Dynamic);
	type->m_vertexBuffers[1].create((ParticleType*)0, maxParticles, BufferUsage::Dynamic);

	// Create vertex array
	priv::bindParticleBuffer(type->m_vertexArray, type->m_vertexBuffers[0]);
	type->m_vertexArray.setDrawMode(DrawMode::Points);
	type->m_vertexArray.setNumVertices(1);

	// Set the number of instances and texture
	type->m_texture = texture;
	type->m_numInstances = 0;

	// Update function
	type->m_updateFunc = update;

	// Add to map
	m_particleTypes[shader] = type;
}


///////////////////////////////////////////////////////////
void ParticleSystem::createEmitter(const std::string& shader, const Particle& emitter)
{
	// Find the particle type
	auto it = m_particleTypes.find(shader);
	if (it == m_particleTypes.end()) return;
	ParticleType* type = it.value();

	// Update vertex buffer
	type->m_vertexBuffers[type->m_currentBuffer].update(&emitter, 1, type->m_numInstances++);
}


///////////////////////////////////////////////////////////
VertexArray& ParticleSystem::getVertexArray()
{
	if (!s_vertexArray.getId())
	{
		// Set vertex array parameters
		s_vertexArray.bind();
		s_vertexArray.setDrawMode(DrawMode::Points);
		s_vertexArray.setNumVertices(1);
	}

	return s_vertexArray;
}


///////////////////////////////////////////////////////////
Shader& ParticleSystem::getShader()
{
	if (!s_shader.getId())
	{
		s_shader.load("particles/particle.vert", Shader::Vertex);
		s_shader.load("particles/particle.geom", Shader::Geometry);
		s_shader.load("particles/particle.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


}