#include <poly/Graphics/Window.h>

#include "RenderView.h"


///////////////////////////////////////////////////////////
RenderView::RenderView(Camera* camera, FrameBuffer* target) :
	m_camera		(camera),
	m_target		(target),
	m_mousePos		(0.0f),
	m_cameraPos		(0.0f),
	m_cameraRot		(0.0f),
	m_cameraDist	(10.0f),
	m_cameraMode	(false),
	m_paintMode		(false)
{

}


///////////////////////////////////////////////////////////
void RenderView::onBrushDown(const std::function<void()>& func)
{
	m_onBrushDown = func;
}


///////////////////////////////////////////////////////////
void RenderView::onBrushUp(const std::function<void()>& func)
{
	m_onBrushUp = func;
}


///////////////////////////////////////////////////////////
void RenderView::onBrushMove(const std::function<void(const Vector3f&)>& func)
{
	m_onBrushMove = func;
}


void RenderView::terrainPaint(const Vector2f& mousePos)
{
	// Update transforms for size
	updateTransforms();

	// Return if camera is below terrain
	if (m_camera->getPosition().y <= 0.0f)
		return;

	// Get local space coords
	Vector2f p = getLocalCoordinate(mousePos);

	// Get depth value
	float depth = 0.0f;
	m_target->readPixels(&depth, (Uint32)p.x, (Uint32)(m_target->getHeight() - p.y), 1, 1, PixelFormat::Depth, GLType::Float);

	// Get normalized coords
	p /= Vector2f(m_target->getWidth(), m_target->getHeight());
	p.y = 1.0f - p.y;

	// Get fragment location
	Matrix4f invProjView = inverse(m_camera->getProjMatrix() * m_camera->getViewMatrix());
	depth = 2.0f * depth - 1.0f;
	Vector4f clipPos = Vector4f(2.0f * p - 1.0f, depth, 1.0f);
	Vector4f pos = invProjView * clipPos;
	Vector3f intersection = Vector3f(pos) / pos.w;

	// If the intersection is underground, don't count as brush move
	if (m_onBrushMove)
		m_onBrushMove(intersection);
}


///////////////////////////////////////////////////////////
void RenderView::onMouseMove(const E_MouseMove& e)
{
	// Get mouse position
	Vector2f mousePos = Vector2f(e.m_x, e.m_y);
	Vector2f offset = mousePos - m_mousePos;
	m_mousePos = mousePos;
	
	if (m_cameraMode)
	{
		if (Window::getCurrent()->isKeyPressed(Keyboard::LeftShift))
		{
			// Sensitivity
			offset *= 0.01f;

			// Move camera
			const Vector3f& right = m_camera->getRightDir();
			Vector3f up = cross(right, m_camera->getDirection());

			// Update camera position
			m_cameraPos -= offset.x * right * m_cameraDist;
			m_cameraPos += offset.y * up * m_cameraDist;
		}
		else if (Window::getCurrent()->isKeyPressed(Keyboard::LeftControl))
		{
			// Sensitivity
			offset *= 0.01f;

			// Move camera
			const Vector3f& right = m_camera->getRightDir();

			// Update camera position
			m_cameraPos -= offset.x * right * m_cameraDist;
			m_cameraPos += offset.y * m_camera->getDirection() * m_cameraDist;
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

	if (m_paintMode)
		terrainPaint(mousePos);
}


///////////////////////////////////////////////////////////
void RenderView::onMouseButton(const E_MouseButton& e)
{
	if (e.m_button == Mouse::Right)
	{
		if (e.m_action == InputAction::Press)
		{
			m_cameraMode = true;
			m_mousePos = Window::getCurrent()->getCursorPos();
		}
		else
			m_cameraMode = false;
	}
	else if (e.m_button == Mouse::Left)
	{
		if (e.m_action == InputAction::Press)
		{
			m_paintMode = true;
			m_mousePos = Window::getCurrent()->getCursorPos();

			// Paint circle on mouse down
			terrainPaint(Window::getCurrent()->getCursorPos());

			if (m_onBrushDown)
				m_onBrushDown();
		}
		else if (m_paintMode)
		{
			m_paintMode = false;

			if (m_onBrushUp)
				m_onBrushUp();
		}
	}
}


///////////////////////////////////////////////////////////
void RenderView::onMouseScroll(const E_MouseScroll& e)
{
	Window* window = Window::getCurrent();
	if (window->isKeyPressed(Keyboard::LeftControl) ||
		window->isKeyPressed(Keyboard::LeftShift) ||
		window->isKeyPressed(Keyboard::LeftAlt))
		return;

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


///////////////////////////////////////////////////////////
bool RenderView::handlesMouseEvents() const
{
	return true;
}