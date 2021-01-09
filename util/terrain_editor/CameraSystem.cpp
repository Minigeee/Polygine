#include "CameraSystem.h"


///////////////////////////////////////////////////////////
CameraSystem::CameraSystem(Window* window, Camera* camera, UISystem* ui) :
	m_mousePos		(0.0f),
	m_cameraPos		(0.0f),
	m_cameraRot		(0.0f),
	m_cameraDist	(10.0f),
	m_isPressed		(false)
{
	m_window = window;
	m_camera = camera;
	m_ui = ui;

	// Add listeners
	window->addListener<E_MouseMove>(std::bind(&CameraSystem::onMouseMove, this, std::placeholders::_1));
	window->addListener<E_MouseButton>(std::bind(&CameraSystem::onMouseButton, this, std::placeholders::_1));
	window->addListener<E_MouseScroll>(std::bind(&CameraSystem::onMouseScroll, this, std::placeholders::_1));
}


///////////////////////////////////////////////////////////
void CameraSystem::onMouseMove(const E_MouseMove& e)
{
	if (!m_isPressed) return;

	// Get mouse position
	Vector2f mousePos = Vector2f(e.m_x, e.m_y);
	Vector2f offset = mousePos - m_mousePos;
	m_mousePos = mousePos;

	if (m_window->isKeyPressed(Keyboard::LeftShift))
	{
		// Sensitivity
		offset *= 0.01f;

		// Move camera

		const Vector3f& right = m_camera->getRight();
		Vector3f up = cross(right, m_camera->getDirection());

		// Update camera position
		m_cameraPos -= offset.x * right * m_cameraDist;
		m_cameraPos += offset.y * up * m_cameraDist;
	}
	else
	{
		// Sensitivity
		offset *= 0.2f;

		// Rotate camera
		m_cameraRot.x = fmod(m_cameraRot.x - offset.y, 360.0f);
		m_cameraRot.y = fmod(m_cameraRot.y + offset.x, 360.0f);
		if (m_cameraRot.x > 89.0f)
			m_cameraRot.x = 89.0f;
		else if (m_cameraRot.x < -89.0f)
			m_cameraRot.x = -89.0f;

		m_camera->setRotation(m_cameraRot);
	}

	// Update camera position based on rotation
	m_camera->setPosition(m_cameraPos - m_camera->getDirection() * m_cameraDist);
}


///////////////////////////////////////////////////////////
void CameraSystem::onMouseButton(const E_MouseButton& e)
{
	// Only handle right clicks, and if mouse is over a UI element, don't move camera
	if (e.m_button != Mouse::Right || m_ui->hasHover()) return;

	if (e.m_action == InputAction::Press)
	{
		m_isPressed = true;
		m_mousePos = m_window->getCursorPos();
	}
	else
		m_isPressed = false;
}


///////////////////////////////////////////////////////////
void CameraSystem::onMouseScroll(const E_MouseScroll& e)
{
	if (m_ui->hasHover()) return;

	const float minDist = 2.0f;
	const float maxDist = 800.0f;

	float scale = 1.0f - e.m_dy * 0.1f;

	// Update camera dist
	m_cameraDist *= scale;
	if (m_cameraDist < minDist)
		m_cameraDist = minDist;
	else if (m_cameraDist > maxDist)
		m_cameraDist = maxDist;

	m_camera->setPosition(m_cameraPos - m_camera->getDirection() * m_cameraDist);
}