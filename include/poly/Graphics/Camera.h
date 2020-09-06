#ifndef POLY_CAMERA_H
#define POLY_CAMERA_H

#include <poly/Math/Matrix4.h>
#include <poly/Math/Vector3.h>

namespace poly
{

class Camera
{
public:
	Camera();
	~Camera();

	void setPosition(const Vector3f& pos);

	void setPosition(float x, float y, float z);

	void setDirection(const Vector3f& pos);

	void setDirection(float x, float y, float z);

	void setRotation(const Vector2f& rotation);

	void setRotation(float x, float y);

	void setZoom(float zoom);

	void move(const Vector3f& pos);

	void move(float x, float y, float z);

	void zoom(float zoom);

	const Vector3f& getPosition() const;

	const Vector3f& getDirection() const;

	const Vector3f& getRight() const;

	float getZoom() const;

	void setPerspective(float fov, float ar, float near, float far);

	void setFov(float fov);

	void setAspectRatio(float ar);

	void setNear(float near);

	void setFar(float far);

	const Matrix4f& getProjMatrix();

	const Matrix4f& getViewMatrix();

	float getFov() const;

	float getAspectRatio() const;

	float getNear() const;

	float getFar() const;

private:
	Matrix4f m_projMatrix;
	Matrix4f m_viewMatrix;

	Vector3f m_position;
	Vector3f m_direction;
	Vector3f m_right;
	float m_zoom;

	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;

	bool m_isProjDirty;
	bool m_isViewDirty;
};

}

#endif