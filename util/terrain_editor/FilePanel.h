#ifndef TERRAIN_EDITOR_FILE_PANEL_H
#define TERRAIN_EDITOR_FILE_PANEL_H

#include <poly/UI/Button.h>
#include <poly/UI/Text.h>

using namespace poly;

class FilePanel : public UIElement
{
public:
	FilePanel();

private:
	void onBtnRelease(Button* btn);

private:
	Text* m_panelTitle;
	Button* m_importHeightBtn;
	Button* m_exportHeightBtn;
	Button* m_importColorBtn;
	Button* m_exportColorBtn;
};

#endif