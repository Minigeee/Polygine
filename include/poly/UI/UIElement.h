#ifndef POLY_UI_ELEMENT_H
#define POLY_UI_ELEMENT_H

#include <poly/Core/XmlNode.h>

#include <poly/Engine/Input.h>

#include <poly/Graphics/GLType.h>
#include <poly/Graphics/Shader.h>

#include <poly/Math/Vector2.h>

#include <vector>

namespace poly
{

class Texture;
class UISystem;

///////////////////////////////////////////////////////////
typedef HashMap<std::string, XmlNode> UITemplateMap;


///////////////////////////////////////////////////////////
/// \brief An enum that describes relative UI element positions
///
///////////////////////////////////////////////////////////
enum class UIPosition
{
	TopLeft,
	TopCenter,
	TopRight,
	Left,
	Center,
	Right,
	BotLeft,
	BotCenter,
	BotRight
};


///////////////////////////////////////////////////////////
/// \brief An intermediate struct used to render UI quads
///
///////////////////////////////////////////////////////////
struct UIQuad
{
	Vector2f m_position;
	float m_rotation;
	Vector2f m_size;
	Vector2f m_origin;
	Vector4f m_color;
	Vector4f m_textureRect;
	Uint32 m_index;
};


///////////////////////////////////////////////////////////
/// \brief The base class for all UI elements
///
///////////////////////////////////////////////////////////
class UIElement
{
	friend UISystem;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	UIElement();

	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~UIElement();

	///////////////////////////////////////////////////////////
	/// \brief Parse properties for this UI element from an XML node
	///
	/// This function will parse and apply properties found in the
	/// given XML node. This applies to most of the properties that
	/// are modifiable, and it is even possible to parse callback
	/// functions. Before parsing and applying element properties,
	/// any child XML nodes are checked to see if they represent
	/// a UI element. If any child nodes of the given XML node represent
	/// a UI element, a new element is created, based on the name of
	/// the XML node, and that new element is parsed. Any children
	/// elements found in the XML node will be allocated and parsed
	/// recursively, using this parse() function.
	///
	/// The \a templates parameter should contain a map of template
	/// names (string) to their respective XML node. The templates map
	/// is needed to apply any template elements that have been defiend
	/// and are being used.
	///
	/// To parse callback functions, each callback function must be
	/// added to the UI parser using UIParser::addCallback().
	///
	/// In most cases, the user won't need to call this function, as
	/// it is automatically called from UISystem::load(), where it is
	/// used to parse an already loaded XML file.
	///
	/// For an example of how to structure UI XML files, please see
	/// UISystem::load().
	///
	/// \param node The XML node to parse properties and children elements from
	/// \param templates The map of template names to XML nodes
	///
	///////////////////////////////////////////////////////////
	virtual void parse(XmlNode node, const UITemplateMap& templates = UITemplateMap());

	///////////////////////////////////////////////////////////
	/// \brief Execute any updates for this UI element, including UI animations
	///
	/// \param dt The time elapsed since the last frame in seconds
	///
	///////////////////////////////////////////////////////////
	virtual void update(float dt);

	///////////////////////////////////////////////////////////
	/// \brief Add a child UI element
	///
	/// The added element goes to the back of the list of children,
	/// meaning that it gets rendered on top of its siblings.
	///
	/// \param child A pointer to a UI element
	///
	///////////////////////////////////////////////////////////
	virtual void addChild(UIElement* child);

	///////////////////////////////////////////////////////////
	/// \brief Remove a child UI element
	///
	/// \param child A pointer to a child UI element
	///
	///////////////////////////////////////////////////////////
	virtual void removeChild(UIElement* child);

	///////////////////////////////////////////////////////////
	/// /brief Move the UI element in front of all its sibling elements
	///
	///////////////////////////////////////////////////////////
	void moveToFront();

	///////////////////////////////////////////////////////////
	/// /brief Move the UI element behind all its sibling elements
	///
	///////////////////////////////////////////////////////////
	void moveToBack();

	///////////////////////////////////////////////////////////
	/// \brief Set the element string id
	///
	/// The element id is an optional string identifier, that can
	/// be used to identify certain important elements when working
	/// with many elements. It is really only useful if the ui layout
	/// is loaded from a ui layout file.
	///
	/// \param id The string id
	///
	///////////////////////////////////////////////////////////
	void setId(const std::string& id);

