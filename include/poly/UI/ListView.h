#ifndef POLY_LIST_VIEW

#include <poly/UI/UIElement.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief A vertical list view
///
///////////////////////////////////////////////////////////
class VListView : public UIElement
{
public:
	void addChild(UIElement* child) override;

	void addChild(UIElement* child, const Vector2f& margins);

	void removeChild(UIElement* child);

private:
	void getQuads(std::vector<UIQuad>& quads) override;

private:
	std::vector<Vector2f> m_margins;
};


///////////////////////////////////////////////////////////
/// \brief A horizontal list view
///
///////////////////////////////////////////////////////////
class HListView : public UIElement
{
public:
	void addChild(UIElement* child) override;

	void addChild(UIElement* child, const Vector2f& margins);

	void removeChild(UIElement* child);

private:
	void getQuads(std::vector<UIQuad>& quads) override;

private:
	std::vector<Vector2f> m_margins;
};


///////////////////////////////////////////////////////////
/// \brief A vertical list view
///
///////////////////////////////////////////////////////////
typedef VListView ListView;

}

#endif