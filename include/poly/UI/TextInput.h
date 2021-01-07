#ifndef POLY_TEXT_INPUT_H
#define POLY_TEXT_INPUT_H

#include <poly/UI/UIElement.h>

namespace poly
{

class Text;


///////////////////////////////////////////////////////////
/// \brief A text input UI element
///
///////////////////////////////////////////////////////////
class TextInput : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	TextInput();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~TextInput();

	///////////////////////////////////////////////////////////
	/// \brief Update any UI animations and update text cursor
	///
	///////////////////////////////////////////////////////////
	void update(float dt) override;

	///////////////////////////////////////////////////////////
	/// \brief Submit the value of the text input
	///
	/// This functions simply calls the submit function callback.
	/// The callback function can be set with onSubmit().
	///
	///////////////////////////////////////////////////////////
	void submit();

	///////////////////////////////////////////////////////////
	/// \brief Set the value of the text input
	///
	/// \param value The string value to give the text input
	///
	///////////////////////////////////////////////////////////
	void setValue(const std::string& value);

	///////////////////////////////////////////////////////////
	/// \brief Set the designated submit key
	///
	/// Whenever the submit key is pressed on the keyboard,
	/// the text input will be submit and its function callback
	/// will be called. The submit callback can be set with
	/// onSubmit(). By default, the submit key is Keyboard::Enter.
	///
	/// \param key The key to designate as the submit key
	///
	///////////////////////////////////////////////////////////
	void setSubmitKey(Keyboard key);

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the text cursor
	///
	/// By default, the text cursor size is (1, 15).
	///
	/// \param size The text cursor size in pixels
	///
	///////////////////////////////////////////////////////////
	void setTextCursorSize(const Vector2f& size);

	///////////////////////////////////////////////////////////
	/// \brief Set the size of the text cursor
	///
	/// By default, the text cursor size is (1, 15).
	///
	/// \param w The width of the text cursor in pixels
	/// \param h The height of the text cursor in pixels
	///
	///////////////////////////////////////////////////////////
	void setTextCursorSize(float w, float h);

