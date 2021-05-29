#include "UITemplates.h"


///////////////////////////////////////////////////////////
void onMouseEnterBtn(Button* btn, const E_MouseMove& e)
{
	if (!btn->isPressed())
		btn->setColor(0.25f, 0.25f, 0.30f, 1.0f);
}


///////////////////////////////////////////////////////////
void onMouseLeaveBtn(Button* btn, const E_MouseMove& e)
{
	if (!btn->isPressed())
		btn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
}



///////////////////////////////////////////////////////////
void onBtnPress(Button* btn)
{
	btn->setColor(0.2f, 0.2f, 0.25f, 1.0f);
}