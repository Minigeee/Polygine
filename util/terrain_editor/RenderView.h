#ifndef TERRAIN_EDITOR_RENDER_VIEW_H
#define TERRAIN_EDITOR_RENDER_VIEW_H

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/FrameBuffer.h>

#include <poly/UI/UIElement.h>

using namespace poly;

class RenderView : public UIElement
{
public:
	RenderView(Camera* camera, FrameBuffer* target);

	void onBrushDown(const std::function<void()>& func);

	void onBrushUp(const std::function<void()>& func);

	void onBrushMove(const std::function<void(const Vector3f&)>& func);

private:
	void onMouseMove(const E_MouseMove& e) override;

	void onMouseButton(const E_MouseButton& e) override;

	void onMouseScroll(const E_MouseScroll& e) override;

	bool handlesMouseEvents() const override;

private:
	Camera* m_camera;
	FrameBuffer* m_target;

	Vector2f m_mousePos;
	Vector3f m_cameraPos;
	Vector2f m_cameraRot;
	float m_cameraDist;
	bool m_cameraMode;
	bool m_paintMode;

	std::function<void()> m_onBrushDown;
	std::function<void()> m_onBrushUp;
	std::function<void(const Vector3f&)> m_onBrushMove;
};

#endif