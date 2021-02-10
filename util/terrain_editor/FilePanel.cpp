#include <poly/Core/ObjectPool.h>

#include <poly/Graphics/Window.h>

#include "FilePanel.h"
#include "UITemplates.h"

#ifdef WIN32
#include <Windows.h>
#endif

#include <png.h>


///////////////////////////////////////////////////////////
bool isLittleEndian()
{
	short int number = 0x1;
	char* numPtr = (char*)&number;
	return (numPtr[0] == 1);
}


///////////////////////////////////////////////////////////
std::string openFileDialog(const char* filter)
{
#ifdef WIN32
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
		return std::string(ofn.lpstrFile);
#endif

	return "";
}


///////////////////////////////////////////////////////////
std::string saveFileDialog(const char* filter)
{
#ifdef WIN32
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn) == TRUE)
		return std::string(ofn.lpstrFile);
#endif

	return "";
}


///////////////////////////////////////////////////////////
void ImportExportDialog::onBtnRelease(Button* btn)
{
	if (btn->hasHover())
	{
		btn->setColor(0.25f, 0.25f, 0.30f, 1.0f);

		if (btn == m_pathBtn)
		{
			if (m_exportMode)
			{
				std::string path = saveFileDialog(".png\0*.png\0");

				if (path.size())
				{
					m_filePaths[m_typeMenu->getSelectedItem() + (m_exportMode ? 3 : 0)] = path;
					setPath(path);
				}
			}
			else
			{
				std::string path = openFileDialog(".png\0*.png\0");

				if (path.size())
				{
					m_filePaths[m_typeMenu->getSelectedItem() + (m_exportMode ? 3 : 0)] = path;
					setPath(path);
				}
			}
		}
		else
			// Hide window
			setVisible(false);

		if (m_onConfirm && btn == m_confirmBtn)
			m_onConfirm();
	}
	else
		btn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
}


