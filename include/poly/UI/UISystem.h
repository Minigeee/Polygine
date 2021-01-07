#ifndef POLY_UI_SYSTEM_H
#define POLY_UI_SYSTEM_H

#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <poly/UI/UIElement.h>

namespace poly
{

class Window;


///////////////////////////////////////////////////////////
struct UIRenderData
{
	Texture* m_texture;
	BlendFactor m_srcFactor;
	BlendFactor m_dstFactor;
	Vector4f m_blendColor;		//!< This is exclusively for text rendering atm
	Shader* m_shader;
	Vector4f m_clipRect;
	Uint32 m_offset;
	Uint32 m_instances;
	bool m_transparent;
};


///////////////////////////////////////////////////////////
struct UIInstanceData
{
	Vector2f m_position;
	float m_rotation;
	Vector2f m_size;
	Vector2f m_origin;
	Vector4f m_color;
	Vector4f m_textureRect;
	float m_index;
};


///////////////////////////////////////////////////////////
/// \brief TODO
///
///////////////////////////////////////////////////////////
class UISystem :
	public UIElement
{
public:
	UISystem();

	UISystem(Uint32 w, Uint32 h);

	void update(float dt);

	void render(FrameBuffer& target = FrameBuffer::Default, bool overlay = true);

	void setWindow(Window* window);

private:
	void getRenderQuads(
		UIElement* element,
		std::vector<std::vector<UIQuad>>& quads,
		std::vector<UIRenderData>& renderData,
		std::vector<UIQuad>& transparentQuads,
		std::vector<UIRenderData>& transparentRenderData,
		const Vector4f& clipRect,
		Uint32& index
	);

	void updateElement(UIElement* element, float dt);

	void onKeyEvent(const E_KeyEvent& e) override;

	void onMouseButton(const E_MouseButton& e) override;

	void onMouseMove(const E_MouseMove& e) override;

	void onMouseScroll(const E_MouseScroll& e) override;

	void onTextInput(const E_TextInput& e) override;

	bool relayMouseMove(UIElement* element, const E_MouseMove& e);

private:
	Window* m_window;
	Handle m_onKeyEventHandle;
	Handle m_onMouseButtonHandle;
	Handle m_onMouseMoveHandle;
	Handle m_onMouseScrollHandle;
	Handle m_onTextInputHandle;

	VertexArray m_vertexArray;
	VertexBuffer m_instanceBuffer;						//!< The instance buffer that stores instance transform data
	Uint32 m_instanceBufferOffset;						//!< The offset of the valid range of the instance buffer

	UIElement* m_hovered;
	UIElement* m_focused;
};

}

#endif