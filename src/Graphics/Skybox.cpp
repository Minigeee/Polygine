#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Image.h>
#include <poly/Graphics/Skybox.h>

#include <poly/Math/Functions.h>

namespace poly
{


///////////////////////////////////////////////////////////
VertexArray Skybox::s_vertexArray;

///////////////////////////////////////////////////////////
VertexBuffer Skybox::s_vertexBuffer;

///////////////////////////////////////////////////////////
Shader Skybox::s_shader;

///////////////////////////////////////////////////////////
Shader ProceduralSkybox::s_shader;


///////////////////////////////////////////////////////////
Skybox::Skybox() :
    m_id    (0)
{

}

///////////////////////////////////////////////////////////
Skybox::~Skybox()
{
    if (m_id)
        glCheck(glDeleteTextures(1, &m_id));
}


///////////////////////////////////////////////////////////
void Skybox::init(Scene* scene)
{
    // Nothing to do here
}


///////////////////////////////////////////////////////////
void Skybox::render(Camera& camera)
{
    Shader& shader = getShader();

    shader.bind();

    // Projection view
    Matrix4f view = Matrix4f(Matrix3f(camera.getViewMatrix()), 1.0f);
    shader.setUniform("u_projView", camera.getProjMatrix() * view);

    // Cube map
    shader.setUniform("u_cubemap", 0);
    glCheck(glActiveTexture(GL_TEXTURE0));
    glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));

    // Disable writing to depth buffer
    glCheck(glDepthMask(GL_FALSE));

    // Draw cubemap
    Skybox::getVertexArray().draw();

    glCheck(glDepthMask(GL_TRUE));
}


///////////////////////////////////////////////////////////
bool Skybox::load(const std::string& fname, Side side)
{
    // Load the image
    Image image;
    bool success = image.load(fname);
    if (!success) return false;

    // Create cube map if not create yet
    if (!m_id)
    {
        // Create texture
        glCheck(glGenTextures(1, &m_id));
        glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));

        // Set texture parameters
        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    }

    // Bind texture
    glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));

    // Set texture
    GLenum dtype = (GLenum)image.getDataType();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)side, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, dtype, image.getData());

    return true;
}


///////////////////////////////////////////////////////////
Shader& Skybox::getShader()
{
    if (!s_shader.getId())
    {
        s_shader.load("shaders/cubemap.vert", Shader::Vertex);
        s_shader.load("shaders/skybox.frag", Shader::Fragment);
        s_shader.compile();
    }

    return s_shader;
}


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
	m_zenithColor		(0.172f, 0.448f, 0.775f),
	m_horizonColor		(0.9f, 1.0f, 0.75f),
    m_groundColor       (0.12f, 0.22f, 0.25f),
	m_bloomColor		(0.8f, 0.75f, 0.5f),
	m_bloomSize 		(0.8f),
	m_lightStrength		(12.0f),
    m_topRadius         (6420.0f),
    m_botRadius         (6360.0f),
    m_altitude          (0.2f),
    m_colorsChanged     (true)
{

}

/*

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
    m_altitude          (0.2f),
    m_colorsChanged     (true)
{

}

*/


///////////////////////////////////////////////////////////
void ProceduralSkybox::init(Scene* scene)
{
	m_scene = scene;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::render(Camera& camera)
{
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
                shader.setUniform("u_bloomColor", light.m_diffuse);
            }
        }
    );

    // Skybox parameters
    shader.setUniform("u_zenithColor", m_zenithColor);
    shader.setUniform("u_horizonColor", m_horizonColor);
    shader.setUniform("u_groundColor", m_groundColor);
    shader.setUniform("u_bloomSize", m_bloomSize);
    shader.setUniform("u_lightStrength", m_lightStrength);
    shader.setUniform("u_topRadius", m_topRadius);
    shader.setUniform("u_botRadius", m_botRadius);
    shader.setUniform("u_radius", m_altitude + m_botRadius);

    // Disable writing to depth buffer
    glCheck(glDepthMask(GL_FALSE));

    // Draw cubemap
    Skybox::getVertexArray().draw();

    glCheck(glDepthMask(GL_TRUE));
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
    m_colorsChanged = true;
}


///////////////////////////////////////////////////////////
void ProceduralSkybox::setHorizonColor(const Vector3f& color)
{
    m_horizonColor = color;
    m_colorsChanged = true;
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


///////////////////////////////////////////////////////////
const Vector3f& ProceduralSkybox::getAmbientColor()
{
    if (m_colorsChanged)
    {
        m_ambient = Vector3f(0.0f);

        const Uint32 NUM_SAMPLES = 50;

        // Get min and max distances
        float r = m_botRadius + m_altitude;
        float d_min = m_topRadius - r;
        float distToHorizon = std::sqrt(r * r - m_botRadius * m_botRadius);
        float distBotToTop = std::sqrt(m_topRadius * m_topRadius - m_botRadius * m_botRadius);
        float d_max = distToHorizon + distBotToTop;

        // Get integration length
        float arc = 0.5f * PI + acosf(m_botRadius / r);
        float dtheta = arc / NUM_SAMPLES;

        for (Uint32 i = 0; i < NUM_SAMPLES; ++i)
        {
            float mu = cosf(dtheta * (i + 0.5f));

            // Get distance to top of atmosphere
            float d = -r * mu + std::sqrt(r * r * (mu * mu - 1.0f) + m_topRadius * m_topRadius);

            // Calculate factor and final color
            float factor = (d - d_min) / (d_max - d_min);
            Vector3f color = m_zenithColor * (1.0f - factor) + m_horizonColor * factor;

            // Add to final color
            float weight = (i == 0 || i == NUM_SAMPLES - 1) ? 0.5f : 1.0f;
            m_ambient += color * weight / (float)NUM_SAMPLES;
        }

        m_colorsChanged = false;
    }

    return m_ambient;
}


}