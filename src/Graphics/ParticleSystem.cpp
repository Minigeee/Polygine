#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/ParticleSystem.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
void bindParticleBuffer(VertexArray& vao, VertexBuffer& vbo)
{
	vbo.bind(BufferTarget::Array);
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
Shader ParticleSystem::s_shader;


///////////////////////////////////////////////////////////
Particle::Particle() :
	m_type				(0),
	m_position			(0.0f),
	m_size				(1.0f),
	m_rotation			(0.0f),
	m_texCoord			(0.0f),
	m_texSize			(1.0f),
	m_color				(1.0f),
	m_velocity			(0.0f),
	m_angularVelocity	(0.0f),
	m_elapsed			(0.0f)
{

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

		// Begin transform feedback
		glCheck(glBeginTransformFeedback(GL_POINTS));

		// Need to keep track of how many particles are output
		Uint32 query;
		glCheck(glGenQueries(1, &query));

		// Update particles
		glCheck(glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query));
		type->m_vertexArray.draw(type->m_numInstances);
		glCheck(glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN));

		// Update the number of particles
		glCheck(glGetQueryObjectuiv(query, GL_QUERY_RESULT, &type->m_numInstances));

		// End transform feedback
		glCheck(glEndTransformFeedback());

		// Update the type's current buffer
		type->m_currentBuffer = nextBuffer;
	}

	// Enable fragment shader
	glCheck(glDisable(GL_RASTERIZER_DISCARD));
}


///////////////////////////////////////////////////////////
void ParticleSystem::render(Camera& camera)
{
	// Bind shader and set uniforms
	Shader& shader = getShader();
	shader.bind();
	shader.setUniform("u_projView", camera.getProjMatrix() * camera.getViewMatrix());
	shader.setUniform("u_cameraPos", camera.getPosition());

	// Iterate through particle types
	for (auto it = m_particleTypes.begin(); it != m_particleTypes.end(); ++it)
	{
		// Get particle type struct
		ParticleType* type = it.value();

		// Skip if no particles exist
		if (!type->m_numInstances) continue;

		// Bind the particle texture
		shader.setUniform("u_hasTexture", (int)type->m_texture);
		if (type->m_texture)
			shader.setUniform("u_texture", *type->m_texture);

		// Bind particle buffer
		VertexBuffer& inputBuffer = type->m_vertexBuffers[type->m_currentBuffer];
		priv::bindParticleBuffer(type->m_vertexArray, inputBuffer);

		// Draw the particles
		type->m_vertexArray.draw(type->m_numInstances);
	}
}


///////////////////////////////////////////////////////////
void ParticleSystem::addParticleType(const std::string& shader, Uint32 maxParticles, Texture* texture, std::function<void(Shader&)>&& update)
{
	// Check if the shader has already been added
	if (m_particleTypes.find(shader) != m_particleTypes.end()) return;

	// Allocate particle type
	ParticleType* type = (ParticleType*)m_typePool.alloc();
	new(type)ParticleType();

	// Specify transform feedback variables
	std::vector<const char*> varyings;
	varyings.push_back("g_type");
	varyings.push_back("g_position");
	varyings.push_back("g_size");
	varyings.push_back("g_rotation");
	varyings.push_back("g_texCoord");
	varyings.push_back("g_texSize");
	varyings.push_back("g_color");
	varyings.push_back("g_velocity");
	varyings.push_back("g_angularVelocity");
	varyings.push_back("g_elapsed");

	// Load update shader
	type->m_shader.load("shaders/particles/update.vert", Shader::Vertex);
	type->m_shader.load(shader, Shader::Geometry);
	type->m_shader.compile(varyings);

	// Create vertex buffer
	type->m_vertexBuffers[0].create((Particle*)0, maxParticles, BufferUsage::Dynamic);
	type->m_vertexBuffers[1].create((Particle*)0, maxParticles, BufferUsage::Dynamic);

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
Shader& ParticleSystem::getShader()
{
	if (!s_shader.getId())
	{
		s_shader.load("shaders/particles/particle.vert", Shader::Vertex);
		s_shader.load("shaders/particles/particle.geom", Shader::Geometry);
		s_shader.load("shaders/particles/particle.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


}