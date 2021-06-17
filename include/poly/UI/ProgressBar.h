#ifndef POLY_PROGRESS_BAR_H
#define POLY_PROGRESS_BAR_H

#include <poly/UI/UIElement.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A UI element used to display progress of a task
///
///////////////////////////////////////////////////////////
class ProgressBar : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	ProgressBar();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	///////////////////////////////////////////////////////////
	~ProgressBar();

	///////////////////////////////////////////////////////////
	/// \brief Parse properties for this progress bar from an XML node
	///
	/// \copydoc UIElement::parse
	///
	///////////////////////////////////////////////////////////
	void parse(XmlNode node, const UITemplateMap& templates = UITemplateMap()) override;

	///////////////////////////////////////////////////////////
	/// \brief Set the padding to control the spacing around the filled area
	///
	/// \param padding The padding of the progress bar in pixels
	///
	///////////////////////////////////////////////////////////
	void setPadding(const Vector2f& padding);

	///////////////////////////////////////////////////////////
	/// \brief Set the padding to control the spacing around the filled area
	///
	/// \param x The x-component of the padding of the progress bar in pixels
	/// \param y The y-component of the padding of the progress bar in pixels
	///
	///////////////////////////////////////////////////////////
	void setPadding(float x, float y);

	///////////////////////////////////////////////////////////
	/// \brief Set the color of the filled area of the progress bar
	///
	/// \param color The color of the filled area
	///
	///////////////////////////////////////////////////////////
	void setFillColor(const Vector4f& color);

	///////////////////////////////////////////////////////////
	/// \brief Set the color of the filled area of the progress bar
	///
	/// \param r The r-component of the filled area
	/// \param g The g-component of the filled area
	/// \param b The b-component of the filled area
	/// \param a The a-component of the filled area
	///
	///////////////////////////////////////////////////////////
	void setFillColor(float r, float g, float b, float a);

	///////////////////////////////////////////////////////////
	/// \brief Set the texture to display in the filled area
	///
	/// \copydoc UIElement::setTexture
	///
	///////////////////////////////////////////////////////////
	void setFillTexture(Texture* texture);

	///////////////////////////////////////////////////////////
	/// \brief Set the progress value as a percentage from 0 to 1
	///
	/// Using this function will cause the size of the filled area
	/// to change. If the new value is outside the range 0 to 1, it
	/// will be clamped to this range.
	///
	/// \param value The value to set the progress bar at, between 0 and 1
	///
	///////////////////////////////////////////////////////////
	void setValue(float value);

	///////////////////////////////////////////////////////////
	/// \brief Get the padding value of the progress bar
	///
	/// \return The padding value
	///
	///////////////////////////////////////////////////////////
	const Vector2f& getPadding() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the color of the filled area
	///
	/// \return The color of the filled area
	///
	///////////////////////////////////////////////////////////
	const Vector4f& getFillColor() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the texture of the filled area
	///
	/// \return A pointer to the texture of the filled area
	///
	///////////////////////////////////////////////////////////
	Texture* getFillTexture() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the value of the progress bar, as a percentage between 0 and 1
	///
	/// \return The value of the progress bar
	///
	///////////////////////////////////////////////////////////
	float getValue() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the fill box element used to indicate progress
	///
	/// \return A pointer to the fill box UI element
	///
	///////////////////////////////////////////////////////////
	UIElement* getFillBox() const;

private:
	void updateFillBox();

private:
	UIElement* m_fillBox;			//!< A pointer to the fill box element
	Vector2f m_padding;				//!< The padding of the progress bar
	float m_value;					//!< The progress bar value
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::ProgressBar
/// \ingroup UI
///
/// A progress bar can be used to indicate progress towards the
/// completion of a task, or to display a visualization of a
/// percentage. The progress bar always fill up from the right
/// side towards the left side.
///
/// To use a progress bar, set the size and position (and other
/// standard UI element properties), then set the color of the
/// fill area, and the padding of the fill area. Add it to a UI
/// element tree that is connected to a UISystem so that the element
/// can be rendered.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a UI system
/// UIsystem ui;
///
/// ProgressBar progress;
/// progress.setPosition(5, 5);
/// progress.setSize(100.0f, 15.0f);
/// progress.setFillColor(0.5f, 0.5f, 1.0f, 1.0f);
/// progress.setPadding(3.0f, 3.0f);
/// ui.addChild(&progress);
///
/// \endcode
///
///////////////////////////////////////////////////////////