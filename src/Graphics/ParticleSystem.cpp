#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/ParticleSystem.h>

namespace poly
{


///////////////////////////////////////////////////////////
Shader ParticleSystem::s_shader;


///////////////////////////////////////////////////////////
ParticleType::ParticleType() :
	m_texture		(0),
	m_particleSize	(0),
	m_numParticles	(0),
	m_currentBuffer	(0)
{

}


///////////////////////////////////////////////////////////
void ParticleType::setUpdateShader(const std::string& fname)
{
	// Can only set update shader if it hasn't been loaded yet
	if (m_shader.getId()) return;

	// Specify transform feedback variables
	std::vector<const char*> varyings;
	for (Uint32 i = 0; i < m_fields.size(); ++i)
		varyings.push_back(m_fields[i].m_uniform.c_str());

	// Load update shader
	m_shader.load("shaders/particles/update.vert", Shader::Vertex);
	m_shader.load(fname, Shader::Geometry);
	m_shader.compile(varyings);
}


///////////////////////////////////////////////////////////
void ParticleType::setUpdateFunc(const std::function<void(Shader&)>& func)
{
	m_updateFunc = func;
}


///////////////////////////////////////////////////////////
void ParticleType::setTexture(Texture* texture)
{
	m_texture = texture;
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
		std::vector<ParticleType::ParticleField>& fields = type->m_fields;
		for (Uint32 i = 0; i < fields.size(); ++i)
		{
			type->m_vertexArray.addBuffer(
				inputBuffer,
				i,
				fields[i].m_size / 4,
				type->m_particleSize,
				fields[i].m_offset,
				1,
				fields[i].m_isFloatType ? GLType::Float : GLType::Int32
			);
		}

		// Output buffer
		outputBuffer.bind(BufferTarget::TransformFeedback, 0);

		// Begin transform feedback
		glCheck(glBeginTransformFeedback(GL_POINTS));

		// Need to keep track of how many particles are output
		Uint32 query;
		glCheck(glGenQueries(1, &query));

		// Update particles
		glCheck(glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query));
		type->m_vertexArray.draw(type->m_numParticles);
		glCheck(glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN));

		// Update the number of particles
		glCheck(glGetQueryObjectuiv(query, GL_QUERY_RESULT, &type->m_numParticles));

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
		if (!type->m_numParticles) continue;

		// Bind the particle texture
		shader.setUniform("u_hasTexture", (int)type->m_texture);
		if (type->m_texture)
			shader.setUniform("u_texture", *type->m_texture);

		// Bind particle buffer
		VertexBuffer& inputBuffer = type->m_vertexBuffers[type->m_currentBuffer];
		std::vector<ParticleType::ParticleField>& fields = type->m_fields;
		for (Uint32 i = 0; i < fields.size(); ++i)
		{
			type->m_vertexArray.addBuffer(
				inputBuffer,
				i,
				fields[i].m_size / 4,
				type->m_particleSize,
				fields[i].m_offset,
				1,
				fields[i].m_isFloatType ? GLType::Float : GLType::Int32
			);
		}

		// Draw the particles
		type->m_vertexArray.draw(type->m_numParticles);
	}
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