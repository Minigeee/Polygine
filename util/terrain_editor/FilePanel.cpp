#include <poly/Core/ObjectPool.h>

#include "FilePanel.h"
#include "UITemplates.h"


///////////////////////////////////////////////////////////
void FilePanel::onBtnRelease(Button* btn)
{
	if (btn->hasHover())
	{
		btn->setColor(0.25f, 0.25f, 0.30f, 1.0f);
	}
	else
		btn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
}


///////////////////////////////////////////////////////////
FilePanel::FilePanel() :
	m_panelTitle		(Pool<Text>::alloc()),
	m_importHeightBtn	(Pool<Button>::alloc()),
	m_exportHeightBtn	(Pool<Button>::alloc()),
	m_importColorBtn	(Pool<Button>::alloc()),
	m_exportColorBtn	(Pool<Button>::alloc())
{
	setSize(200.0f, 145.0f);
	setVisible(false, false);

	// Panel title
	m_panelTitle->setOrigin(UIPosition::TopCenter);
	m_panelTitle->setPosition(100.0f, 10.0f);
	m_panelTitle->setString("File Settings");
	addChild(m_panelTitle);

	// Import height map
	m_importHeightBtn->setPosition(5.0f, 30.0f);
	m_importHeightBtn->setSize(190.0f, 25.0f);
	m_importHeightBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_importHeightBtn->setString("Import Height Map");
	m_importHeightBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_importHeightBtn, std::placeholders::_1));
	m_importHeightBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_importHeightBtn, std::placeholders::_1));
	m_importHeightBtn->onPress(std::bind(onBtnPress, m_importHeightBtn));
	m_importHeightBtn->onRelease(std::bind(&FilePanel::onBtnRelease, this, m_importHeightBtn));
	addChild(m_importHeightBtn);

	// Export height map
	m_exportHeightBtn->setPosition(5.0f, 60.0f);
	m_exportHeightBtn->setSize(190.0f, 25.0f);
	m_exportHeightBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_exportHeightBtn->setString("Export Height Map");
	m_exportHeightBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_exportHeightBtn, std::placeholders::_1));
	m_exportHeightBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_exportHeightBtn, std::placeholders::_1));
	m_exportHeightBtn->onPress(std::bind(onBtnPress, m_exportHeightBtn));
	m_exportHeightBtn->onRelease(std::bind(&FilePanel::onBtnRelease, this, m_exportHeightBtn));
	addChild(m_exportHeightBtn);

	// Import color map
	m_importColorBtn->setPosition(5.0f, 90.0f);
	m_importColorBtn->setSize(190.0f, 25.0f);
	m_importColorBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_importColorBtn->setString("Import Color Map");
	m_importColorBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_importColorBtn, std::placeholders::_1));
	m_importColorBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_importColorBtn, std::placeholders::_1));
	m_importColorBtn->onPress(std::bind(onBtnPress, m_importColorBtn));
	m_importColorBtn->onRelease(std::bind(&FilePanel::onBtnRelease, this, m_importColorBtn));
	addChild(m_importColorBtn);

	// Export color map
	m_exportColorBtn->setPosition(5.0f, 120.0f);
	m_exportColorBtn->setSize(190.0f, 25.0f);
	m_exportColorBtn->setColor(0.22f, 0.22f, 0.27f, 1.0f);
	m_exportColorBtn->setString("Export Color Map");
	m_exportColorBtn->onMouseEnter(std::bind(onMouseEnterBtn, m_exportColorBtn, std::placeholders::_1));
	m_exportColorBtn->onMouseLeave(std::bind(onMouseLeaveBtn, m_exportColorBtn, std::placeholders::_1));
	m_exportColorBtn->onPress(std::bind(onBtnPress, m_exportColorBtn));
	m_exportColorBtn->onRelease(std::bind(&FilePanel::onBtnRelease, this, m_exportColorBtn));
	addChild(m_exportColorBtn);
}