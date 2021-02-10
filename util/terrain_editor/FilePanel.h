#ifndef TERRAIN_EDITOR_FILE_PANEL_H
#define TERRAIN_EDITOR_FILE_PANEL_H

#include <poly/UI/Button.h>
#include <poly/UI/Dropdown.h>
#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>

#include "EditSystem.h"

using namespace poly;


class ImportExportDialog : public UIElement
{
public:
	ImportExportDialog();

	~ImportExportDialog();

	void setExportMode(bool mode);

	void setPath(const std::string& path);

	Uint32 getMode() const;

	const std::string& getPath() const;

	bool isExportMode() const;

	void onConfirm(const std::function<void()>& func);

private:
	void onMouseButton(const E_MouseButton& e) override;

	void onMouseMove(const E_MouseMove& e) override;

	bool handlesMouseEvents() const override;

	void onBtnRelease(Button* btn);

private:
	Text* m_title;
	Text* m_typeTitle;
	Text* m_pathTitle;
	Dropdown* m_typeMenu;
	Text* m_pathLabel;
	Button* m_pathBtn;

	Button* m_confirmBtn;
	Button* m_cancelBtn;

	std::function<void()> m_onConfirm;
	Vector2f m_mouseOffset;
	bool m_isPressed;
	bool m_exportMode;

	std::string m_filePaths[6];
};


class FilePanel : public UIElement
{
public:
	FilePanel(ImportExportDialog* dialog, EditSystem* editSystem);

	~FilePanel();

	void openFile();

	void saveFile();

	void importFile();

	void exportFile();

private:
	void onDialogConfirm();

	void onBtnRelease(Button* btn);

private:
	ImportExportDialog* m_importExportDialog;
	EditSystem* m_editSystem;

	Text* m_panelTitle;
	Button* m_openBtn;
	Button* m_saveBtn;
	Button* m_importBtn;
	Button* m_exportBtn;
};

#endif