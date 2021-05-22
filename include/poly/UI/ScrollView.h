#ifndef POLY_SCROLL_VIEW_H
#define POLY_SCROLL_VIEW_H

#include <poly/UI/UIElement.h>

namespace poly
{

class Slider;

///////////////////////////////////////////////////////////
/// \brief A UI element specialized for the scroll view
///
///////////////////////////////////////////////////////////
class ScrollBody : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	ScrollBody();

	///////////////////////////////////////////////////////////
	/// \brief Set the clip margins
	///
	/// \param margin The clip margin in pixels
	///
	///////////////////////////////////////////////////////////
	void setClipMargin(float margin);

	///////////////////////////////////////////////////////////
	/// \brief Get the clip margins
	///
	/// \return The clip margin in pixels
	///
	///////////////////////////////////////////////////////////
	float getClipMargin() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the clipping rectangle
	///
	/// \return The clipping rectangle
	///
	///////////////////////////////////////////////////////////
	Vector4f getClipRect() override;

private:
	float m_clipMargin;		//!< The clipping margin in pixels
};


///////////////////////////////////////////////////////////
/// \brief A vertical scroll box
///
///////////////////////////////////////////////////////////
class ScrollView : public UIElement
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates the scroll body and the scroll bar
	///
	///////////////////////////////////////////////////////////
	ScrollView();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Freest the scroll body and scroll bar
	///
	///////////////////////////////////////////////////////////
	~ScrollView();

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	virtual void parse(XmlNode node) override;

	///////////////////////////////////////////////////////////
	/// \brief Add a child to the scroll body of the scroll view element
	///
	/// All elements that are added to the scroll view will be scrollable
	/// if its contents are out of bounds of the scroll view.
	///
	///////////////////////////////////////////////////////////
	void addChild(UIElement* child) override;

	///////////////////////////////////////////////////////////
	/// \brief Remove a child UI element
	///
	/// \param child The child element to remove
	///
	///////////////////////////////////////////////////////////
	void removeChild(UIElement* child) override;

	///////////////////////////////////////////////////////////
	/// \brief Recalculate the scroll view bounds
	///
	/// This function recalculates the scroll view's bounds based
	/// on the axis-aligned bounding boxes of the children elements.
	///
	///////////////////////////////////////////////////////////
	void updateViewBounds();

	///////////////////////////////////////////////////////////
	/// \brief Set the margin of the clipping region
	///
	/// The clipping margin is the spacing between the border of
	/// the scroll view and the clipping area. By default, this value
	/// is 5 pixels.
	///
	/// \param margin The margin of the clipping area in pixels
	///
	///////////////////////////////////////////////////////////
	void setClipMargin(float margin);

	///////////////////////////////////////////////////////////
	/// \brief Set the offset of the scroll view content
	///
	/// The scroll offset is equivalent to the negative y-coordinate
	/// of the view content
	///
	/// \param offset The scroll offset in pixels
	///
	///////////////////////////////////////////////////////////
	void setScrollOffset(float offset);

	///////////////////////////////////////////////////////////
	/// \brief Set the scrolling speed when using the mouse scrolling wheel
	///
	/// The scroll speed value defines how many pixels to move the
	/// view content per scroll wheel tick. By default, this value
	/// is 50.
	///
	/// \param speed The scroll wheel speed
	///
	///////////////////////////////////////////////////////////
	void setScrollSpeed(float speed);

	///////////////////////////////////////////////////////////
	/// \brief Set the width of the vertical scroll bar
	///
	/// \param width The width of the scroll bar in pixels
	///
	///////////////////////////////////////////////////////////
	void setScrollBarWidth(float width);

	///////////////////////////////////////////////////////////
	/// \brief Get the margins of the clipping region
	///
	/// \return The margins of the clipping region in pixels
	///
	///////////////////////////////////////////////////////////
	float getClipMargin() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the offset of the scroll view content
	///
	/// \return The offset of the view content in pixels
	///
	///////////////////////////////////////////////////////////
	float getScrollOffset() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the speed of scrolling the view when the mouse wheel is used
	///
	/// \return The scrolling speed
	///
	///////////////////////////////////////////////////////////
	float getScrollSpeed() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the width of the vertical scroll bar
	///
	/// \return The width of the vertical scroll bar
	///
	///////////////////////////////////////////////////////////
	float getScrollBarWidth() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to the vertical scroll bar
	///
	/// \return A pointer to the slider scroll bar
	///
	///////////////////////////////////////////////////////////
	Slider* getScrollBar();

private:
	void onMouseScroll(const E_MouseScroll& e) override;

	void onScrollBarChange(float value);

	void updateTransforms() override;

	bool handlesMouseEvents() const override;

private:
	ScrollBody* m_scrollBody;	//!< The scroll body
	Slider* m_scrollBar;		//!< The scroll bar
	float m_scrollSpeed;		//!< The number of pixels to move per mouse wheel tick
	float m_minY;				//!< The minimum y-value
	float m_maxY;				//!< The maximum y-value
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::ScrollView
/// \ingroup UI
///
/// This class is a vertical scroll view that creates a scrollable
/// view when its children bounds are bigger than its own size.
/// This scroll view only supports vertical scrolling.
///
/// To use a scoll view, initialize the scroll view like any
/// other UI element, then add UI elements to the scroll view.
/// All children element will be scrollable.
///
/// There are also several adjustable parameters specific to
/// the scroll view, such as scrolling speed, the clipping area
/// margins, the scroll bar width, etc.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// ScrollView scroll;
/// scroll.setPosition(30.0f, 30.0f);
/// scroll.setSize(100.0f, 300.0f);
/// scroll.setColor(0.2f, 0.2f, 0.25f, 1.0f);
///
/// // Set scroll bar width
/// scroll.setScrollBarWidth(5.0f);
///
/// // Set scrolling speed
/// scroll.setScrollSpeed(45.0f);
///
/// // Set clipping margins
/// scroll.setClipMargins(5.0f);
///
/// // Then add other elements to the scroll view...
///
/// \endcode
///
///////////////////////////////////////////////////////////