	///////////////////////////////////////////////////////////
	/// \brief Set the text cursor color
	///
	/// By default, the text cursor is white.
	///
	/// \param color The color of the text cursor
	///
	///////////////////////////////////////////////////////////
	void setTextCursorColor(const Vector4f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the text cursor color
	///
	/// By default, the text cursor is white.
	///
	/// \param r The r-component of the color
	/// \param g The g-component of the color
	/// \param b The b-component of the color
	/// \param a The a-component of the color
	///
	///////////////////////////////////////////////////////////
	void setTextCursorColor(float r, float g, float b, float a);

	///////////////////////////////////////////////////////////
	/// \brief Set the position of the cursor as an index in the input string
	///
	/// The position of the text cursor is equal to its index
	/// in the input string.
	///
	/// \param pos The index position of the text cursor
	///
	///////////////////////////////////////////////////////////
	void setTextCursorPosition(Uint32 pos);

	///////////////////////////////////////////////////////////
	/// \brief Set the time it takes the text cursor to make one full blinking cycle
	///
	/// By default, this value is 1.2 seconds.
	///
	/// \param sec The time in seconds to make a full text cursor blinking cycle
	///
	///////////////////////////////////////////////////////////
	void setTextCursorCycle(float sec);

	///////////////////////////////////////////////////////////
	/// \brief Set the color of the text selection highlight
	///
	/// The text highlight is rendered behind the text in the input
	/// box, so transparency is not needed.
	///
	/// \param color The text highlight color
	///
	///////////////////////////////////////////////////////////
	void setHighlightColor(const Vector4f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the color of the text selection highlight
	///
	/// The text highlight is rendered behind the text in the input
	/// box, so transparency is not needed.
	///
	/// \param color The text highlight color
	///
	///////////////////////////////////////////////////////////
	void setHighlightColor(float r, float g, float b, float a);

	///////////////////////////////////////////////////////////
	/// \brief Set the range of character that are selected
	///
	/// This will cause the highlight box to cover the area that
	/// is being selected.
	///
	/// \param start The index to start the text selection (inclusive)
	/// \param end The index to end the text selection (exclusive)
	///
	///////////////////////////////////////////////////////////
	void setTextSelection(Uint32 start, Uint32 end);

	///////////////////////////////////////////////////////////
	/// \brief Set the alignment of the text
	///
	/// \param align A predefined text alignment
	///
	///////////////////////////////////////////////////////////
	void setTextAlign(UIPosition align);

	///////////////////////////////////////////////////////////
	/// \brief Set the offset of the text in pixels
	///
	/// \param offset The offset of the text in pixels
	///
	///////////////////////////////////////////////////////////
	void setTextOffset(const Vector2f& offset);

	///////////////////////////////////////////////////////////
	/// \brief Set the offset of the text in pixels
	///
	/// \param x The x-coordinate of the text offset
	/// \param y The y-coordinate of the text offset
	///
	///////////////////////////////////////////////////////////
	void setTextOffset(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Get the string value of the text input
	///
	/// \return The text value of the input
	///
	///////////////////////////////////////////////////////////
	const std::string& getValue() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the key designated as the submit key
	///
	/// \return The key designated as the submit key
	///
	///////////////////////////////////////////////////////////
	Keyboard getSubmitKey() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the size of the text cursor
	///
	/// \return The text cursor size in pixels
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getTextCursorSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the color of the text cursor
	///
	/// \return The color of the text cursor
	///
	///////////////////////////////////////////////////////////
	const Vector4f& getTextCursorColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the index position of the text cursor
	///
	/// \return The index position of the text cursor
	///
	///////////////////////////////////////////////////////////
	Uint32 getTextCursorPosition() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the time in seconds for the text cursor to make a full blinking cycle
	///
	/// \return The time in seconds for the text cursor to make a full blinking cycle
	///
	///////////////////////////////////////////////////////////
	float getTextCursorCycle() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the color of the text highlight
	///
	/// \return The color of the text highlight
	///
	///////////////////////////////////////////////////////////
	const Vector4f& getHighlightColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the selected text
	///
	/// A 2D vector is returned, where the x-coordinate is the start
	///  of the range, and the y-coordinate is end of the range.
	///
	/// \return The text selection
	///
	///////////////////////////////////////////////////////////
	const Vector2u& getTextSelection() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the alignment of the text element
	///
	/// \return The text alignment
	///
	///////////////////////////////////////////////////////////
	UIPosition getTextAlign() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the offset of the text element in pixels
	///
	/// \return The offset of the text element in pixels
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getTextOffset() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to the internal text element
	///
	/// \return A pointer to the text element
	///
	///////////////////////////////////////////////////////////
	Text* getText();

	///////////////////////////////////////////////////////////
	/// \brief Set the callback function that gets called when the input value changes
	///
	/// \param func The callback function
	///
	///////////////////////////////////////////////////////////
	void onValueChange(const std::function<void(const std::string&)>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the callback function that gets called when the element gains focus
	///
	/// \param func The callback function
	///
	///////////////////////////////////////////////////////////
	void onGainFocus(const std::function<void()>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the callback function that gets called when the element loses focus
	///
	/// \param func The callback function
	///
	///////////////////////////////////////////////////////////
	void onLoseFocus(const std::function<void()>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the callback function that gets called when the input is submitted
	///
	/// \param func The callback function
	///
	///////////////////////////////////////////////////////////
	void onSubmit(const std::function<void(const std::string&)>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseMove(const E_MouseMove& e) override;

	void onMouseEnter(const E_MouseMove& e) override;

	void onMouseLeave(const E_MouseMove& e) override;

	void onKeyEvent(const E_KeyEvent& e) override;

	void onTextInput(const E_TextInput& e) override;

	void onGainFocus() override;

	void onLoseFocus() override;

	bool handlesMouseEvents() const override;

private:
	Text* m_text;				//!< The text element
	UIElement* m_textCursor;	//!< The text cursor
	UIElement* m_highlight;		//!< The highlight box
	float m_cursorCycle;		//!< The text cursor cycle
	float m_time;				//!< The current time into the cursor cycle

	Keyboard m_submitKey;		//!< The submit key
	Uint32 m_cursorCharPos;		//!< The cursor character position
	Vector2u m_textSelection;	//!< The range of text selection
	UIPosition m_textAlign;		//!< The alignment of the text

	Uint32 m_selectStart;		//!< The start position of the text selection (mostly used for UI purposes)
	bool m_isPressed;			//!< This is true if the element is pressed

	std::function<void(const std::string&)> m_onValueChanged;
	std::function<void()> m_onGainFocus;
	std::function<void()> m_onLoseFocus;
	std::function<void(const std::string&)> m_onSubmit;
};

}

#endif