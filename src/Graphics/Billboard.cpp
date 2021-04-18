#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Billboard.h>
#include <poly/Graphics/Material.h>
#include <poly/Graphics/Texture.h>

namespace poly
{


///////////////////////////////////////////////////////////
VertexArray Billboard::s_vertexArray;

///////////////////////////////////////////////////////////
Shader Billboard::s_shader;


///////////////////////////////////////////////////////////
VertexArray& Billboard::getVertexArray()
{
	if (!s_vertexArray.getId())
	{
		s_vertexArray.bind();
		s_vertexArray.setDrawMode(DrawMode::Points);
		s_vertexArray.setNumVertices(1);
	}

	return s_vertexArray;
}


///////////////////////////////////////////////////////////
Shader& Billboard::getDefaultShader()
{
	if (!s_shader.getId())
	{
		s_shader.load("shaders/billboard.vert", Shader::Vertex);
		s_shader.load("shaders/billboard.geom", Shader::Geometry);
		s_shader.load("shaders/billboard.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


///////////////////////////////////////////////////////////
Billboard::Billboard() :
	m_material			(Pool<Material>::alloc()),
	m_texture			(0),
	m_size				(1.0f),
	m_origin			(0.5f),
	m_axisLocked		(false),
	m_lightingEnabled	(false),
	m_shadowingEnabled	(false)
{
	// By default, the bounding volumes should have a value of 1
	m_boundingBox.m_min = Vector3f(-0.5f);
	m_boundingBox.m_max = Vector3f(0.5f);

	m_boundingSphere.m_radius = sqrtf(0.5f);
}


///////////////////////////////////////////////////////////
Billboard::~Billboard()
{
	if (m_material)
		Pool<Material>::free(m_material);
	if (m_texture)
		Pool<Texture>::free(m_texture);

	m_material = 0;
	m_texture = 0;
}


///////////////////////////////////////////////////////////
bool Billboard::load(const std::string& fname)
{
	// Create and set the texture
	if (!m_texture)
		m_texture = Pool<Texture>::alloc();
	m_material->setDiffTexture(m_texture);

	// Load the image
	Image image;
	if (!image.load(fname))
		return false;

	// Set texture data
	m_texture->create(image);

	return true;
}


///////////////////////////////////////////////////////////
void Billboard::setSize(const Vector2f& size)
{
	m_size = size;

	// Update bounding volumes
	updateBoundingVolumes();
}


///////////////////////////////////////////////////////////
void Billboard::setSize(float w, float h)
{
	m_size = Vector2f(w, h);

	// Update bounding volumes
	updateBoundingVolumes();
}


///////////////////////////////////////////////////////////
void Billboard::setOrigin(const Vector2f& origin)
{
	m_origin = origin;

	if (m_origin.x < 0.0f)
		m_origin.x = 0.0f;
	else if (m_origin.x > 1.0f)
		m_origin.x = 1.0f;

	if (m_origin.y < 0.0f)
		m_origin.y = 0.0f;
	else if (m_origin.y > 1.0f)
		m_origin.y = 1.0f;

	// Update bounding volumes
	updateBoundingVolumes();
}


///////////////////////////////////////////////////////////
void Billboard::setOrigin(float x, float y)
{
	setOrigin(Vector2f(x, y));
}


///////////////////////////////////////////////////////////
void Billboard::setAxisLocked(bool locked)
{
	m_axisLocked = locked;
}


///////////////////////////////////////////////////////////
void Billboard::setLightingEnabled(bool enabled)
{
	m_lightingEnabled = enabled;
}


///////////////////////////////////////////////////////////
void Billboard::setShadowingEnabled(bool enabled)
{
	m_shadowingEnabled = enabled;
}


///////////////////////////////////////////////////////////
Material* Billboard::getMaterial() const
{
	return m_material;
}


///////////////////////////////////////////////////////////
Texture* Billboard::getTexture() const
{
	return m_texture;
}


///////////////////////////////////////////////////////////
const Vector2f& Billboard::getSize() const
{
	return m_size;
}


///////////////////////////////////////////////////////////
const Vector2f& Billboard::getOrigin() const
{
	return m_origin;
}


///////////////////////////////////////////////////////////
bool Billboard::isAxisLocked() const
{
	return m_axisLocked;
}


///////////////////////////////////////////////////////////
bool Billboard::isLightingEnabled() const
{
	return m_lightingEnabled;
}


///////////////////////////////////////////////////////////
bool Billboard::isShadowingEnabled() const
{
	return m_shadowingEnabled;
}


///////////////////////////////////////////////////////////
void Billboard::updateBoundingVolumes()
{
	// Bounding box
	m_boundingBox.m_min = Vector3f(-m_origin.x, -m_origin.y, -0.5f) * Vector3f(m_size.x, m_size.y, m_size.x);
	m_boundingBox.m_max = (m_boundingBox.m_min + 1.0f) * Vector3f(m_size.x, m_size.y, m_size.x);

	// Bounding sphere
	m_boundingSphere.m_position.x = -m_origin.x + 0.5f;
	m_boundingSphere.m_position.y = -m_origin.y + 0.5f;
	m_boundingSphere.m_position.z = 0.0f;
	m_boundingSphere.m_radius = sqrtf(2.0f * powf(0.5f * std::max(m_size.x, m_size.y), 2.0f));
}


}