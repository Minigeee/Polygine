#ifndef TERRAIN_EDITOR_COLOR_SELECTOR_H
#define TERRAIN_EDITOR_COLOR_SELECTOR_H

#include <poly/UI/Button.h>
#include <poly/UI/Slider.h>
#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>

using namespace poly;


///////////////////////////////////////////////////////////
class ColorSelector : public UIElement
{
public:
	ColorSelector();

	~ColorSelector();

	void setSelectedColor(float r, float g, float b);

	const Vector3f& getSelectedColor() const;

	void onConfirm(const std::function<void()>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseMove(const E_MouseMove& e) override;

	bool handlesMouseEvents() const override;

	void onConfirmCancelBtn(Button* btn);

private:
	Text* m_panelTitle;
	Text* m_rTitle;
	Text* m_gTitle;
	Text* m_bTitle;
	Text* m_hexTitle;
	Text* m_decTitle;

	Slider* m_rSlider;
	Slider* m_gSlider;
	Slider* m_bSlider;
	TextInput* m_rInput;
	TextInput* m_gInput;
	TextInput* m_bInput;
	TextInput* m_hexInput;
	TextInput* m_decInput;

	UIElement* m_sampleBox;
	Button* m_confirmBtn;
	Button* m_cancelBtn;

	std::function<void()> m_onConfirm;
	Vector3f m_selectedColor;
	Vector2f m_mouseOffset;
	bool m_isPressed;
};



#endif