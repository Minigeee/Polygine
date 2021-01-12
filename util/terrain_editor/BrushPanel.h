#ifndef TERRAIN_EDITOR_BRUSH_PANEL_H
#define TERRAIN_EDITOR_BRUSH_PANEL_H

#include <poly/UI/Dropdown.h>
#include <poly/UI/ListView.h>
#include <poly/UI/Slider.h>
#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>

using namespace poly;

class BrushPanel : public ListView
{
public:
	BrushPanel();

	~BrushPanel();

	void setMode(Uint32 mode);

	void setRadius(float radius);

	void setStrength(float strength);

	void setGradient(float gradient);

	Uint32 getMode() const;

	float getRadius() const;

	float getStrength() const;

	float getGradient() const;

private:
	Text* m_panelTitle;
	Text* m_modeTitle;
	Text* m_functionTitle;
	Text* m_radiusTitle;
	Text* m_strengthTitle;
	Text* m_gradientTitle;

	Dropdown* m_modeMenu;
	Dropdown* m_functionMenu;
	Slider* m_radiusSlider;
	Slider* m_strengthSlider;
	Slider* m_gradientSlider;
	TextInput* m_radiusInput;
	TextInput* m_strengthInput;
	TextInput* m_gradientInput;
	HListView* m_radiusRow;
	HListView* m_strengthRow;
	HListView* m_gradientRow;

	float m_radius[3];
	float m_strength[3];
	float m_gradient[3];
};

#endif