///////////////////////////////////////////////////////////
ImportExportDialog::ImportExportDialog() :
	m_title			(Pool<Text>::alloc()),
	m_typeTitle		(Pool<Text>::alloc()),
	m_pathTitle		(Pool<Text>::alloc()),
	m_typeMenu		(Pool<Dropdown>::alloc()),
	m_pathLabel		(Pool<Text>::alloc()),
	m_pathBtn		(Pool<Button>::alloc()),
	m_confirmBtn	(Pool<Button>::alloc()),
	m_cancelBtn		(Pool<Button>::alloc()),
	m_mouseOffset	(0.0f),
	m_isPressed		(false),
	m_exportMode	(false)
{
	setSize(380.0f, 138.0f);
	setColor(0.15f, 0.15f, 0.18f, 1.0f);
	setAnchor(UIPosition::Center);
	setOrigin(UIPosition::Center);

	// Title
	m_title->setAnchor(UIPosition::TopCenter);
	m_title->setOrigin(UIPosition::TopCenter);
	m_title->setPosition(0.0f, 8.0f);
	m_title->setString("Import File");
	addChild(m_title);

	// Type title
	m_typeTitle->setPosition(8.0f, 35.0f);
	m_typeTitle->setString("Type:");
	addChild(m_typeTitle);

	// Type menu
	m_typeMenu->setAnchor(UIPosition::TopRight);
	m_typeMenu->setOrigin(UIPosition::TopRight);
	m_typeMenu->setPosition(-8.0f, 28.0f);
	m_typeMenu->setSize(150.0f, 25.0f);
	m_typeMenu->setColor(0.1f, 0.1f, 0.12f, 1.0f);
	m_typeMenu->setItemHeight(25.0f);
	m_typeMenu->setItemColor(0.18f, 0.18f, 0.2f, 1.0f);
	m_typeMenu->addItem("Height");
	m_typeMenu->addItem("Color");
	m_typeMenu->addItem("Detail");
	addChild(m_typeMenu);

	// Path title
	m_pathTitle->setPosition(8.0f, 65.0f);
	m_pathTitle->setString("Path:");
	addChild(m_pathTitle);

	// Path button
	m_pathBtn->setAnchor(UIPosition::TopRight);
	m_pathBtn->setOrigin(UIPosition::TopRight);
	m_pathBtn->setPosition(-8.0f, 58.0f);
	m_pathBtn->setSize(80.0f, 25.0f);
	m_pathBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_pathBtn->setString("Choose");
	m_pathBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_pathBtn, std::placeholders::_1));
	m_pathBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_pathBtn, std::placeholders::_1));
	m_pathBtn->onPress(std::bind(onBtnPress, m_pathBtn));
	m_pathBtn->onRelease(std::bind(&ImportExportDialog::onBtnRelease, this, m_pathBtn));
	addChild(m_pathBtn);

	// Path input
	m_pathLabel->setAnchor(UIPosition::TopRight);
	m_pathLabel->setOrigin(UIPosition::TopRight);
	m_pathLabel->setPosition(-92.0f, 65.0f);
	addChild(m_pathLabel);

	// Cancel button
	m_cancelBtn->setAnchor(UIPosition::TopRight);
	m_cancelBtn->setOrigin(UIPosition::TopRight);
	m_cancelBtn->setPosition(-8.0f, 105.0f);
	m_cancelBtn->setSize(80.0f, 25.0f);
	m_cancelBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_cancelBtn->setString("Cancel");
	m_cancelBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_cancelBtn, std::placeholders::_1));
	m_cancelBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_cancelBtn, std::placeholders::_1));
	m_cancelBtn->onPress(std::bind(onBtnPress, m_cancelBtn));
	m_cancelBtn->onRelease(std::bind(&ImportExportDialog::onBtnRelease, this, m_cancelBtn));
	addChild(m_cancelBtn);

	// Confirm button
	m_confirmBtn->setAnchor(UIPosition::TopRight);
	m_confirmBtn->setOrigin(UIPosition::TopRight);
	m_confirmBtn->setPosition(-92.0f, 105.0f);
	m_confirmBtn->setSize(80.0f, 25.0f);
	m_confirmBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_confirmBtn->setString("Import");
	m_confirmBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_confirmBtn, std::placeholders::_1));
	m_confirmBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_confirmBtn, std::placeholders::_1));
	m_confirmBtn->onPress(std::bind(onBtnPress, m_confirmBtn));
	m_confirmBtn->onRelease(std::bind(&ImportExportDialog::onBtnRelease, this, m_confirmBtn));
	addChild(m_confirmBtn);

	// Type menu
	m_typeMenu->onMouseEnter(
		[&](const E_MouseMove& e)
		{
			m_typeMenu->setColor(0.12f, 0.12f, 0.15f, 1.0f);
		}
	);

	m_typeMenu->onMouseLeave(
		[&](const E_MouseMove& e)
		{
			m_typeMenu->setColor(0.1f, 0.1f, 0.12f, 1.0f);
		}
	);

	m_typeMenu->onMouseEnterItem(
		[&](Button* button, const E_MouseMove& e)
		{
			button->setColor(0.2f, 0.2f, 0.25f, 1.0f);
		}
	);

	m_typeMenu->onMouseLeaveItem(
		[&](Button* button, const E_MouseMove& e)
		{
			button->setColor(0.18f, 0.18f, 0.2f, 1.0f);
		}
	);

	m_typeMenu->onItemChange(
		[&](Uint32 index)
		{
			// Change text input
			setPath(m_filePaths[index + (m_exportMode ? 3 : 0)]);
		}
	);

	setVisible(false);
}


///////////////////////////////////////////////////////////
ImportExportDialog::~ImportExportDialog()
{
	Pool<Text>::free(m_title);
	Pool<Text>::free(m_typeTitle);
	Pool<Text>::free(m_pathTitle);
	Pool<Dropdown>::free(m_typeMenu);
	Pool<Text>::free(m_pathLabel);
	Pool<Button>::free(m_pathBtn);
	Pool<Button>::free(m_confirmBtn);
	Pool<Button>::free(m_cancelBtn);
}


///////////////////////////////////////////////////////////
void ImportExportDialog::setExportMode(bool mode)
{
	m_exportMode = mode;

	if (m_exportMode)
	{
		m_title->setString("Export File");
		m_confirmBtn->setString("Export");
	}
	else
	{
		m_title->setString("Import File");
		m_confirmBtn->setString("Import");
	}

	setPath(m_filePaths[m_typeMenu->getSelectedItem() + (m_exportMode ? 3 : 0)]);
}


///////////////////////////////////////////////////////////
void ImportExportDialog::setPath(const std::string& path)
{
	m_filePaths[m_typeMenu->getSelectedItem() + (m_exportMode ? 3 : 0)] = path;

	// Display string
	const int maxLength = 40;
	std::string displayStr = path;
	if (displayStr.size() == 0)
		displayStr = "No path chosen";
	else if (displayStr.size() > maxLength)
		displayStr = "..." + displayStr.substr(displayStr.size() - maxLength + 3, maxLength - 3);

	m_pathLabel->setString(displayStr);
}


