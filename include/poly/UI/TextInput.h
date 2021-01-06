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

	void setClipMargins(float margins);

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
};

}

#endif