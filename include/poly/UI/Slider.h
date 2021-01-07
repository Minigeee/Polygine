#ifndef POLY_SLIDER_H
#define POLY_SLIDER_H

#include <poly/UI/UIElement.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief The UI element that is dragged in a slider element
///
///////////////////////////////////////////////////////////
class SliderButton : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Sets the default origin and anchor
	///
	///////////////////////////////////////////////////////////
	SliderButton();

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the cursor enters the element area
	///
	/// \param func The function callback
	///
	///////////////////////////////////////////////////////////
	void onMouseEnter(const std::function<void(const E_MouseMove&)>& func);

	///////////////////////////////////////////////////////////
	/// \brief Set the function callback that gets called when the cursor leaves the element area
	///
	/// \param func The function callback
	///
	///////////////////////////////////////////////////////////
	void onMouseLeave(const std::function<void(const E_MouseMove&)>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseMove(const E_MouseMove& e) override;

	void onMouseEnter(const E_MouseMove& e) override;

	void onMouseLeave(const E_MouseMove& e) override;

	bool handlesMouseEvents() const override;

private:
	Vector2f m_offset;		//!< The mouse offset
	bool m_isPressed;		//!< True whenever the left mouse button is pressed in the element

	std::function<void(const E_MouseMove&)> m_onMouseEnter;
	std::function<void(const E_MouseMove&)> m_onMouseLeave;
};


///////////////////////////////////////////////////////////
/// \brief A slider element
///
///////////////////////////////////////////////////////////
class Slider : public UIElement
{
	friend SliderButton;

public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	/// Creates a slider button child
	///
	///////////////////////////////////////////////////////////
	Slider();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Frees the slider button child
	///
	///////////////////////////////////////////////////////////
	~Slider();

	///////////////////////////////////////////////////////////
	/// \brief Set the slider value
	///
	/// The value should be a decimal value between 0 and 1, where
	/// at a value of 0, the slider button would be positioned on
	/// the left side of the element.
	///
	/// \param value The value to assign 
	///
	///////////////////////////////////////////////////////////
	void setValue(float value);

	///////////////////////////////////////////////////////////
	/// \brief Get the slider's value
	///
	/// \return The slider's value
	///
	///////////////////////////////////////////////////////////
	float getValue() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the slider button
	///
	/// \return The slider button
	///
	///////////////////////////////////////////////////////////
	SliderButton* getSliderButton();

	///////////////////////////////////////////////////////////
	/// \brief Set the function that gets called when the slider value changes
	///
	/// \param func The function callback
	///
	///////////////////////////////////////////////////////////
	void onValueChange(const std::function<void(float)>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseMove(const E_MouseMove& e) override;

	bool handlesMouseEvents() const override;

private:
	SliderButton* m_button;		//!< The slider button element
	float m_value;				//!< The slider value
	bool m_isPressed;			//!< True if the slider is pressed

	std::function<void(float)> m_onValueChange;
};

}

#endif