///////////////////////////////////////////////////////////
Uint32 ImportExportDialog::getMode() const
{
	return m_typeMenu->getSelectedItem();
}


///////////////////////////////////////////////////////////
const std::string& ImportExportDialog::getPath() const
{
	return m_filePaths[m_typeMenu->getSelectedItem() + (m_exportMode ? 3 : 0)];
}


///////////////////////////////////////////////////////////
bool ImportExportDialog::isExportMode() const
{
	return m_exportMode;
}


///////////////////////////////////////////////////////////
void ImportExportDialog::onConfirm(const std::function<void()>& func)
{
	m_onConfirm = func;
}


///////////////////////////////////////////////////////////
void ImportExportDialog::onMouseButton(const E_MouseButton& e)
{
	// Left click in top margin
	if (e.m_button != Mouse::Left) return;

	if (e.m_action == InputAction::Press)
	{
		const float margin = 25.0f;
		Vector2f offset = getLocalCoordinate(Window::getCurrent()->getCursorPos());
		float localY = offset.y + m_origin.y * getPixelSize().y;

		if (localY < margin)
		{
			// Keep track of offset
			m_mouseOffset = offset;
			m_isPressed = true;
		}
	}
	else
		m_isPressed = false;
}


///////////////////////////////////////////////////////////
void ImportExportDialog::onMouseMove(const E_MouseMove& e)
{
	if (m_isPressed)
	{
		// Get position relative to parent
		Vector2f p = m_parent->getLocalCoordinate(Vector2f(e.m_x, e.m_y));

		// Set position to mouse position plus original offset
		setPosition(p - m_mouseOffset - m_anchor * m_parent->getPixelSize());
	}
}


///////////////////////////////////////////////////////////
bool ImportExportDialog::handlesMouseEvents() const
{
	return true;
}


///////////////////////////////////////////////////////////
void FilePanel::onBtnRelease(Button* btn)
{
	if (btn->hasHover())
	{
		btn->setColor(0.25f, 0.25f, 0.30f, 1.0f);

		if (btn == m_openBtn)
			openFile();
		else if (btn == m_saveBtn)
			saveFile();
		else if (btn == m_importBtn)
			importFile();
		else if (btn == m_exportBtn)
			exportFile();
	}
	else
		btn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
}


///////////////////////////////////////////////////////////
FilePanel::FilePanel(ImportExportDialog* dialog, EditSystem* editSystem) :
	m_importExportDialog	(dialog),
	m_editSystem			(editSystem),
	m_panelTitle			(Pool<Text>::alloc()),
	m_openBtn				(Pool<Button>::alloc()),
	m_saveBtn				(Pool<Button>::alloc()),
	m_importBtn				(Pool<Button>::alloc()),
	m_exportBtn				(Pool<Button>::alloc())
{
	setSize(200.0f, 145.0f);
	setVisible(false, false);

	// Panel title
	m_panelTitle->setOrigin(UIPosition::TopCenter);
	m_panelTitle->setPosition(100.0f, 10.0f);
	m_panelTitle->setString("File Settings");
	addChild(m_panelTitle);
	
	// Open button
	m_openBtn->setPosition(5.0f, 30.0f);
	m_openBtn->setSize(190.0f, 25.0f);
	m_openBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_openBtn->setString("Open");
	m_openBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_openBtn, std::placeholders::_1));
	m_openBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_openBtn, std::placeholders::_1));
	m_openBtn->onPress(std::bind(onBtnPress, m_openBtn));
	m_openBtn->onRelease(std::bind(&FilePanel::onBtnRelease, this, m_openBtn));
	addChild(m_openBtn);

	// Open button
	m_saveBtn->setPosition(5.0f, 60.0f);
	m_saveBtn->setSize(190.0f, 25.0f);
	m_saveBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_saveBtn->setString("Save");
	m_saveBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_saveBtn, std::placeholders::_1));
	m_saveBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_saveBtn, std::placeholders::_1));
	m_saveBtn->onPress(std::bind(onBtnPress, m_saveBtn));
	m_saveBtn->onRelease(std::bind(&FilePanel::onBtnRelease, this, m_saveBtn));
	addChild(m_saveBtn);

	// Import
	m_importBtn->setPosition(5.0f, 90.0f);
	m_importBtn->setSize(190.0f, 25.0f);
	m_importBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_importBtn->setString("Import");
	m_importBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_importBtn, std::placeholders::_1));
	m_importBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_importBtn, std::placeholders::_1));
	m_importBtn->onPress(std::bind(onBtnPress, m_importBtn));
	m_importBtn->onRelease(std::bind(&FilePanel::onBtnRelease, this, m_importBtn));
	addChild(m_importBtn);

	// Export
	m_exportBtn->setPosition(5.0f, 120.0f);
	m_exportBtn->setSize(190.0f, 25.0f);
	m_exportBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_exportBtn->setString("Export");
	m_exportBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_exportBtn, std::placeholders::_1));
	m_exportBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_exportBtn, std::placeholders::_1));
	m_exportBtn->onPress(std::bind(onBtnPress, m_exportBtn));
	m_exportBtn->onRelease(std::bind(&FilePanel::onBtnRelease, this, m_exportBtn));
	addChild(m_exportBtn);

	// Setup dialog callback
	m_importExportDialog->onConfirm(std::bind(&FilePanel::onDialogConfirm, this));
}


