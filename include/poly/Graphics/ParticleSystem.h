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
/// \return A reference to the default particle render shader
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
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	CpuParticles();

	///////////////////////////////////////////////////////////
	/// \brief Initialize the particle system
	///
	/// This function is automatically called when it is added
	/// to a scene as a render system.
	///
	/// \param scene A pointer to a scene
	///
	///////////////////////////////////////////////////////////
	void init(Scene* scene) override;

	///////////////////////////////////////////////////////////
	/// \brief Render all particles in the particle system
	///
	/// If the particle system has been added to a scene, this
	/// will automatically be called when Scene::render() is called.
	///
	/// \param camera The camera to render the particles from the perspective of
	/// \param pass The render pass type
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera, RenderPass pass) override;

	///////////////////////////////////////////////////////////
	/// \brief Add a particle object to the system to be rendered
	///
	/// \param particle The particle object to add
	///
	///////////////////////////////////////////////////////////
	void addParticle(const T& particle);

	///////////////////////////////////////////////////////////
	/// \brief Execute an update for all particles in the system
	///
	/// This function executes the given update function for every
	/// particle in the particle system. The \a func paramteter can
	/// be any callable object, such as a function pointer, lambda,
	/// a functor, or std::function, and it has to have the following
	/// signature:
	///
	/// \code
	/// bool update(float dt, T& particle);
	/// \endcode
	///
	/// This update function is executed for every particle individually,
	/// where \a dt will contain the time elapsed since the last update
	/// in seconds, \a particle will contain the particle object that is
	/// being updated, and the function must return \a true if the particle
	/// should be kept, and \a false if the particle should be removed.
	///
	/// Please see the class description for an example of how to update
	/// particles.
	///
	/// \param func The update function
	///
	///////////////////////////////////////////////////////////
	template <typename Func>
	void update(Func&& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the texture that should be used to render particles
	///
	/// \param texture A pointer to the particle texture
	///
	///////////////////////////////////////////////////////////
	void setTexture(Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Set the particle render shader
	///
	/// If no shader is set, then the default shader is used.
	///
	/// \param shader A pointer to the shader
	///
	///////////////////////////////////////////////////////////
	void setShader(Shader* shader);

	///////////////////////////////////////////////////////////
	/// \brief Set which particle field properties will be usedin the shader
	///
	/// This function should be used to define which particle fields
	/// will be used in the render shader, where the order the
	/// fields are defined will be the order the properties are
	/// defined in the vertex shader. The input parameter is a
	/// callable object, but using the PARTICLE_FIELDS() macro
	/// is most convenient. This function should only be used
	/// if custom particle properties are needed for a custom
	/// particle shader.
	///
	/// The default properties that are added are: m_position,
	/// m_rotation, m_size, m_color, and m_textureRect.
	///
	/// Usage example:
	/// \code
	///
	/// CpuParticles<Particle> particles;
	/// 
	/// // Custom shader
	/// particles.setShader(&customShader);
	///
	/// // That uses custom properties that only uses position, rotation, size, and color
	/// particles.setFields(PARTICLE_FIELDS(Particle, m_position, m_rotation, m_size, m_color));
	///
	/// \endcode
	///
	///////////////////////////////////////////////////////////
	void setFields(const std::function<std::vector<Vector2u>()>& func);

	///////////////////////////////////////////////////////////
	/// \brief Get the number of particles currently existing in the system
	///
	/// \return The number of particles in the system
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumParticles() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the particle texture
	///
	/// \return A pointer to the particle texture
	///
	///////////////////////////////////////////////////////////
	Texture* getTexture() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the particle render shader
	///
	/// \return A pointer to the render shader
	///
	///////////////////////////////////////////////////////////
	Shader* getShader() const;

private:
	void updateVertexArray();

private:
	Clock m_clock;					//!< Each particle system has its own internal clock
	Texture* m_texture;				//!< The particle texture
	Shader* m_shader;				//!< The particle render shader

	Uint32 m_bufferSize;			//!< The size of the vertex buffer in number of particles
	VertexArray m_vertexArray;		//!< The vertex array to render particles
	VertexBuffer m_vertexBuffer;	//!< The vertex buffer containing the render data for particles
	std::vector<T> m_particles;		//!< The list of particles

	std::function<std::vector<Vector2u>()> m_fieldsFunc;	//!< The fields info function
};


///////////////////////////////////////////////////////////
/// \brief A system of particle effects where processing occurs on GPU
///
///////////////////////////////////////////////////////////
template <typename T>
class GpuParticles : public RenderSystem
{
public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	GpuParticles();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~GpuParticles();

	///////////////////////////////////////////////////////////
	/// \brief Initialize the particle system
	///
	/// This function is automatically called when it is added
	/// to a scene as a render system.
	///
	/// \param scene A pointer to a scene
	///
	///////////////////////////////////////////////////////////
	void init(Scene* scene) override;

	///////////////////////////////////////////////////////////
	/// \brief Render all particles in the particle system
	///
	/// If the particle system has been added to a scene, this
	/// will automatically be called when Scene::render() is called.
	///
	/// \param camera The camera to render the particles from the perspective of
	/// \param pass The render pass type
	///
	///////////////////////////////////////////////////////////
	void render(Camera& camera, RenderPass pass) override;

	///////////////////////////////////////////////////////////
	/// \brief Add a particle object to the system to be rendered
	///
	/// \param particle The particle object to add
	///
	///////////////////////////////////////////////////////////
	void addParticle(const T& particle);

	///////////////////////////////////////////////////////////
	/// \brief Run the GPU particle update pass
	///
	/// The GPU particle update disables fragment shader to run
	/// the update logic in a geometry shader using transform feedback.
	/// The optional \a func parameter should be used to set shader
	/// uniforms that are necessary for the particle update.
	///
	/// Please see the class description for an example of how to setup
	/// an update shader.
	///
	/// \param func The shader uniform update function.
	///
	///////////////////////////////////////////////////////////
	void update(const std::function<void(Shader*)>& func = std::function<void(Shader*)>());

	///////////////////////////////////////////////////////////
	/// \brief Set the maximum number of particles allowed in the system
	///
	/// This function updates the max number and resizes the vertex
	/// buffers.
	///
	/// \param max The max number of particles
	///
	///////////////////////////////////////////////////////////
	void setMaxParticles(Uint32 max);

	///////////////////////////////////////////////////////////
	/// \brief Set the texture that should be used to render particles
	///
	/// \param texture A pointer to the particle texture
	///
	///////////////////////////////////////////////////////////
	void setTexture(Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Set the particle update shader
	///
	/// If no particle update shader is set, then update() will
	/// not work correctly.
	///
	/// \param shader A pointer to the shader
	///
	///////////////////////////////////////////////////////////
	void setUpdateShader(Shader* shader);

	///////////////////////////////////////////////////////////
	/// \brief Set the particle render shader
	///
	/// If no particle update shader is set, then the default
	/// particle shader will be used.
	///
	/// \param shader A pointer to the shader
	///
	///////////////////////////////////////////////////////////
	void setRenderShader(Shader* shader);

	///////////////////////////////////////////////////////////
	/// \brief Set which particle field properties will be usedin the shader
	///
	/// This function should be used to define which particle fields
	/// will be used in the render shader, where the order the
	/// fields are defined will be the order the properties are
	/// defined in the vertex shader. The input parameter is a
	/// callable object, but using the PARTICLE_FIELDS() macro
	/// is most convenient. This function should only be used
	/// if custom particle properties are needed for a custom
	/// particle shader.
	///
	/// The default properties that are added are: m_position,
	/// m_rotation, m_size, m_color, and m_textureRect.
	///
	/// Usage example:
	/// \code
	///
	/// GpuParticles<Particle> particles;
	/// 
	/// // Custom shader
	/// particles.setShader(&customShader);
	///
	/// // That uses custom properties that only uses position, rotation, size, and color
	/// particles.setFields(PARTICLE_FIELDS(Particle, m_position, m_rotation, m_size, m_color));
	///
	/// \endcode
	///
	///////////////////////////////////////////////////////////
	void setFields(const std::function<std::vector<Vector2u>()>& func);

	///////////////////////////////////////////////////////////
	/// \brief Get the number of particles currently existing in the system
	///
	/// \return The number of particles in the system
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumParticles();

	///////////////////////////////////////////////////////////
	/// \brief Get the maximum number of allowed particles
	///
	/// \return The maximum number of allowed particles
	///
	///////////////////////////////////////////////////////////
	Uint32 getMaxParticles() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the particle texture
	///
	/// \return A pointer to the particle texture
	///
	///////////////////////////////////////////////////////////
	Texture* getTexture() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the particle render shader
	///
	/// \return A pointer to the render shader
	///
	///////////////////////////////////////////////////////////
	Shader* getUpdateShader() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the particle render shader
	///
	/// \return A pointer to the render shader
	///
	///////////////////////////////////////////////////////////
	Shader* getRenderShader() const;

private:
	void bindInputBuffer(VertexBuffer& buffer);

private:
	Clock m_clock;						//!< Each particle system has its own internal clock
	Texture* m_texture;					//!< The particle texture
	Shader* m_updateShader;				//!< The particle update shader
	Shader* m_renderShader;				//!< The particle render shader

	Uint32 m_tfQuery;					//!< An OpenGL transform feedback query object (for counting particles)
	Uint32 m_numParticles;				//!< The number of particles
	Uint32 m_bufferSize;				//!< The vertex buffer size
	VertexArray m_vertexArray;			//!< The vertex array to render and update particles
	VertexBuffer m_vertexBuffers[2];	//!< Two vertex buffers for updating particles and rendering
	Uint32 m_currentBuffer;				//!< The current buffer index

	std::function<std::vector<Vector2u>()> m_fieldsFunc;	//!< The fields info function
};


#include <poly/Core/Macros.h>


#ifndef DOXYGEN_SKIP

#define _GET_FIELD_PROPERTIES_EXPR(x) info.push_back(Vector2u((Uint32)((Uint8*)&p.x - (Uint8*)&p), ((Uint32)sizeof(p.x) + 3) / 4));

#define _GET_FIELD_PROPERTIES(...) LOOP(_GET_FIELD_PROPERTIES_EXPR, __VA_ARGS__)

#endif


///////////////////////////////////////////////////////////
/// \brief A macro used to define which particle fields should be sent to the shaders
///
/// The first argument of the macro should be the particle type
/// of the properties that are being defined. The rest of the
/// arguments should be the particle property variable names.
///
/// Usage example:
/// \code
///
/// // Custom particle type
/// struct ParticleA
/// {
///		Vector3f m_position;
///		float m_rotation;
///		Vector2f m_size;
///		Vector4f m_color;
/// };
///
/// CpuParticles<ParticleA> particles;
///
/// // Define the custom partlce fields
/// particles.setFields(PARTICLE_FIELDS(ParticleA, m_position, m_rotation, m_size, m_color));
///
/// \endcode
///
///////////////////////////////////////////////////////////
#define PARTICLE_FIELDS(type, ...) \
	[]() -> std::vector<Vector2u> { std::vector<Vector2u> info; type p; _GET_FIELD_PROPERTIES(__VA_ARGS__) return info; }


}

#include <poly/Graphics/ParticleSystem.inl>

#endif


///////////////////////////////////////////////////////////
/// \class poly::CpuParticles
/// \ingroup Graphics
///
/// This the CPU particle system is responsible for rendering
/// particles and handling update of the particle properties
/// on the CPU side. The CPU particle system should be used
/// for particles with more complex behavior, but it can't
/// handle as many particles as the GPU particle system can.
///
/// To use texture particles, use the setTexture() function
/// to use a single texture or a texture atlas. If the texture
/// being used is a texture atlas, setting the texture rectangle
/// may be necessary using each particle's individual m_texRect
/// property.
///
/// To use a custom render shader, use setShader(). If using
/// a custom shader, then custom input vertex attributes may
/// be needed. To do this, use setFields() to define all the
/// necessary particle properties.
///
/// To update the particles each frame, call update() with
/// an update function that takes as its parameters a \a dt
/// for elapsed time, and a \a particle for the particle struct.
/// The given update function will be run once for each existing
/// particle in the system, and the return value of this function
/// should be a boolean, where \a true would indicate that the
/// particle should be kept, and \a false would indicate that
/// the particle should be removed from the system. An internal
/// clock is used to keep track of elapsed time between each
/// particle system update.
///
/// Since the particle system is a render system, it should
/// be added to a scene using Scene::addRenderSystem() so it
/// can be rendered correctly. Each particle system should
/// only be used to render one type of particles (i.e. a single
/// particle system should be used to render sparks, and another
/// could be used to render rain, etc).
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // A scene is needed to render
/// Scene scene;
///
/// // Custom particle struct
/// struct FireParticle
/// {
///		FireParticle() :
///			m_position		(0.0f),
///			m_rotation		(0.0f),
///			m_size			(0.1f),
///			m_color			(1.0f, 0.5f, 0.0f, 1.0f),
///			m_velocity		(0.0f),
///			m_age			(0.0f)
///		{ }
///
///		Vector3f m_position;
///		float m_rotation;
///		Vector2f m_size;
///		Vector4f m_color;
///
///		Vector3f m_velocity;
///		
///		// The age property is necessary for all CPU particles
///		float m_age;
/// };
///
/// // Create a particle system with the particle struct
/// CpuParticles<FireParticle> particles;
///
/// // Define particle properties (not including velocity or age because they are not needed for rendering)
/// particles.setFields(PARTICLE_FIELDS(FireParticle, m_position, m_rotation, m_size, m_color));
///
/// // Load a custom render shader
/// Shader fireShader;
/// fireShader.load("shaders/particles/fire.vert", Shader::Vertex);
/// fireShader.load("shaders/particles/fire.geom", Shader::Geometry);
/// fireShader.load("shaders/particles/fire.frag", Shader::Fragment);
/// fireShader.compile();
/// particles.setShader(&fireShader);
///
/// scene.addRenderSystem(&particles);
///
/// // Create a camera to render the particles
/// Camera camera;
///
///
/// // Game loop
/// while (true)
/// {
///		// Add a new particle every frame
///		particles.addParticle(FireParticle());
///
///		// Update particles
///		particles.update(
///			[&](float dt, FireParticle& particle) -> bool
///			{
///				// Particles should have a lifetime of 1 second
///				if (particle.m_age > 1.0f)
///					// Return false so that the particle gets removed
///					return false;
///
///				particle.m_velocity.y += dt * 5.0f;
///				particle.m_position += dt * particle.m_velocity;
///				partlce.m_color.a = 1.0f - particle.m_age;
///
///				// Return true to keep the particle
///				return true;
///			}
///		);
///
///		// Render particles
///		scene.render(camera);
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
/// \class poly::GpuParticles
/// \ingroup Graphics
///
/// This the GPU particle system is responsible for rendering
/// particles and handling update of the particle properties
/// on the GPU side. The GPU particle system should be used
/// for creating particle effects with relatively simple
/// behavior because updating on the GPU side is much more limited
/// in terms of the complexity that can be handled, but it
/// can handle the update and rendering of many more particles.
///
/// To use texture particles, use the setTexture() function
/// to use a single texture or a texture atlas. If the texture
/// being used is a texture atlas, setting the texture rectangle
/// may be necessary using each particle's individual m_texRect
/// property.
///
/// To use a custom render shader, use setRenderShader(). A user
/// defined update shader is required and can be specified using
/// setUpdateShader(). If using a custom particle struct or
/// a custom shader, then custom input vertex attributes may
/// be needed. To do this, use setFields() to define all the
/// necessary particle properties.
///
/// To update the particles each frame, call update() with an
/// optional update function to set any shader uniform values
/// that may be needed for the update. This function uses
/// transform feedback rendering to update particle property
/// values, and the update logic is often written in the geometry
/// shader, where a variable number of vertices can be outputted.
/// The output variables of the update shader must be the same
/// as the input vertex attribs, in the same order. And when
/// compiling the shader, all output variables must be specified
/// so that transform feedback can capture the values of these
/// variables.
///
/// Since the particle system is a render system, it should
/// be added to a scene using Scene::addRenderSystem() so it
/// can be rendered correctly. Each particle system should
/// only be used to render one type of particles (i.e. a single
/// particle system should be used to render sparks, and another
/// could be used to render rain, etc).
///
/// Usage example:
///
/// update.vert:
/// \code
///
/// // update.vert
///
/// #version 330 core
/// 
/// layout(location = 0) in vec3 a_position;
/// layout(location = 1) in float a_rotation;
/// layout(location = 2) in vec2 a_size;
/// layout(location = 3) in vec4 a_color;
/// layout(location = 4) in vec3 a_velocity;
/// layout(location = 5) in float a_age;
/// layout(location = 6) in int a_type;
/// 
/// out vec3 v_position;
/// out float v_rotation;
/// out vec2 v_size;
/// out vec4 v_color;
/// out vec3 v_velocity;
/// out float v_age;
/// flat out int v_type;
/// 
/// void main()
/// {
/// 	v_position = a_position;
/// 	v_rotation = a_rotation;
/// 	v_size = a_size;
/// 	v_color = a_color;
/// 	v_velocity = a_velocity;
/// 	v_age = a_age;
/// 	v_type = a_type;
/// }
///
/// \endcode
///
/// update.geom:
/// \code
///
/// // update.geom
///
/// #version 330 core
/// 
/// // Include for random and noise functions
/// #include "../noise.glsl"
/// 
/// layout(points) in;
/// layout(points, max_vertices = 10) out;
/// 
/// in vec3 v_position[];
/// in float v_rotation[];
/// in vec2 v_size[];
/// in vec4 v_color[];
/// in vec3 v_velocity[];
/// in float v_age[];
/// flat in int v_type[];
/// 
/// out vec3 g_position;
/// out float g_rotation;
/// out vec2 g_size;
/// out vec4 g_color;
/// out vec3 g_velocity;
/// out float g_age;
/// flat out int g_type;
/// 
/// uniform float u_elapsed;
/// 
/// void main()
/// {
/// 	g_position = v_position[0];
/// 	g_rotation = v_rotation[0];
/// 	g_size = v_size[0];
/// 	g_color = v_color[0];
/// 	g_velocity = v_velocity[0];
/// 	g_age = v_age[0] + u_elapsed;
/// 	g_type = v_type[0];
/// 
///		// Emitter type = 0
///		// Standard type = 1
///
/// 	if (g_type == 0)
/// 	{
/// 		// Create particle to maintain the emitter particle
/// 		EmitVertex();
/// 		EndPrimitive();
/// 
/// 		// Create another particle
/// 		g_velocity.xz = random2(vec2(g_age, 2.0f * g_age)) * 0.8f - 0.4f;
/// 		g_age = 0.0f;
/// 		g_type = 1;
/// 
/// 		EmitVertex();
/// 		EndPrimitive();
/// 	}
/// 	else
/// 	{
/// 		if (g_age > 3.0f)
///				// Return without creating a particle to remove the current particle
/// 			return;
/// 
/// 		// Update position
/// 		g_velocity.y += 9.8f * u_elapsed;
/// 		g_position += g_velocity * u_elapsed;
/// 
/// 		EmitVertex();
/// 		EndPrimitive();
/// 	}
/// }
///
/// \endcode
///
/// main.cpp:
/// \code
///
/// // main.cpp
///
/// using namespace poly;
///
/// // A scene is needed to render
/// Scene scene;
///
/// // Custom particle struct
/// struct FireParticle
/// {
///		FireParticle() :
///			m_position		(0.0f),
///			m_rotation		(0.0f),
///			m_size			(0.1f),
///			m_color			(1.0f, 0.5f, 0.0f, 1.0f),
///			m_velocity		(0.0f),
///			m_age			(0.0f),
///			m_type			(0)
///		{ }
///
///		Vector3f m_position;
///		float m_rotation;
///		Vector2f m_size;
///		Vector4f m_color;
///
///		Vector3f m_velocity;
///		float m_age;
///		int m_type;
/// };
///
/// // Create a particle system with the particle struct
/// GpuParticles<FireParticle> particles;
///
/// // Define particle properties
/// particles.setFields(PARTICLE_FIELDS(FireParticle, m_position, m_rotation, m_size, m_color, m_velocity, m_age, m_type));
///
/// // Load the update shader
/// Shader updateShader;
/// updateShader.load("shaders/particles/update.vert", Shader::Vertex);
/// updateShader.load("shaders/particles/update.geom", Shader::Geometry);
/// // Compile and specify the output variables
/// updateShader.compile({ "g_position", "g_rotation", "g_size", "g_color", "g_velocity", "g_age", "g_type" });
/// particles.setUpdateShader(&updateShader);
///
/// // Load a custom render shader
/// Shader renderShader;
/// renderShader.load("shaders/particles/fire.vert", Shader::Vertex);
/// renderShader.load("shaders/particles/fire.geom", Shader::Geometry);
/// renderShader.load("shaders/particles/fire.frag", Shader::Fragment);
/// renderShader.compile();
/// particles.setRenderShader(&renderShader);
///
/// scene.addRenderSystem(&particles);
///
/// // Create a camera to render the particles
/// Camera camera;
///
///
/// // Game loop
/// while (true)
/// {
///		// Update particles
///		particles.update();
///
///		// Render particles
///		scene.render(camera);
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////