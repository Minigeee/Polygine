#ifndef POLY_BUTTON_H
#define POLY_BUTTON_H

#include <poly/UI/UIElement.h>

namespace poly
{

class Text;

///////////////////////////////////////////////////////////
/// \brief A button UI element
///
///////////////////////////////////////////////////////////
class Button : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates text element and centers it
	///
	///////////////////////////////////////////////////////////
	Button();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Frees text element
	///
	///////////////////////////////////////////////////////////
	~Button();

	///////////////////////////////////////////////////////////
	/// \brief Set the text string
	///
	/// \param string The string to display on the button
	///
	///////////////////////////////////////////////////////////
	void setString(const std::string& string);

	///////////////////////////////////////////////////////////
	/// \brief Set the text alignment
	///
	/// \param align The position to align the text. By default
	/// this is UIPosition::Center for vertically and horizontally
	/// centered text.
	///
	/// \param align The text alignment position
	///
	///////////////////////////////////////////////////////////
	void setTextAlign(UIPosition align);

	///////////////////////////////////////////////////////////
	/// \brief Set the text offset
	///
	/// \param offset The text offset
	///
	///////////////////////////////////////////////////////////
	void setTextOffset(const Vector2f& offset);

	///////////////////////////////////////////////////////////
	/// \brief Set the text offset
	///
	/// \param x The x-coordinate of the text offset
	/// \param y The y-coordinate of the text offset
	///
	///////////////////////////////////////////////////////////
	void setTextOffset(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Get the text string
	///
	/// \return Te text string
	///
	///////////////////////////////////////////////////////////
	const std::string& getString();

	///////////////////////////////////////////////////////////
	/// \brief Get the text alignment
	///
	/// \return The text alignment
	///
	///////////////////////////////////////////////////////////
	UIPosition getTextAlign() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the text offset
	///
	/// \return The text offset
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getTextOffset();

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to the button text element
	///
	/// \return A pointer to the text element
	///
	///////////////////////////////////////////////////////////
	Text* getText();
	
	///////////////////////////////////////////////////////////
	/// \brief Check if the button is being pressed with left-click
	///
	/// \return True if the button is being pressed with left-click
	///
	///////////////////////////////////////////////////////////
	bool isPressed() const;

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the mouse cursor enters the button
	///
	/// This function gets called even if the left mouse button
	/// is already pressed.
	///
	/// \param func The function callback
	///
	///////////////////////////////////////////////////////////
	void onMouseEnter(const std::function<void(const E_MouseMove&)>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the mouse cursor leaves the button
	///
	/// This function gets called even if the left mouse button
	/// is already pressed.
	///
	/// \param func The function callback
	///
	///////////////////////////////////////////////////////////
	void onMouseLeave(const std::function<void(const E_MouseMove&)>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the button gets pressed with left-click
	///
	/// This function gets called only if the left mouse button is
	/// pressed while the cursor is inside the button area.
	///
	/// \param func The function callback
	///
	///////////////////////////////////////////////////////////
	void onPress(const std::function<void()>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the left mouse button is released inside the button
	///
	/// This function gets called only if the left mouse button was pressed
	/// while the cursor is inside the button area. The cursor must be inside
	/// the button when it is released for the function to be called.
	///
	/// \param func The function callback
	///
	///////////////////////////////////////////////////////////
	void onRelease(const std::function<void()>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the left mouse button is released outside the button
	///
	/// This function gets called only if the left mouse button was pressed
	/// while the cursor is inside the button area. The cursor must be inside
	/// the button when it is released for the function to be called.
	///
	/// \param func The function callback
	///
	///////////////////////////////////////////////////////////
	void onCancel(const std::function<void()>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseEnter(const E_MouseMove& e) override;

	void onMouseLeave(const E_MouseMove& e) override;

	bool handlesMouseEvents() const override;

	void createText();

private:
	Text* m_text;			//!< The button label
	UIPosition m_textAlign;	//!< The text alignment
	bool m_isPressed;		//!< True if the button is being pressed

	std::function<void(const E_MouseMove&)> m_onMouseEnter;
	std::function<void(const E_MouseMove&)> m_onMouseLeave;
	std::function<void()> m_onPress;
	std::function<void()> m_onRelease;
	std::function<void()> m_onCancel;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Button
/// \ingroup UI
///
/// A standard button with an optional text label. If none
/// of the text parameters are changed, then the button will
/// not have a text element. Fi the button has a text element,
/// the text will be center aligned by default.
///
/// To set button callbacks, use onMouseEnter(), onMouseLeave(),
/// onPress(), and onRelease(). A button press only counts if
/// the press started while the cursor was inside the button.
/// The release event will occur no matter where the cursor
/// is when the mouse button is released. The button only
/// handles left-button events.
///
/// Usage eample:
/// \code
///
/// using namespace poly;
///
/// Button btn;
/// btn.setPosition(30.0f, 30.0f);
/// btn.setSize(100.0f, 30.0f);
/// btn.setColor(0.2f, 0.2f, 0.25f, 1.0f);
/// btn.setString("Play");
///
/// // The text color is white by default
/// // And centered by default
///
/// // Set function callbacks
/// btn.onMouseEnter(
///		[&](const E_MouseMove& e)
///		{
///			btn.setColor(0.3f, 0.3f, 0.35f, 1.0f);
///		}
/// );
///
/// btn.onMouseLeave(
///		[&](const E_MouseMove& e)
///		{
///			btn.setColor(0.2f, 0.2f, 0.25f, 1.0f);
///		}
/// );
///
/// btn.onRelease(
///		[&](const E_MouseMove& e)
///		{
///			// Make sure the button still has hover in order to count as a valid press
///			if (btn.hasHover())
///				std::cout << "Pressed\n";
///		}
/// );
///
///
/// // For more general callback functions
///
/// // Make a function that accepts a button
/// void onMouseEnter(Button* button, const E_MouseMove& e)
/// {
///		button->setColor(0.3f, 0.3f, 0.35f, 1.0f);
/// }
///
/// // Set the callback using bind, where the 2nd parameter corresponds to the 1st parameter in the function, etc.
/// btn.onMouseEnter(std::bind(onMouseEnter, &btn, std::placeholders::_1));
///
/// \endcode
///
///////////////////////////////////////////////////////////