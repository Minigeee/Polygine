#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/UniformBuffer.h>

#include <poly/Math/Functions.h>
#include <poly/Math/Transform.h>

namespace poly
{


///////////////////////////////////////////////////////////
std::vector<UniformBuffer*> Camera::s_unusedUniformBuffers;


///////////////////////////////////////////////////////////
Camera::Camera() :
	m_projMatrix	(1.0f),
	m_viewMatrix	(1.0f),

	m_position		(0.0f),
	m_direction		(0.0f, 0.0f, -1.0f),
	m_rightDir		(1.0f, 0.0f, 0.0f),
	m_zoom			(1.0f),

	m_fov			(90.0f),
	m_aspectRatio	(16.0f / 9.0f),
	m_near			(0.1f),
	m_far			(1000.0f),

	m_isPerspective	(true),
	m_isProjDirty	(true),
	m_isViewDirty	(true),
	m_isBufferDirty	(true),

	m_uniformBuffer	(getUniformBuffer())
{

}


///////////////////////////////////////////////////////////
Camera::~Camera()
{
	Camera::free(m_uniformBuffer);
}


///////////////////////////////////////////////////////////
UniformBuffer* Camera::getUniformBuffer()
{
	if (s_unusedUniformBuffers.size())
	{
		// Get an unused buffer if available
		UniformBuffer* buffer = s_unusedUniformBuffers.back();
		s_unusedUniformBuffers.pop_back();
		return buffer;
	}
	else
	{
		// Allocate a new one
		UniformBuffer* buffer = Pool<UniformBuffer>::alloc();

		// Make enough space for 10 blocks
		Uint32 align = UniformBuffer::getUniformBlockAlignment();
		Uint32 size = (sizeof(UniformBlock_Camera) + align - 1) / align * align;
		buffer->create(size * 10);

		return buffer;
	}
}


///////////////////////////////////////////////////////////
void Camera::free(UniformBuffer* buffer)
{
	// Return to unused buffers
	s_unusedUniformBuffers.push_back(buffer);
}


///////////////////////////////////////////////////////////
void Camera::apply(Shader* shader)
{
	if (m_isProjDirty || m_isViewDirty || m_isBufferDirty)
	{
		UniformBlock_Camera block;
		block.m_projView = getProjMatrix() * getViewMatrix();
		block.m_cameraPos = m_position;
		block.m_near = m_near;
		block.m_far = m_far;

		m_uniformBuffer->pushData(block);

		m_isBufferDirty = false;
	}

	// Bind to shader
	shader->bindUniformBlock("Camera", *m_uniformBuffer);
}


///////////////////////////////////////////////////////////
void Camera::setPosition(const Vector3f& pos)
{
	m_position = pos;
	m_isViewDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setPosition(float x, float y, float z)
{
	m_position = Vector3f(x, y, z);
	m_isViewDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setDirection(const Vector3f& dir)
{
	m_direction = normalize(dir);
	m_rightDir = normalize(cross(m_direction, Vector3f(0.0f, 1.0f, 0.0f)));
	m_isViewDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setDirection(float x, float y, float z)
{
	m_direction = normalize(Vector3f(x, y, z));
	m_rightDir = normalize(cross(m_direction, Vector3f(0.0f, 1.0f, 0.0f)));
	m_isViewDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setRotation(const Vector2f& rotation)
{
	float x = rad(rotation.x);
	float y = rad(rotation.y - 90.0f);

	float cx = cos(x);
	float cy = cos(y);
	float sx = sin(x);
	float sy = sin(y);

	m_direction = normalize(Vector3f(cy * cx, sx, sy * cx));
	m_rightDir = normalize(cross(m_direction, Vector3f(0.0f, 1.0f, 0.0f)));

	m_isViewDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setRotation(float x, float y)
{
	setRotation(Vector2f(x, y));
}


///////////////////////////////////////////////////////////
void Camera::setZoom(float zoom)
{
	m_zoom = zoom;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::move(const Vector3f& pos)
{
	m_position += pos;
	m_isViewDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::move(float x, float y, float z)
{
	m_position += Vector3f(x, y, z);
	m_isViewDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::zoom(float zoom)
{
	m_zoom *= zoom;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
const Vector3f& Camera::getPosition() const
{
	return m_position;
}


///////////////////////////////////////////////////////////
const Vector3f& Camera::getDirection() const
{
	return m_direction;
}


///////////////////////////////////////////////////////////
const Vector3f& Camera::getRightDir() const
{
	return m_rightDir;
}


///////////////////////////////////////////////////////////
float Camera::getZoom() const
{
	return m_zoom;
}


///////////////////////////////////////////////////////////
void Camera::setPerspective(float fov, float ar, float near, float far)
{
	m_fov = fov;
	m_aspectRatio = ar;
	m_near = near;
	m_far = far;
	m_isPerspective = true;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setFov(float fov)
{
	m_fov = fov;
	m_isPerspective = true;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setAspectRatio(float ar)
{
	m_aspectRatio = ar;
	m_isPerspective = true;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setNear(float near)
{
	m_near = near;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setFar(float far)
{
	m_far = far;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	m_left = left;
	m_right = right;
	m_bottom = bottom;
	m_top = top;
	m_near = near;
	m_far = far;
	m_isPerspective = false;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setLeft(float left)
{
	m_left = left;
	m_isPerspective = false;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setRight(float right)
{
	m_right = right;
	m_isPerspective = false;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setBottom(float bottom)
{
	m_bottom = bottom;
	m_isPerspective = false;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
void Camera::setTop(float top)
{
	m_top = top;
	m_isPerspective = false;
	m_isProjDirty = true;
}


///////////////////////////////////////////////////////////
const Matrix4f& Camera::getProjMatrix()
{
	if (m_isProjDirty)
	{
		if (m_isPerspective)
			m_projMatrix = toPerspectiveMatrix(m_fov * m_zoom, m_aspectRatio, m_near, m_far);
		else
			m_projMatrix = toOrthographicMatrix(m_left, m_right, m_bottom, m_top, m_near, m_far);

		m_isProjDirty = false;
		m_isBufferDirty = true;
	}

	return m_projMatrix;
}


///////////////////////////////////////////////////////////
const Matrix4f& Camera::getViewMatrix()
{
	if (m_isViewDirty)
	{
		m_viewMatrix = toViewMatrix(m_position, m_direction, m_rightDir);

		m_isViewDirty = false;
		m_isBufferDirty = true;
	}

	return m_viewMatrix;
}


///////////////////////////////////////////////////////////
const Frustum& Camera::getFrustum()
{
	Matrix4f m = getProjMatrix() * getViewMatrix();

	m_frustum.setPlane(Plane(
		m.x.w + m.x.x,
		m.y.w + m.y.x,
		m.z.w + m.z.x,
		m.w.w + m.w.x
	), Frustum::Left);

	m_frustum.setPlane(Plane(
		m.x.w - m.x.x,
		m.y.w - m.y.x,
		m.z.w - m.z.x,
		m.w.w - m.w.x
	), Frustum::Right);

	m_frustum.setPlane(Plane(
		m.x.w + m.x.y,
		m.y.w + m.y.y,
		m.z.w + m.z.y,
		m.w.w + m.w.y
	), Frustum::Bottom);

	m_frustum.setPlane(Plane(
		m.x.w - m.x.y,
		m.y.w - m.y.y,
		m.z.w - m.z.y,
		m.w.w - m.w.y
	), Frustum::Top);

	m_frustum.setPlane(Plane(
		m.x.w + m.x.z,
		m.y.w + m.y.z,
		m.z.w + m.z.z,
		m.w.w + m.w.z
	), Frustum::Near);

	m_frustum.setPlane(Plane(
		m.x.w - m.x.z,
		m.y.w - m.y.z,
		m.z.w - m.z.z,
		m.w.w - m.w.z
	), Frustum::Far);

	return m_frustum;
}


///////////////////////////////////////////////////////////
float Camera::getFov() const
{
	return m_fov;
}


///////////////////////////////////////////////////////////
float Camera::getAspectRatio() const
{
	return m_aspectRatio;
}


///////////////////////////////////////////////////////////
float Camera::getNear() const
{
	return m_near;
}


///////////////////////////////////////////////////////////
float Camera::getFar() const
{
	return m_far;
}


///////////////////////////////////////////////////////////
float Camera::getLeft() const
{
	return m_left;
}


///////////////////////////////////////////////////////////
float Camera::getRight() const
{
	return m_right;
}


///////////////////////////////////////////////////////////
float Camera::getBottom() const
{
	return m_bottom;
}


///////////////////////////////////////////////////////////
float Camera::getTop() const
{
	return m_top;
}


}