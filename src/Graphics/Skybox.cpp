#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Skybox.h>

namespace poly
{


///////////////////////////////////////////////////////////
VertexArray Skybox::s_vertexArray;

///////////////////////////////////////////////////////////
VertexBuffer Skybox::s_vertexBuffer;

///////////////////////////////////////////////////////////
Shader ProceduralSkybox::s_shader;


///////////////////////////////////////////////////////////
VertexArray& Skybox::getVertexArray()
{
	if (!s_vertexArray.getId())
	{
        // Create skybox
        float vertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        // Add to vertex buffer
        s_vertexBuffer.create(vertices, 108);

        // Add to vertex array
        s_vertexArray.bind();
        s_vertexArray.addBuffer(s_vertexBuffer, 0, 3);
	}

	return s_vertexArray;
}


///////////////////////////////////////////////////////////
ProceduralSkybox::ProceduralSkybox() :
	m_zenithColor		(0.172f, 0.448f, 0.675f),
	m_horizonColor		(0.9f, 1.0f, 0.75f),
    m_groundColor       (0.12f, 0.22f, 0.25f),
	m_bloomColor		(0.8f, 0.75f, 0.5f),
	m_bloomStrength		(0.8f),
	m_lightStrength		(12.0f),
    m_topRadius         (6420.0f),
    m_botRadius         (6360.0f),
    m_altitude          (0.2f)
{

}


///////////////////////////////////////////////////////////
void ProceduralSkybox::init(Scene* scene)
{
	m_scene = scene;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::render(Camera& camera)
{
    // glCullFace(GL_FRONT);

    Shader& shader = getShader();

    shader.bind();

    // Projection view
    Matrix4f view = Matrix4f(Matrix3f(camera.getViewMatrix()), 1.0f);
    shader.setUniform("u_projView", camera.getProjMatrix() * view);

    // Set light directions
    int i = 0;
    m_scene->system<DirLightComponent>(
        [&](const Entity::Id id, DirLightComponent& light)
        {
            if (i++ == 0)
            {
                shader.setUniform("u_lightDir", normalize(light.m_direction));
            }
        }
    );

    // Skybox parameters
    shader.setUniform("u_zenithColor", m_zenithColor);
    shader.setUniform("u_horizonColor", m_horizonColor);
    shader.setUniform("u_groundColor", m_groundColor);
    shader.setUniform("u_bloomColor", m_bloomColor);
    shader.setUniform("u_bloomStrength", m_bloomStrength);
    shader.setUniform("u_lightStrength", m_lightStrength);
    shader.setUniform("u_topRadius", m_topRadius);
    shader.setUniform("u_botRadius", m_botRadius);
    shader.setUniform("u_radius", m_altitude + m_botRadius);

    // Draw cubemap
    Skybox::getVertexArray().draw();
}


///////////////////////////////////////////////////////////
Shader& ProceduralSkybox::getShader()
{
    if (!s_shader.getId())
    {
        s_shader.load("shaders/cubemap.vert", Shader::Vertex);
        s_shader.load("shaders/procedural_skybox.frag", Shader::Fragment);
        s_shader.compile();
    }

	return s_shader;
}


}