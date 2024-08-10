#ifndef POLY_UI_SYSTEM_H
#define POLY_UI_SYSTEM_H

#include <poly/Core/HandleArray.h>

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
	/// \brief Load a UI layout from an XML file
	///
	/// \param fname The path to the UI XML file
	///
	/// \see load
	///
	///////////////////////////////////////////////////////////
	UISystem(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~UISystem();

	///////////////////////////////////////////////////////////
	/// \brief Initialize the UI system
	///
	/// Creates the render buffer. This can't be done in the constructor
	/// in case some applications initialize a UI system before creating
	/// the window.
	///
	///////////////////////////////////////////////////////////
	void init();

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

	///////////////////////////////////////////////////////////
	/// \brief Load a UI layout from an XML file
	///
	/// This function reads an XML file that contains UI layout data,
	/// and allocates and structures UI elements in a way that matches
	/// the layout described by the file. The main XML node that encompasses
	/// all other nodes should be called "ui", and this node's children
	/// nodes should represent the UI element tree. Each node in the
	/// tree could represent a UI element, and the element type is
	/// based of the node name. Here are the available node names
	/// (their corresponding UI element type should be easy to tell):
	///
	/// \li `ui_element`
	/// \li `button`
	/// \li `dropdown`
	/// \li `h_list_view`
	/// \li `list_view` or `v_list_view`
	/// \li `scroll_view`
	/// \li `slider`
	/// \li `text`
	/// \li `text_input`
	///
	/// To set the default font, add an XML node under the <ui> node
	/// called <font>. Please look at the font overload of UIParser::parse()
	/// for a more in depth description of how to specify a font in an
	/// XML file.
	///
	/// To define UI element "templates", or UI elements that other elements
	/// can base their properties off of, add any template elements under
	/// the <templates> node. Each UI element that is a direct child of the
	/// <template> node should have an "id" attribute, which will be used
	/// as an identifier for that template. To use a template, add a
	/// "template" attribute to any other UI element node, with the value
	/// of the attribute being the id of the template. Note that when applying
	/// any defined template element, the entire subtree of that template
	/// element will be applied. This means that any children elements that
	/// are attached to the main template element will also be added to the
	/// UI element that is using the template.
	///
	/// It is possible to link callback functions from the XML file as well.
	/// To setup callback functions, create the function in c++, then register
	/// the function under a name using UIParser::addCallback(). For non-member
	/// functions (just standard and static functions), it is possible to use
	/// the UI_XML_CALLBACK() macro as a convenience macro function. Then in
	/// the XML file, add an attribute with the desired callback function name
	/// as its value. The name of these callback XML attributes will vary depending
	/// on the event that is being connected. For example, for the Button class,
	/// it is possible to connect the "on_mouse_enter", "on_mouse_leave", "on_press",
	/// "on_release", and "on_cancel" callbacks.
	///
	/// In general, when the property type is a vector type (position, scale, origin,
	/// anchor, etc...), each decimal number in the vector can be separated by
	/// commas, spaces, or both. For anchor, origin, and align, it is possible
	/// to use one of the following strings:
	///
	/// \li `top_left`
	/// \li `top_center`
	/// \li `top_right`
	/// \li `left`
	/// \li `center`
	/// \li `right`
	/// \li `bot_left`
	/// \li `bot_center`
	/// \li `bot_right`
	///
	/// Specifying fonts, textures, and shaders, are slightly more complicated
	/// so please view the overloaded versions of UIParser::parse() of each for
	/// more information.
	///
	/// For specifying color, either 3D or 4D floating point vectors may be used,
	/// as well as a hex code starting with '#'. The color attribute also generally
	/// accepts a wide variety of pre-defined colors, such as:
	///
	/// \li salmon
	/// \li coral
	/// \li steel_blue
	/// \li sea_green
	/// \li lavender
	/// \li maroon
	/// \li 133 more...
	///
	/// For more information on which properties can be applied from an XML
	/// file, please look at the UIElement::parse() overrides of each element.
	/// (No documentation has been written for the structure/format of the
	/// XML files yet, hehe sorry).
	///
	/// ui.xml:
	/// \code
	///
	/// <ui>
	///		<font file="examples/fonts/segoeui/segoeui.ttf" />
	/// 
	/// 	<templates>
	/// 		<button id="num_dropdown_btn" color="sea_green">
	/// 			<button_text color="#AAFFFF" />
	/// 		</button>
	/// 	</templates>
	/// 
	/// 	<list_view id="form1">
	/// 		<button text="Hello #1" position="5,5" size="200,30" color="dark_slate_gray" margins="0,5" on_press="onTestPress" />
	/// 		<text_input value="Testing" position="5,0" size="200,30" color="dark_slate_gray" on_submit="onSubmit" />
	/// 	</list_view>
	/// 
	/// 	<list_view id="form2" position="220,0">
	/// 		<button text="Hello #2" position="5,5" size="200,30" color="dark_slate_gray" margins="0,5" on_press="onTestPress" />
	/// 		<text_input value="Test" position="5,0" size="200,30" color="dark_slate_gray" />
	/// 	</list_view>
	/// 
	/// 	<dropdown position="5 100" size="200 30" color="dark_slate_gray" item_color="slate_gray">
	/// 		<dropdown_item template="num_dropdown_btn" value="One" />
	/// 		<dropdown_item template="num_dropdown_btn" value="Two" />
	/// 		<dropdown_item template="num_dropdown_btn" value="Three" />
	/// 	</dropdown>
	/// </ui>
	///
	/// \endcode
	///
	/// Main.cpp:
	/// \code
	///
	/// using namespace poly;
	/// 
	/// // Setup the callback functions
	///
	/// // When the first parameter of the callback function is a UI element,
	/// // the parser will attempt to pass the element that sent the callback event
	/// // so that the callback function has access to its sender
	/// void onTestPress(UIElement* elem)
	/// {
	/// 	Button* btn = (Button*)elem;
	/// 	std::cout << btn->getString().c_str() << '\n';
	/// }
	/// 
	/// void onSubmit(const std::string& text)
	/// {
	/// 	std::cout << text.c_str() << '\n';
	/// }
	///
	/// // Register the callback functions
	/// UIParser::addCallback("onTestPress", std::function<void(UIElement*)>(onTestPress));
	///
	/// // This following macro only works for non-member functions
    /// UI_XML_CALLBACK(onSubmit);
	///
	/// // To add a member function, use something like this
	/// // std::bind(&Class::function, &instance, std::placeholders::_1, ...);
	///
	///
	/// Window window;
	/// UISystem ui;
	/// ui.setWindow(&window);
	/// ui.load("ui.xml");
	///
	/// ...
	///
	/// \endcode
	///
	/// \param fname The path to the UI XML file
	///
	/// \return True if the file was loaded without any errors
	///
	///////////////////////////////////////////////////////////
	bool load(const std::string& fname);

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to an element by id
	///
	/// This function is to access UI elements that were loaded
	/// from a UI XML file.
	///
	/// \param id The id of the element
	///
	/// \return A pointer to the UI element
	///
	///////////////////////////////////////////////////////////
	UIElement* getElement(const std::string& id) const;

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

	HashMap<std::string, UIElement*> m_elements;
	bool m_loaded;
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