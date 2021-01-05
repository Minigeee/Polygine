#ifndef POLY_SLIDER_H
#define POLY_SLIDER_H

#include <poly/UI/UIElement.h>

namespace poly
{

class SliderButton : public UIElement
{
public:
	SliderButton();

	void onMouseEnter(const std::function<void(const E_MouseMove&)>& func);

	void onMouseLeave(const std::function<void(const E_MouseMove&)>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseMove(const E_MouseMove& e) override;

	void onMouseEnter(const E_MouseMove& e) override;

	void onMouseLeave(const E_MouseMove& e) override;

	bool handlesMouseEvents() const override;

private:
	Vector2f m_offset;
	bool m_isPressed;

	std::function<void(const E_MouseMove&)> m_onMouseEnter;
	std::function<void(const E_MouseMove&)> m_onMouseLeave;
};

class Slider : public UIElement
{
	friend SliderButton;

public:
	Slider();

	~Slider();

	void setValue(float value);

	float getValue() const;

	SliderButton* getSliderButton();

	void onValueChanged(const std::function<void(float)>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseMove(const E_MouseMove& e) override;

	bool handlesMouseEvents() const override;

private:
	SliderButton* m_button;
	Vector2f m_mousePos;
	float m_value;
	bool m_isPressed;

	std::function<void(float)> m_onValueChanged;
};

}

#endif