#ifndef POLY_BUTTON_H
#define POLY_BUTTON_H

#include <poly/UI/UIElement.h>

namespace poly
{

class Text;

class Button : public UIElement
{
public:
	Button();

	~Button();

	void setString(const std::string& string);

	void setTextAlign(UIPosition align);

	void setTextOffset(const Vector2f& offset);

	void setTextOffset(float x, float y);

	const std::string& getString() const;

	UIPosition getTextAlign() const;

	const Vector2f& getTextOffset() const;

	Text* getText() const;
	
	bool isPressed() const;

	void onMouseEnter(const std::function<void(const E_MouseMove&)>& func);

	void onMouseLeave(const std::function<void(const E_MouseMove&)>& func);

	void onPress(const std::function<void()>& func);

	void onRelease(const std::function<void()>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseEnter(const E_MouseMove& e) override;

	void onMouseLeave(const E_MouseMove& e) override;

	bool handlesMouseEvents() const override;

private:
	Text* m_text;
	UIPosition m_textAlign;
	bool m_isPressed;

	std::function<void(const E_MouseMove&)> m_onMouseEnter;
	std::function<void(const E_MouseMove&)> m_onMouseLeave;
	std::function<void()> m_onPress;
	std::function<void()> m_onRelease;
};

}

#endif