	///////////////////////////////////////////////////////////
	/// \brief Set the UI element's z-index
	///
	/// The z-index determines what order this element and its
	/// sibling elements will be rendered in, where an index of 0
	/// would mean that the element is rendered first.
	///
	/// \param index The z-index
	///
	///////////////////////////////////////////////////////////
	void setIndex(Uint32 index);

	///////////////////////////////////////////////////////////
	/// \brief Set the UI element's relative position in pixels
	///
	/// This sets the element's position relative to its parent
	/// element.
	///
	/// \param pos The relative position in pixels
	///
	///////////////////////////////////////////////////////////
	virtual void setPosition(const Vector2f& pos);

	///////////////////////////////////////////////////////////
	/// \brief Set the UI element's relative position in pixels
	///
	/// This sets the element's position relative to its parent
	/// element.
	///
	/// \param x The relative x-coordinate in pixels
	/// \param y The relative y-coordinate in pixels
	///
	///////////////////////////////////////////////////////////
	virtual void setPosition(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Set the UI element's relative rotation in degrees
	///
	/// This sets the element's rotation relative to its parent
	/// element.
	///
	/// \param rot The relative rotation in degrees
	///
	///////////////////////////////////////////////////////////
	virtual void setRotation(float rot);

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the UI element in pixels
	///
	/// This sets the size of the element in pixels, so that the size
	/// of the element does not change, even if the size of its
	/// parent element changes.
	///
	/// \param size The element size in pixels
	///
	///////////////////////////////////////////////////////////
	virtual void setSize(const Vector2f& size);

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the UI element in pixels
	///
	/// This sets the size of the element in pixels, so that the size
	/// of the element does not change, even if the size of its
	/// parent element changes.
	///
	/// \param w The element width in pixels
	/// \param h The element height in pixels
	///
	///////////////////////////////////////////////////////////
	virtual void setSize(float w, float h);

	///////////////////////////////////////////////////////////
	/// \brief Set the relative size of the UI element as a percentage of its parent element
	///
	/// This sets the size of the element as a decimal percentage of
	/// its parent element, where a size of 1.0 is the size of the
	/// parent element. This means that whenever the pixel size of the
	/// parent element changes, the pixel size of this element will
	/// change as well.
	///
	/// \param size The relative element size
	///
	///////////////////////////////////////////////////////////
	virtual void setRelSize(const Vector2f& size);

	///////////////////////////////////////////////////////////
	/// \brief Set the relative size of the UI element as a percentage of its parent element
	///
	/// This sets the size of the element as a decimal percentage of
	/// its parent element, where a size of 1.0 is the size of the
	/// parent element. This means that whenever the pixel size of the
	/// parent element changes, the pixel size of this element will
	/// change as well.
	///
	/// \param w The relative element width
	/// \param h The relative element height
	///
	///////////////////////////////////////////////////////////
	virtual void setRelSize(float w, float h);

	///////////////////////////////////////////////////////////
	/// \brief Set the width of the UI element in pixels
	///
	/// This sets the wdith of the element in pixels, so that the
	/// width of the element does not change when the width of its
	/// parent element changes.
	///
	/// This can be used to give the element a fixed width, with
	/// a dynamic height that scales off its parent element.
	///
	/// \param w The element width in pixels
	///
	///////////////////////////////////////////////////////////
	virtual void setWidth(float w);

	///////////////////////////////////////////////////////////
	/// \brief Set the height of the UI element in pixels
	///
	/// This sets the height of the element in pixels, so that the
	/// height of the element does not change when the height of its
	/// parent element changes.
	///
	/// This can be used to give the element a fixed height, with
	/// a dynamic width that scales off its parent element.
	///
	/// \param w The element height in pixels
	///
	///////////////////////////////////////////////////////////
	virtual void setHeight(float h);

	///////////////////////////////////////////////////////////
	/// \brief Set the relative width of the UI element as a percentage of its parent element
	///
	/// This sets the width of the element as a decimal percentage of
	/// its parent element, where a width of 1.0 is the width of the
	/// parent element. This means that whenever the pixel width of the
	/// parent element changes, the pixel width of this element will
	/// change as well.
	///
	/// This can be used to give the element a dynamic width that
	/// scales off its parent element, with a height that has a fixed
	/// pixel value.
	///
	/// \param w The relative width of the element
	///
	///////////////////////////////////////////////////////////
	virtual void setRelWidth(float w);

	///////////////////////////////////////////////////////////
	/// \brief Set the relative height of the UI element as a percentage of its parent element
	///
	/// This sets the height of the element as a decimal percentage of
	/// its parent element, where a height of 1.0 is the height of the
	/// parent element. This means that whenever the pixel height of the
	/// parent element changes, the pixel height of this element will
	/// change as well.
	///
	/// This can be used to give the element a dynamic height that
	/// scales off its parent element, with a width that has a fixed
	/// pixel value.
	///
	/// \param w The relative height of the element
	///
	///////////////////////////////////////////////////////////
	virtual void setRelHeight(float h);

	///////////////////////////////////////////////////////////
	/// \brief Set the origin of the UI element
	///
	/// The origin of the element is the center of its coordinate
	/// system. The origin should be a decimal value that is a
	/// percentage of its pixel size, where (0, 0) would be the
	/// top-left corner of the element and (1, 1) would be the
	/// bottom right corner of the element.
	///
	/// \param origin The origin of the element
	///
	///////////////////////////////////////////////////////////
	virtual void setOrigin(const Vector2f& origin);

	///////////////////////////////////////////////////////////
	/// \brief Set the origin of the UI element
	///
	/// The origin of the element is the center of its coordinate
	/// system. The origin should be a decimal value that is a
	/// percentage of its pixel size, where (0, 0) would be the
	/// top-left corner of the element and (1, 1) would be the
	/// bottom right corner of the element.
	///
	/// \param x The x-coordinate of the origin of the element
	/// \param y The y-coordinate of the origin of the element
	///
	///////////////////////////////////////////////////////////
	virtual void setOrigin(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Set the origin of the UI element using a predefined location
	///
	/// The origin of the element is the center of its coordinate
	/// system.
	///
	/// \param x The x-coordinate of the origin of the element
	/// \param y The y-coordinate of the origin of the element
	///
	///////////////////////////////////////////////////////////
	virtual void setOrigin(UIPosition origin);

	///////////////////////////////////////////////////////////
	/// \brief Set the anchor of the UI element
	///
	/// The anchor of the element is the adjustable origin of the
	/// coordinate space the element is placed in. The anchor should
	/// be a decimal value that is a percentage of its pixel size,
	/// where (0, 0) would be the top-left corner of its parent 
	/// element and (1, 1) would be the bottom right corner of
	/// its parent element.
	///
	/// For example, if the element as a position of (5, 5) with
	/// an anchor of (0, 0), it would be placed 5 pixels down and
	/// to the right of the top left corner of its parent element.
	/// If it kept the same position but is given an anchor of (1, 0),
	/// the element would be placed 5 pixels down and to the right of
	/// the parent element's top right corner.
	///
	/// \param anchor The anchor of the element
	///
	///////////////////////////////////////////////////////////
	virtual void setAnchor(const Vector2f& anchor);

	///////////////////////////////////////////////////////////
	/// \brief Set the anchor of the UI element
	///
	/// The anchor of the element is the adjustable origin of the
	/// coordinate space the element is placed in. The anchor should
	/// be a decimal value that is a percentage of its pixel size,
	/// where (0, 0) would be the top-left corner of its parent 
	/// element and (1, 1) would be the bottom right corner of
	/// its parent element.
	///
	/// For example, if the element as a position of (5, 5) with
	/// an anchor of (0, 0), it would be placed 5 pixels down and
	/// to the right of the top left corner of its parent element.
	/// If it kept the same position but is given an anchor of (1, 0),
	/// the element would be placed 5 pixels down and to the right of
	/// the parent element's top right corner.
	///
	/// \param x The x-coordinate of the anchor
	/// \param y The y-coordinate of the anchor
	///
	///////////////////////////////////////////////////////////
	virtual void setAnchor(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Set the anchor of the UI element using a predefined position
	///
	/// The anchor of the element is the adjustable origin of the
	/// coordinate space the element is placed in. The anchor should
	/// be a decimal value that is a percentage of its pixel size,
	/// where (0, 0) would be the top-left corner of its parent 
	/// element and (1, 1) would be the bottom right corner of
	/// its parent element.
	///
	/// For example, if the element as a position of (5, 5) with
	/// an anchor of (0, 0), it would be placed 5 pixels down and
	/// to the right of the top left corner of its parent element.
	/// If it kept the same position but is given an anchor of (1, 0),
	/// the element would be placed 5 pixels down and to the right of
	/// the parent element's top right corner.
	///
	/// \param anchor The anchor of the element
	///
	///////////////////////////////////////////////////////////
	virtual void setAnchor(UIPosition anchor);

	///////////////////////////////////////////////////////////
	/// \brief Set the background color of the UI element
	///
	/// \param color The RGBA color of the element
	///
	///////////////////////////////////////////////////////////
	virtual void setColor(const Vector4f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the background color of the UI element
	///
	/// \param r The r-component of the background color
	/// \param g The g-component of the background color
	/// \param b The b-component of the background color
	/// \param a The a-component of the background color
	///
	///////////////////////////////////////////////////////////
	virtual void setColor(float r, float g, float b, float a);

	///////////////////////////////////////////////////////////
	/// \brief Set the background texture of the UI element
	///
	/// This texture is displayed on the UI element quad, with no
	/// padding between the edge of the texture and the edge of
	/// the quad. The portion of the texture that gets displayed
	/// can be controlled by setting the texture rectangle.
	///
	/// \param texture A pointer to the texture to display
	///
	/// \see setTextureRect
	///
	///////////////////////////////////////////////////////////
	void setTexture(Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Set the portion of the rectangle that gets displayed with a subregion rectangle
	///
	/// The texture rectangle includes a coordinate marking the
	/// top-left corner of the texture region, and a size indicating
	/// the size of the texture region that gets displayed. Both
	/// the coordinate and the size should be in UV coordinate space,
	/// where (0, 0) is the top-left of the texture and (1, 1) is
	/// the bottom-right of the texture.
	///
	/// The \a rect parameter should contain the texture rectangle
	/// data in the format: (x, y, w, h).
	///
	/// \param rect The texture rectangle
	///
	///////////////////////////////////////////////////////////
	void setTextureRect(const Vector4f& rect);

	///////////////////////////////////////////////////////////
	/// \brief Set the portion of the rectangle that gets displayed with a subregion rectangle
	///
	/// The texture rectangle includes a coordinate marking the
	/// top-left corner of the texture region, and a size indicating
	/// the size of the texture region that gets displayed. Both
	/// the coordinate and the size should be in UV coordinate space,
	/// where (0, 0) is the top-left of the texture and (1, 1) is
	/// the bottom-right of the texture.
	///
	/// \param x The x-coordinate of the texture rectangle in UV coordinate space
	/// \param y The y-coordinate of the texture rectangle in UV coordinate space
	/// \param w The width of the texture rectangle in UV coordinate space
	/// \param h The height of the texture rectangle in UV coordinate space
	///
	///////////////////////////////////////////////////////////
	void setTextureRect(float x, float y, float w, float h);

	///////////////////////////////////////////////////////////
	/// \brief Set whether the y-axis of the texture should be flipped or not
	///
	/// \param flipped The flipped UV flag
	///
	///////////////////////////////////////////////////////////
	void setFlippedUv(bool flipped);

	///////////////////////////////////////////////////////////
	/// \brief Set the alpha blend factors
	///
	/// The source factor determines how much of the source color
	/// to keep, and the destination color determines how much of
	/// the destination color to keep.
	///
	/// \param src The blend factor
	///
	///////////////////////////////////////////////////////////
	void setBlendFactors(BlendFactor src, BlendFactor dst);

	///////////////////////////////////////////////////////////
	/// \brief Set the shader that will be used to render the UI element
	///
	/// The element shader is set to the default shader when the
	/// element is created. This function will change the shader
	/// that gets used to render the element.
	///
	/// \param shader A pointer to the shader
	///
	///////////////////////////////////////////////////////////
	void setShader(Shader* shader);

	///////////////////////////////////////////////////////////
	/// \brief Set the visibility of the UI element
	///
	/// If the \a recursive parameter is set to true, which it
	/// is by default, this visibilty setting will be recursively
	/// applied to all of the element's children. Otherwise, only
	/// this element will have its visibilty settings changed.
	///
	/// \param visible Visibilty flag
	/// \param recursive Set this to true to apply to all subchildren
	///
	///////////////////////////////////////////////////////////
	virtual void setVisible(bool visible, bool recursive = true);

	///////////////////////////////////////////////////////////
	/// \brief Set the transparent flag for the element
	///
	/// This should be used if a transparent texture is being used
	/// because it is much harder to check transparency in a texture.
	/// It does not need to be used for transparent colors.
	///
	/// \param transparent Transparency flag
	///
	///////////////////////////////////////////////////////////
	void setTransparent(bool transparent);

	///////////////////////////////////////////////////////////
	/// \brief Move the UI element by the specified pixel offset
	///
	/// \param offset The pixel offset
	///
	///////////////////////////////////////////////////////////
	virtual void move(const Vector2f& offset);

	///////////////////////////////////////////////////////////
	/// \brief Move the UI element by the specified pixel offset
	///
	/// \param x The x-coordinate of the pixel offset
	/// \param y The y-coordinate of the pixel offset
	///
	///////////////////////////////////////////////////////////
	virtual void move(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Rotate the UI element by the specified angle offset
	///
	/// \param angle The angle offset in degrees
	///
	///////////////////////////////////////////////////////////
	virtual void rotate(float angle);

	///////////////////////////////////////////////////////////
	/// \brief Scale the UI element by the specified factor
	///
	/// \param scale The scale factor
	///
	///////////////////////////////////////////////////////////
	virtual void scale(const Vector2f& scale);

	///////////////////////////////////////////////////////////
	/// \brief Scale the UI element by the specified factor
	///
	/// \param x The x-component scale factor
	/// \param y The y-component scale factor
	///
	///////////////////////////////////////////////////////////
	virtual void scale(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Get the element string id
	///
	/// \return The element string id
	///
	///////////////////////////////////////////////////////////
	const std::string& getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the position of the UI element relative to its parent's position
	///
	/// \return The relative position in pixels
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getRelPosition() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the position of the UI element in pixels
	///
	/// \return The absolute position in pixels
	///
	///////////////////////////////////////////////////////////
	virtual const Vector2f& getAbsPosition();

	///////////////////////////////////////////////////////////
	/// \brief Get the rotation of the UI element relative to its parent's rotation
	///
	/// \return The relative rotation in degrees
	///
	///////////////////////////////////////////////////////////
	float getRelRotation() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the rotation of the UI element
	///
	/// \return The absolute rotation in degrees
	///
	///////////////////////////////////////////////////////////
	virtual float getAbsRotation();

	///////////////////////////////////////////////////////////
	/// \brief Get the relative size of the UI element as a percentage of its parent's size
	///
	/// \return The relative size
	///
	///////////////////////////////////////////////////////////
	virtual const Vector2f& getRelSize();

	///////////////////////////////////////////////////////////
	/// \brief Get the pixel size of the UI element
	///
	/// \return The pixel size
	///
	///////////////////////////////////////////////////////////
	virtual const Vector2f& getPixelSize();

	///////////////////////////////////////////////////////////
	/// \brief Get the UI element origin
	///
	/// \return The origin
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getOrigin() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the UI element anchor
	///
	/// \return The anchor
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getAnchor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the UI element background color
	///
	/// \return The background color
	///
	///////////////////////////////////////////////////////////
	const Vector4f& getColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the UI element background texture
	///
	/// \return The background texture
	///
	///////////////////////////////////////////////////////////
	Texture* getTexture() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the texture rectangle
	///
	/// \return The texture rectangle that controls which portion of the texture to display
	///
	///////////////////////////////////////////////////////////
	const Vector4f& getTextureRect() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the texture UV coordinates should be flipped
	///
	/// \return True if the UV coordinates should be flipped
	///
	///////////////////////////////////////////////////////////
	bool hasFlippedUv() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the source color blend factor
	///
	/// \return The source color blend factor
	///
	///////////////////////////////////////////////////////////
	BlendFactor getSrcBlend() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the destination color blend factor
	///
	/// \return The destination color blend factor
	///
	///////////////////////////////////////////////////////////
	BlendFactor getDstBlend() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the shader used to render this element
	///
	/// \return A pointer to the element shader
	///
	///////////////////////////////////////////////////////////
	Shader* getShader() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the element clip region
	///
	/// This region is the rectangle that gets kept when the element
	/// and its children get rendered. This clip region is not
	/// adjustable outside the UIElement class, but can be adjusted
	/// by inheriting from this class.
	///
	/// \return A rectangle clip region
	///
	///////////////////////////////////////////////////////////
	virtual Vector4f getClipRect();

	///////////////////////////////////////////////////////////
	/// \brief Check if the UI element is visible
	///
	/// \return The UI element's visibility
	///
	///////////////////////////////////////////////////////////
	bool isVisible() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the element has transparent colors or textures
	///
	/// \return True if the element has transparent colors or texture
	///
	///////////////////////////////////////////////////////////
	bool isTransparent() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the UI element's parent element
	///
	/// \return The UI element's parent element
	///
	///////////////////////////////////////////////////////////
	UIElement* getParent() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a list of children elements
	///
	/// \return A list of children elements
	///
	///////////////////////////////////////////////////////////
	const std::vector<UIElement*>& getChildren() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the element's z-index
	///
	/// \return The element's z-index
	///
	///////////////////////////////////////////////////////////
	Uint32 getIndex() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the element has mouse hover
	///
	/// \return True if the element is being hovered with the cursor
	///
	///////////////////////////////////////////////////////////
	bool hasHover() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the element has focus
	///
	/// \return True if the element has focus
	///
	///////////////////////////////////////////////////////////
	bool hasFocus() const;

	///////////////////////////////////////////////////////////
	/// \brief Transform absolute coordinate to local coordinate space
	///
	/// \param p The absolute coordinate to transform
	///
	/// \return The point in local coordinate space
	///
	///////////////////////////////////////////////////////////
	Vector2f getLocalCoordinate(const Vector2f& abs);

protected:
	///////////////////////////////////////////////////////////
	/// \brief Key event callback
	///
	/// This function will be called whenever a key event occurs
	/// while the element has hover or focus.
	///
	/// \param e The event object
	///
	///////////////////////////////////////////////////////////
	virtual void onKeyEvent(const E_KeyEvent& e);

	///////////////////////////////////////////////////////////
	/// \brief Mouse button callback
	///
	/// This function will be called whenever a mouse button event
	/// occurs while the element has hover or focus.
	///
	/// \param e The event object
	///
	///////////////////////////////////////////////////////////
	virtual void onMouseButton(const E_MouseButton& e);

	///////////////////////////////////////////////////////////
	/// \brief Mouse move callback
	///
	/// This function will be called whenever a mouse move event
	/// occurs while the element has hover or focus.
	///
	/// \param e The event object
	///
	///////////////////////////////////////////////////////////
	virtual void onMouseMove(const E_MouseMove& e);

	///////////////////////////////////////////////////////////
	/// \brief Mouse scroll callback
	///
	/// This function will be called whenever a mouse scroll event
	/// occurs while the element has hover.
	///
	/// \param e The event object
	///
	///////////////////////////////////////////////////////////
	virtual void onMouseScroll(const E_MouseScroll& e);

	///////////////////////////////////////////////////////////
	/// \brief Text input callback
	///
	/// This function will be called whenever a text inpu event
	/// occurs while the element has focus.
	///
	/// \param e The event object
	///
	///////////////////////////////////////////////////////////
	virtual void onTextInput(const E_TextInput& e);

	///////////////////////////////////////////////////////////
	/// \brief Mouse enter callback
	///
	/// This function will be called whenever the cursor enters
	/// the element bounding box.
	///
	/// \param e The event object
	///
	///////////////////////////////////////////////////////////
	virtual void onMouseEnter(const E_MouseMove& e);

	///////////////////////////////////////////////////////////
	/// \brief Mouse leave callback
	///
	/// This function will be called whenever the cursor leaves
	/// the element bounding box.
	///
	/// \param e The event object
	///
	///////////////////////////////////////////////////////////
	virtual void onMouseLeave(const E_MouseMove& e);

	///////////////////////////////////////////////////////////
	/// \brief Gain focus callback
	///
	/// This function will be called whenever the element gains
	/// focus.
	///
	///////////////////////////////////////////////////////////
	virtual void onGainFocus();

	///////////////////////////////////////////////////////////
	/// \brief Lose focus callback
	///
	/// This function will be called whenever the element loses
	/// focus.
	///
	///////////////////////////////////////////////////////////
	virtual void onLoseFocus();

	///////////////////////////////////////////////////////////
	/// \brief This function should return true if the element handles mouse events
	///
	/// \return True if the element handles mouse events
	///
	///////////////////////////////////////////////////////////
	virtual bool handlesMouseEvents() const;

	///////////////////////////////////////////////////////////
	/// \brief Mark that some property of this element has changed
	///
	///////////////////////////////////////////////////////////
	virtual void markTransformDirty();

	///////////////////////////////////////////////////////////
	/// \brief Update element properties that have been changed
	///
	///////////////////////////////////////////////////////////
	virtual void updateTransforms();

	///////////////////////////////////////////////////////////
	/// \brief Get all quads in the UI element
	///
	///////////////////////////////////////////////////////////
	virtual void getQuads(std::vector<UIQuad>& quads);

protected:
	std::string m_id;						//!< The string id of the element
	UIElement* m_parent;					//!< A pointer to the parent element
	std::vector<UIElement*> m_children;		//!< A list of children elements

	Vector2f m_relPosition;					//!< The position relative to the parent element
	Vector2f m_absPosition;					//!< The absolute position
	float m_relRotation;					//!< The rotation relative to the parent element
	float m_absRotation;					//!< The absolute rotation
	Vector2f m_relSize;						//!< The size of the element as a percentage of the parent element
	Vector2f m_pixelSize;					//!< The size of the element in pixels
	Vector2b m_useRelSize;					//!< A boolean vector indicating if the edge uses relative size or pixel size
	Vector2f m_origin;						//!< The origin of the element as a percentage of the size
	Vector2f m_anchor;						//!< The anchor of the element as a percentage of the parent element size
	Vector4f m_color;						//!< The color of the element
	Texture* m_texture;						//!< A pointer to the element texture
	Vector4f m_textureRect;					//!< The subrectangle of the texture to display
	BlendFactor m_srcBlend;					//!< The source color blend mode
	BlendFactor m_dstBlend;					//!< The destination color blend mode
	Shader* m_shader;						//!< A pointer to the shader that should be used to render the element
	bool m_hasFlippedUv;					//!< True if the V axis is flipped
	bool m_isVisible;						//!< True if this element is visible
	bool m_isColorTransparent;				//!< True if the color contains an alpha value less than one
	bool m_isTextureTransparent;			//!< True if the texture contains a pixel alpha value less than zero

	Uint32 m_index;							//!< The z-index of the element
	bool m_transformChanged;				//!< This is set to true if the elemnt transform changed
	bool m_hasHover;						//!< This is true if the mouse is hovering over the element (assuming it handles mouse events)
	bool m_hasFocus;						//!< This is true if the element has focus

private:
	Shader* getDefaultShader();

	static Shader s_shader;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::UIElement
/// \ingroup UI
///
/// The base UI element class that all the other more complex
/// UI elements inherit from. This class can be used without
/// being inherited, but it will have less functionality.
///
/// View the documentation for each individual function for a
/// more detailed explanation of each.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// UISystem ui;
/// 
/// UIElement element;
/// element.setPosition(50.0f, 20.0f);				// Set the position to (50, 20) pixels
/// element.setRotation(-45.0f);					// Set rotation to -45 degrees
/// element.setSize(100.0f, 30.0f);					// Set size to 100 width and 30 height
/// element.setColor(1.0f, 0.0f, 0.0f, 1.0f);		// Set color to red
///
/// element.setOrigin(0.25f, 0.25f);				// Set the origin to a quarter away from the top-left corner
/// element.setOrigin(UIPosition::Center);			// Set origin to the center, which is equivalent to (0.5, 0.5)
/// element.setAnchor(UIPosition::BotRight);		// Set the anchor to the bottom-right corner of the parent, which is equivalent to (1, 1)
///
/// element.setTexture(0);							// Set this to a non-null value to display a texture on the UI
/// element.setTextureRect(0.5, 0.5f, 0.5f, 0.5f);	// Display only the top-right region of the texture
///
/// // Add to the system
/// ui.addChild(&element);
///
/// \endcode
///
///////////////////////////////////////////////////////////