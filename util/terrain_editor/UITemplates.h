#ifndef TERRAIN_EDITOR_UI_TEMPLATES_H
#define TERRAIN_EDITOR_UI_TEMPLATES_H

#include <poly/UI/Button.h>

using namespace poly;


void onMouseEnterBtn(Button* btn, const E_MouseMove& e);

void onMouseLeaveBtn(Button* btn, const E_MouseMove& e);

void onBtnPress(Button* btn);

#endif