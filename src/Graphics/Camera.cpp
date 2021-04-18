#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/UniformBlock.h>

#include <poly/Math/Functions.h>
#include <poly/Math/Transform.h>

namespace poly
{


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

	m_uniformBlock	(new UniformBlock())
{
	Uint32 size = 0;
	size += 16 * sizeof(float);
	size += 4 * sizeof(float);
	size += sizeof(float);
	size += sizeof(float);

	size = (size + 15) / 16 * 16;

	m_uniformBlock->setBufferSize(size * 10);
}


///////////////////////////////////////////////////////////
Camera::~Camera()
{
	delete m_uniformBlock;
}


///////////////////////////////////////////////////////////
void Camera::apply(Shader* shader)
{
	if (m_isProjDirty || m_isViewDirty || m_isBufferDirty)
	{
		m_uniformBlock->addData(getProjMatrix() * getViewMatrix());
		m_uniformBlock->addData(m_position);
		m_uniformBlock->addData(m_near);
		m_uniformBlock->addData(m_far);

		m_uniformBlock->update();
	}

	// Bind to shader
	shader->setUniformBlock("Camera", *m_uniformBlock);
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