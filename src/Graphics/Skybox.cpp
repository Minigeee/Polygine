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


/*

///////////////////////////////////////////////////////////
ProceduralSkybox::ProceduralSkybox() :
	m_zenithColor		(0.172f, 0.448f, 0.775f),
	m_horizonColor		(0.9f, 1.0f, 0.75f),
    m_groundColor       (0.12f, 0.22f, 0.25f),
	m_bloomColor		(0.8f, 0.75f, 0.5f),
	m_bloomSize 		(0.8f),
	m_lightStrength		(12.0f),
    m_topRadius         (6420.0f),
    m_botRadius         (6360.0f),
    m_altitude          (0.2f)
{

}

*/


///////////////////////////////////////////////////////////
ProceduralSkybox::ProceduralSkybox() :
	m_zenithColor		(0.12f, 0.2f, 0.5f),
	m_horizonColor		(0.05f, 0.15f, 0.25f),
    m_groundColor       (0.12f, 0.22f, 0.25f),
	m_bloomColor		(0.05f, 0.05f, 0.06f),
	m_bloomSize 		(0.0f),
	m_lightStrength		(0.5f),
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
    shader.setUniform("u_bloomSize", m_bloomSize);
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


///////////////////////////////////////////////////////////
void ProceduralSkybox::setZenithColor(const Vector3f& color)
{
    m_zenithColor = color;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setHorizonColor(const Vector3f& color)
{
    m_horizonColor = color;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setGroundColor(const Vector3f& color)
{
    m_groundColor = color;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setBloomColor(const Vector3f& color)
{
    m_bloomColor = color;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setBloomSize(float size)
{
    m_bloomSize = size;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setLightStrength(float strength)
{
    m_lightStrength = strength;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setTopRadius(float radius)
{
    m_topRadius = radius;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setBotRadius(float radius)
{
    m_botRadius = radius;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setAltitude(float alt)
{
    m_altitude = alt;
}


///////////////////////////////////////////////////////////
const Vector3f& ProceduralSkybox::getZenithColor() const
{
    return m_zenithColor;
}


///////////////////////////////////////////////////////////
const Vector3f& ProceduralSkybox::getHorizonColor() const
{
    return m_horizonColor;
}


///////////////////////////////////////////////////////////
const Vector3f& ProceduralSkybox::getGroundColor() const
{
    return m_groundColor;
}


///////////////////////////////////////////////////////////
const Vector3f& ProceduralSkybox::getBloomColor() const
{
    return m_bloomColor;
}


///////////////////////////////////////////////////////////
float ProceduralSkybox::getBloomSize() const
{
    return m_bloomSize;
}


///////////////////////////////////////////////////////////
float ProceduralSkybox::getLightStrength() const
{
    return m_lightStrength;
}


///////////////////////////////////////////////////////////
float ProceduralSkybox::getTopRadius() const
{
    return m_topRadius;
}


///////////////////////////////////////////////////////////
float ProceduralSkybox::getBotRadius() const
{
    return m_botRadius;
}


///////////////////////////////////////////////////////////
float ProceduralSkybox::getAltitude() const
{
    return m_altitude;
}


}