///////////////////////////////////////////////////////////
FilePanel::~FilePanel()
{
	Pool<Text>::free(m_panelTitle);
	Pool<Button>::free(m_openBtn);
	Pool<Button>::free(m_saveBtn);
	Pool<Button>::free(m_importBtn);
	Pool<Button>::free(m_exportBtn);
}


///////////////////////////////////////////////////////////
void FilePanel::openFile()
{
	std::string path = openFileDialog(".terrain\0*.terrain\0");

	if (path.size())
	{

	}
}


///////////////////////////////////////////////////////////
void FilePanel::saveFile()
{

}


///////////////////////////////////////////////////////////
void FilePanel::importFile()
{
	m_importExportDialog->setExportMode(false);
	m_importExportDialog->setVisible(true);
}


///////////////////////////////////////////////////////////
void FilePanel::exportFile()
{
	m_importExportDialog->setExportMode(true);
	m_importExportDialog->setVisible(true);
}


///////////////////////////////////////////////////////////
void FilePanel::onDialogConfirm()
{
	// Get file path
	const std::string& path = m_importExportDialog->getPath();

	if (m_importExportDialog->isExportMode())
	{
		FILE* f = fopen(path.c_str(), "wb");
		if (!f)
		{
			LOG_ERROR("Failed to export terrain map %s", path);
			return;
		}

		// Initialize libpng
		png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_infop pngInfo = png_create_info_struct(pngPtr);
		png_init_io(pngPtr, f);

		void* data = 0;
		std::vector<Uint8*> rowPtrs;
		Vector2u size(0);
		int bitDepth, colorType;

		if (m_importExportDialog->getMode() == 0)
		{
			const bool littleEndian = isLittleEndian();
			Image& hmap = m_editSystem->getHeightMap();

			size.x = hmap.getWidth();
			size.y = hmap.getHeight();
			bitDepth = 16;
			colorType = PNG_COLOR_TYPE_GRAY;

			float* src = (float*)hmap.getData();
			Uint16* dst = (Uint16*)malloc(size.x * size.y * sizeof(float));
			data = dst;

			for (Uint32 r = 0, i = 0; r < size.y; ++r)
			{
				rowPtrs.push_back((Uint8*)(dst + r * size.x));

				for (Uint32 c = 0; c < size.x; ++c, ++i)
				{
					Uint32 index = r * size.x + c;
					dst[index] = (Uint16)(src[index] * 65535.0f);

					// Swap bytes if the machine is little endian
					if (littleEndian)
					{
						Uint8* ptr = (Uint8*)&dst[index];
						Uint8 temp = ptr[0];
						ptr[0] = ptr[1];
						ptr[1] = temp;
					}
				}
			}
		}

		// Setup info struct
		png_set_IHDR(pngPtr, pngInfo, size.x, size.y, bitDepth, colorType, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_write_info(pngPtr, pngInfo);

		// Write the image
		png_write_image(pngPtr, (Uint8**)&rowPtrs[0]);

		// Finish write
		png_write_end(pngPtr, NULL);

		// Clean up
		free(data);
		fclose(f);
	}
	else
	{
		FILE* f = fopen(path.c_str(), "rb");
		if (!f)
		{
			LOG_ERROR("Failed to import terrain map %s", path);
			return;
		}

		// Read header
		Uint8 header[8];
		fread(header, 1, 8, f);
		if (png_sig_cmp(header, 0, 8))
		{
			LOG_ERROR("Terrain maps must be PNG format", path);
			return;
		}
	}
}