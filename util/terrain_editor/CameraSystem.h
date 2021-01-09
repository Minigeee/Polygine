#ifndef TERRAIN_EDITOR_CAMERA_SYSTEM_H
#define TERRAIN_EDITOR_CAMERA_SYSTEM_H

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Window.h>

#include <poly/UI/UISystem.h>

using namespace poly;

class CameraSystem
{
public:
	CameraSystem(Window* window, Camera* camera, UISystem* ui);

private:
	void onMouseMove(const E_MouseMove& e);

	void onMouseButton(const E_MouseButton& e);

	void onMouseScroll(const E_MouseScroll& e);

private:
	Window* m_window;
	Camera* m_camera;
	UISystem* m_ui;

	Vector2f m_mousePos;
	Vector3f m_cameraPos;
	Vector2f m_cameraRot;
	float m_cameraDist;
	bool m_isPressed;
};

#endif