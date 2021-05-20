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
/// \brief Render data group (used internally)
///
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
	bool m_hasFlippedUv;
};


///////////////////////////////////////////////////////////
/// \brief Quad instance data (used internally)
///
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
/// \brief The UI system that handles rendering, updates, and input events
///
///////////////////////////////////////////////////////////
class UISystem : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	UISystem();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Used to free all textures that were loaded using the
	/// load() function.
	///
	///////////////////////////////////////////////////////////
	~UISystem();

	///////////////////////////////////////////////////////////
	/// \brief Update UI elements and animations
	///
	/// \brief dt The time elapsed since the last frame in seconds
	///
	///////////////////////////////////////////////////////////
	void update(float dt);

	///////////////////////////////////////////////////////////
	/// \brief Render the UI elements
	///
	/// \param target The target framebuffer to render to
	/// \param overlay This should be true if the framebuffer has already been rendered to, and the UI should be overlayed
	///
	///////////////////////////////////////////////////////////
	void render(FrameBuffer& target = FrameBuffer::Default, bool overlay = true);

	///////////////////////////////////////////////////////////
	/// \brief Set the window to register for input events
	///
	/// \param window The window to register for input events
	///
	///////////////////////////////////////////////////////////
	void setWindow(Window* window);

	bool load(const std::string& fname);

	UIElement* getElement(const std::string& id) const;

private:
	UIElement* createElementFromXml(void* node);

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

	HashMap<std::string, UIElement*> m_elements;
	HashMap<std::string, Texture*> m_loadedTextures;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::UISystem
/// \ingroup UI
///
/// A class that handles the rendering, updating, and input
/// events of UI elements. The UISystem inherits from the
/// UIElement, so it can be treated like a UI element, except
/// it does not have any visual appearance. It supports
/// position, rotation, and having a UI element hierarchy.
///
/// To use a UI system, add other UI elements to the system
/// as system, and every time update() or render() is called,
/// the system will handle all element updates and rendering.
/// To support window input events for an interactive UI,
/// the window must be set with setWindow() so the UI system
/// can register event listeners.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a window
/// Window window;
/// window.create(1280, 720, "Window");
///
/// // Create the UI system
/// UISystem ui;
/// ui.setWindow(&window);
///
/// // Add elements with UISystem::addChild()
///
/// while (true)
/// {
///		// Render the UI system
///		ui.render();
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////