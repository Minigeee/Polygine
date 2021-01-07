#ifndef POLY_TEXT_INPUT_H
#define POLY_TEXT_INPUT_H

#include <poly/UI/UIElement.h>

namespace poly
{

class Text;

class TextInput : public UIElement
{
public:
	TextInput();

	~TextInput();

	void update(float dt) override;

	void submit();

	void setValue(const std::string& value);

	void setSubmitKey(Keyboard key);

	void setTextCursorSize(const Vector2f& size);

	void setTextCursorSize(float w, float h);

	void setTextCursorColor(const Vector4f& color);

	void setTextCursorColor(float r, float g, float b, float a);

	void setTextCursorPosition(Uint32 pos);

	void setTextCursorCycle(float sec);

	void setHighlightColor(const Vector4f& color);

	void setHighlightColor(float r, float g, float b, float a);

	void setTextSelection(Uint32 start, Uint32 end);

	void setTextAlign(UIPosition align);

	void setTextOffset(const Vector2f& offset);

	void setTextOffset(float x, float y);

	void setClipMargins(float margins);

	const std::string& getValue() const;

	Keyboard getSubmitKey() const;

	const Vector2f& getTextCursorSize() const;

	const Vector4f& getTextCursorColor() const;

	Uint32 getTextCursorPosition() const;

	float getTextCursorCycle() const;

	const Vector4f& getHighlightColor() const;

	const Vector2u& getTextSelection() const;

	UIPosition getTextAlign() const;

	const Vector2f& getTextOffset() const;

	Text* getText();

	void onValueChange(const std::function<void(const std::string&)>& func);

	void onGainFocus(const std::function<void()>& func);

	void onLoseFocus(const std::function<void()>& func);

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
	Text* m_text;
	UIElement* m_textCursor;
	UIElement* m_highlight;
	float m_cursorCycle;
	float m_time;

	Keyboard m_submitKey;
	Uint32 m_cursorCharPos;
	Vector2u m_textSelection;
	UIPosition m_textAlign;

	Uint32 m_selectStart;
	bool m_isPressed;

	std::function<void(const std::string&)> m_onValueChanged;
	std::function<void()> m_onGainFocus;
	std::function<void()> m_onLoseFocus;
	std::function<void(const std::string&)> m_onSubmit;
};

}

#endif