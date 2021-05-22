#include <poly/Core/ObjectPool.h>
#include <poly/Core/Profiler.h>
#include <poly/Core/XmlDocument.h>

#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Window.h>

#include <poly/Math/Functions.h>

#include <poly/UI/Button.h>
#include <poly/UI/Dropdown.h>
#include <poly/UI/Font.h>
#include <poly/UI/ListView.h>
#include <poly/UI/UIParser.h>
#include <poly/UI/ScrollView.h>
#include <poly/UI/Slider.h>
#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>
#include <poly/UI/UISystem.h>

#include <rapidxml.hpp>

#include <stack>

namespace poly
{

/*

///////////////////////////////////////////////////////////
void uiXmlParseText(Text* text, XmlNode node, HashMap<std::string, Font*>& fontMap)
{
	// Font
	XmlAttribute fontAttr = node.getFirstAttribute("font");
	if (fontAttr)
	{
		std::string fname = fontAttr.getValue();

		// Check if the font has been loaded
		auto it = fontMap.find(fname);
		if (it == fontMap.end())
		{
			Font* font = Pool<Font>::alloc();
			if (font->load(fname))
			{
				fontMap[fname] = font;
				text->setFont(font);
			}
			else
				Pool<Font>::free(font);
		}
		else
			text->setFont(it->second);
	}

	// Value
	if (strlen(node.getValue()) > 0)
		text->setString(node.getValue());

	else
	{
		XmlAttribute valueAttr = node.getFirstAttribute("value");
		if (valueAttr)
			text->setString(valueAttr.getValue());
	}

	// Character size
	XmlAttribute charSizeAttr = node.getFirstAttribute("character_size");
	if (charSizeAttr)
	{
		Uint32 size;
		if (uiXmlParseInt(charSizeAttr.getValue(), size))
			text->setCharacterSize(size);
	}

	// Character spacing
	XmlAttribute charSpacingAttr = node.getFirstAttribute("character_spacing");
	if (charSpacingAttr)
	{
		float spacing;
		if (uiXmlParseFloat(charSpacingAttr.getValue(), spacing))
			text->setCharacterSpacing(spacing);
	}

	// Line spacing
	XmlAttribute lineSpacingAttr = node.getFirstAttribute("line_spacing");
	if (lineSpacingAttr)
	{
		float spacing;
		if (uiXmlParseFloat(lineSpacingAttr.getValue(), spacing))
			text->setLineSpacing(spacing);
	}
}


///////////////////////////////////////////////////////////
void uiXmlParseButton(Button* button, XmlNode node, HashMap<std::string, Texture*>& textureMap, HashMap<std::string, Font*>& fontMap)
{
	// Text value
	XmlAttribute valueAttr = node.getFirstAttribute("value");
	if (valueAttr)
		button->setString(valueAttr.getValue());

	else
	{
		XmlAttribute textAttr = node.getFirstAttribute("text");
		if (textAttr)
			button->setString(textAttr.getValue());
	}

	// Text align
	XmlAttribute textAlignAttr = node.getFirstAttribute("text_align");
	if (textAlignAttr)
	{
		UIPosition align;
		if (uiXmlParseUiPosition(textAlignAttr.getValue(), align))
			button->setTextAlign(align);
	}

	// Text offset
	XmlAttribute textOffsetAttr = node.getFirstAttribute("text_offset");
	if (textOffsetAttr)
	{
		Vector2f offset;
		if (uiXmlParseVec2(textOffsetAttr.getValue(), offset))
			button->setTextOffset(offset);
	}

	// Parse text options
	XmlNode textNode = node.getFirstNode("element_text");
	if (textNode)
	{
		uiXmlParseElement(button->getText(), textNode, textureMap);
		uiXmlParseText(button->getText(), textNode, fontMap);
	}
}


///////////////////////////////////////////////////////////
void uiXmlParseDropdown(Dropdown* dropdown, XmlNode node, HashMap<std::string, Texture*>& textureMap, HashMap<std::string, Font*>& fontMap)
{
	// Load as button because dropdown inherits from button
	uiXmlParseButton(dropdown, node, textureMap, fontMap);

	// Item height
	XmlAttribute itemHeightAttr = node.getFirstAttribute("item_height");
	if (itemHeightAttr)
	{
		float height;
		if (uiXmlParseFloat(itemHeightAttr.getValue(), height))
			dropdown->setItemHeight(height);
	}

	// Item color
	XmlAttribute itemColorAttr = node.getFirstAttribute("item_color");
	if (itemColorAttr)
	{
		Vector4f color4;
		Vector3f color3;
		char* c = itemColorAttr.getValue();

		if (c[0] == '#' && uiXmlParseHexColor(c, color4))
			dropdown->setItemColor(color4);

		else if (uiXmlParseVec4(c, color4))
			dropdown->setItemColor(color4);

		else if (uiXmlParseVec3(c, color3))
			dropdown->setItemColor(Vector4f(color3, 1.0f));

		else
			dropdown->setItemColor(getNamedColor(c));
	}

	// Add dropdown items
	XmlNode itemNode = node.getFirstNode("dropdown_item");
	while (itemNode)
	{
		Uint32 itemLen = strlen(itemNode.getValue());

		if (itemLen)
			dropdown->addItem(itemNode.getValue());

		else
		{
			Button* item = Pool<Button>::alloc();

			uiXmlParseElement(item, itemNode, textureMap);
			uiXmlParseButton(item, itemNode, textureMap, fontMap);

			// Add the custom button
			dropdown->addItem(item);
		}

		itemNode = itemNode.getNextSibling("dropdown_item");
	}
}


///////////////////////////////////////////////////////////
void uiXmlParseSlider(Slider* slider, XmlNode node, HashMap<std::string, Texture*>& textureMap)
{
	// Slider value
	XmlAttribute valueAttr = node.getFirstAttribute("value");
	if (valueAttr)
	{
		float value;
		if (uiXmlParseFloat(valueAttr.getValue(), value))
			slider->setValue(value);
	}

	// Slider button
	XmlNode buttonNode = node.getFirstNode("slider_button");
	if (buttonNode)
		uiXmlParseElement(slider->getSliderButton(), buttonNode, textureMap);
}


///////////////////////////////////////////////////////////
void uiXmlParseScrollView(ScrollView* view, XmlNode node, HashMap<std::string, Texture*>& textureMap)
{
	// Clip margins
	XmlAttribute marginsAttr = node.getFirstAttribute("clip_margins");
	if (marginsAttr)
	{
		float margins;
		if (uiXmlParseFloat(marginsAttr.getValue(), margins))
			view->setClipMargin(margins);
	}

	// Scroll speed
	XmlAttribute speedAttr = node.getFirstAttribute("scroll_speed");
	if (speedAttr)
	{
		float speed;
		if (uiXmlParseFloat(speedAttr.getValue(), speed))
			view->setScrollSpeed(speed);
	}

	// Scroll bar width
	XmlAttribute barWidthAttr = node.getFirstAttribute("scroll_bar_width");
	if (barWidthAttr)
	{
		float barWidth;
		if (uiXmlParseFloat(barWidthAttr.getValue(), barWidth))
			view->setScrollBarWidth(barWidth);
	}

	// Slider
	XmlNode sliderNode = node.getFirstNode("scroll_bar");
	if (sliderNode)
		uiXmlParseElement(view->getScrollBar(), sliderNode, textureMap);
}


///////////////////////////////////////////////////////////
void uiXmlParseTextInput(TextInput* input, XmlNode node, HashMap<std::string, Texture*>& textureMap, HashMap<std::string, Font*>& fontMap)
{
	// Text value
	XmlAttribute valueAttr = node.getFirstAttribute("value");
	if (valueAttr)
		input->setValue(valueAttr.getValue());

	else
	{
		XmlAttribute textAttr = node.getFirstAttribute("text");
		if (textAttr)
			input->setValue(textAttr.getValue());
	}

	// Text cursor size
	XmlAttribute cursorSizeAttr = node.getFirstAttribute("cursor_size");
	if (cursorSizeAttr)
	{
		Vector2f size;
		if (uiXmlParseVec2(cursorSizeAttr.getValue(), size))
			input->setTextCursorSize(size);
	}

	// Text cursor color
	XmlAttribute cursorColorAttr = node.getFirstAttribute("cursor_color");
	if (cursorColorAttr)
	{
		Vector4f color4;
		Vector3f color3;
		char* c = cursorColorAttr.getValue();

		if (c[0] == '#' && uiXmlParseHexColor(c, color4))
			input->setTextCursorColor(color4);

		else if (uiXmlParseVec4(c, color4))
			input->setTextCursorColor(color4);

		else if (uiXmlParseVec3(c, color3))
			input->setTextCursorColor(Vector4f(color3, 1.0f));

		else
			input->setTextCursorColor(getNamedColor(c));
	}

	// Text cursor cycle
	XmlAttribute cursorCycleAttr = node.getFirstAttribute("cursor_cycle");
	if (cursorCycleAttr)
	{
		float cycle;
		if (uiXmlParseFloat(cursorCycleAttr.getValue(), cycle))
			input->setTextCursorCycle(cycle);
	}

	// Highlight cursor color
	XmlAttribute highlightColorAttr = node.getFirstAttribute("highlight_color");
	if (highlightColorAttr)
	{
		Vector4f color4;
		Vector3f color3;
		char* c = highlightColorAttr.getValue();

		if (c[0] == '#' && uiXmlParseHexColor(c, color4))
			input->setHighlightColor(color4);

		else if (uiXmlParseVec4(c, color4))
			input->setHighlightColor(color4);

		else if (uiXmlParseVec3(c, color3))
			input->setHighlightColor(Vector4f(color3, 1.0f));

		else
			input->setHighlightColor(getNamedColor(c));
	}

	// Text align
	XmlAttribute textAlignAttr = node.getFirstAttribute("text_align");
	if (textAlignAttr)
	{
		UIPosition align;
		if (uiXmlParseUiPosition(textAlignAttr.getValue(), align))
			input->setTextAlign(align);
	}

	// Text offset
	XmlAttribute textOffsetAttr = node.getFirstAttribute("text_offset");
	if (textOffsetAttr)
	{
		Vector2f offset;
		if (uiXmlParseVec2(textOffsetAttr.getValue(), offset))
			input->setTextOffset(offset);
	}

	// Parse text options
	XmlNode textNode = node.getFirstNode("element_text");
	if (textNode)
	{
		uiXmlParseElement(input->getText(), textNode, textureMap);
		uiXmlParseText(input->getText(), textNode, fontMap);
	}
}

*/


///////////////////////////////////////////////////////////
UISystem::UISystem() :
	m_window				(0),
	m_instanceBufferOffset	(0),
	m_hovered				(0),
	m_focused				(0),
	m_loaded				(false)
{
	m_isVisible = false;

	m_vertexArray.setNumVertices(1);
	m_vertexArray.setDrawMode(DrawMode::Points);
	m_instanceBuffer.create((UIInstanceData*)0, 65536, BufferUsage::Stream);
}


///////////////////////////////////////////////////////////
UISystem::~UISystem()
{

}


///////////////////////////////////////////////////////////
void UISystem::updateElement(UIElement* element, float dt)
{
	if (element != this)
		element->update(dt);

	for (Uint32 i = 0; i < element->m_children.size(); ++i)
		updateElement(element->m_children[i], dt);
}


///////////////////////////////////////////////////////////
void UISystem::update(float dt)
{
	updateElement(this, dt);
}


///////////////////////////////////////////////////////////
void UISystem::render(FrameBuffer& target, bool overlay)
{
	START_PROFILING_FUNC;

	// Check if the sizes are the same
	if (target.getWidth() != m_pixelSize.x || target.getHeight() != m_pixelSize.y)
		setSize((float)target.getWidth(), (float)target.getHeight());

	// Bind framebuffer
	target.bind();

	// Clear only the depth buffer (so the UI can be overlayed)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if (overlay)
		glCheck(glClear(GL_DEPTH_BUFFER_BIT));
	else
		glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Enable depth test
	glCheck(glEnable(GL_DEPTH_TEST));

	std::vector<UIRenderData> renderData;
	std::vector<UIRenderData> transparentRenderData;
	std::vector<UIQuad> transparentQuads;
	std::vector<std::vector<UIQuad>> quads;

	// Get all render quads
	Uint32 index = 0;
	getRenderQuads(this, quads, renderData, transparentQuads, transparentRenderData, Vector4f(0.0f), index);

	// Calculate size
	Uint32 numInstances = transparentQuads.size();
	for (Uint32 i = 0; i < quads.size(); ++i)
		numInstances += quads[i].size();

	if (!numInstances) return;

	// Sort render data
	std::sort(renderData.begin(), renderData.end(),
		[](const UIRenderData& a, const UIRenderData& b) -> bool
		{
			if (a.m_shader == b.m_shader)
				return a.m_offset < b.m_offset;
			else
				return a.m_shader < b.m_shader;
		}
	);


	// Stream instance data
	Uint32 size = numInstances * sizeof(UIInstanceData);
	MapBufferFlags flags = MapBufferFlags::Write | MapBufferFlags::Unsynchronized;

	// Choose different flags based on how much space is left
	if (m_instanceBufferOffset + size > m_instanceBuffer.getSize())
	{
		flags |= MapBufferFlags::InvalidateBuffer;
		m_instanceBufferOffset = 0;
	}

	// Map the instance buffer
	UIInstanceData* data = (UIInstanceData*)m_instanceBuffer.map(m_instanceBufferOffset, size, flags);

	Uint32 numInstancesMapped = 0;

	// Iterate through quads and stream data
	for (Uint32 i = 0; i < quads.size(); ++i)
	{
		std::vector<UIQuad>& group = quads[i];

		for (Uint32 j = 0; j < group.size(); ++j)
		{
			UIQuad& quad = group[j];
			data[numInstancesMapped + j] = UIInstanceData
			{
				quad.m_position,
				quad.m_rotation,
				quad.m_size,
				quad.m_origin,
				quad.m_color,
				quad.m_textureRect,
				1.0f - (float)quad.m_index / index,
			};
		}

		// Update render data metadata
		renderData[i].m_offset = m_instanceBufferOffset;
		renderData[i].m_instances = group.size();

		// Update number of instances mapped
		numInstancesMapped += group.size();
		m_instanceBufferOffset += group.size() * sizeof(UIInstanceData);
	}

	// Map transparent data
	Texture* currentTexture = 0;
	Shader* currentShader = 0;
	Vector4f currentClipRect(0.0f);
	Vector4f currentBlendColor(1.0f);
	Uint32 prevOffset = 0;
	for (Uint32 i = 0; i < transparentRenderData.size(); ++i)
	{
		UIRenderData& group = transparentRenderData[i];

		if (
			group.m_texture != currentTexture ||
			group.m_shader != currentShader ||
			group.m_clipRect != currentClipRect ||
			group.m_blendColor != currentBlendColor ||
			i == 0)
		{
			// Finish previous group
			if (i != 0)
				renderData.back().m_instances = group.m_offset - prevOffset;

			// Add a new group every time texture changes
			currentTexture = group.m_texture;
			currentShader = group.m_shader;
			currentClipRect = group.m_clipRect;
			currentBlendColor = group.m_blendColor;
			renderData.push_back(UIRenderData
				{
					currentTexture,
					group.m_srcFactor,
					group.m_dstFactor,
					group.m_blendColor,
					currentShader,
					group.m_clipRect,
					m_instanceBufferOffset,
					transparentQuads.size() - group.m_offset,
					true,
					group.m_hasFlippedUv
				});

			// Update previous offset
			prevOffset = group.m_offset;
		}

		for (Uint32 j = 0; j < group.m_instances; ++j)
		{
			UIQuad& quad = transparentQuads[group.m_offset + j];
			data[numInstancesMapped + j] = UIInstanceData
			{
				quad.m_position,
				quad.m_rotation,
				quad.m_size,
				quad.m_origin,
				quad.m_color,
				quad.m_textureRect,
				1.0f - (float)quad.m_index / index,
			};
		}

		// Update number of instances mapped
		numInstancesMapped += group.m_instances;
		m_instanceBufferOffset += group.m_instances * sizeof(UIInstanceData);
	}

	// Unmap buffer
	m_instanceBuffer.unmap();


	// Bind shader
	Shader* shader = renderData[0].m_shader;
	shader->bind();
	shader->setUniform("u_targetSize", Vector2f(target.getWidth(), target.getHeight()));

	// Render all data
	for (Uint32 i = 0; i < renderData.size(); ++i)
	{
		UIRenderData& group = renderData[i];

		// Change shader if needed
		if (group.m_shader != shader)
		{
			shader = group.m_shader;
			shader->bind();
			shader->setUniform("u_targetSize", Vector2f(target.getWidth(), target.getHeight()));
		}

		// Bind texture
		if (group.m_texture)
		{
			shader->setUniform("u_texture", *group.m_texture);
			shader->setUniform("u_flippedUv", (int)group.m_hasFlippedUv);
			shader->setUniform("u_hasTexture", true);
		}
		else
			shader->setUniform("u_hasTexture", false);

		// Set the blend factor
		if (group.m_transparent)
		{
			if (group.m_srcFactor == BlendFactor::ConstColor)
				glCheck(glBlendColor(group.m_blendColor.r, group.m_blendColor.g, group.m_blendColor.b, group.m_blendColor.a));

			glCheck(glEnable(GL_BLEND));
			glCheck(glBlendFunc((GLenum)group.m_srcFactor, (GLenum)group.m_dstFactor));
		}

		// Set up scissors clipping rectangle
		bool hasClipping = group.m_clipRect.z * group.m_clipRect.w > 0.0f;
		if (hasClipping)
		{
			const Vector4f& rect = group.m_clipRect;

			glCheck(glEnable(GL_SCISSOR_TEST));
			glCheck(glScissor((int)rect.x, (int)target.getHeight() - (int)(rect.y + rect.w), (int)rect.z, (int)rect.w));
		}

		m_vertexArray.bind();
		m_vertexArray.addBuffer(m_instanceBuffer, 0, 2, sizeof(UIInstanceData), group.m_offset + 0 * sizeof(float), 1);		// Position
		m_vertexArray.addBuffer(m_instanceBuffer, 1, 1, sizeof(UIInstanceData), group.m_offset + 2 * sizeof(float), 1);		// Rotation
		m_vertexArray.addBuffer(m_instanceBuffer, 2, 2, sizeof(UIInstanceData), group.m_offset + 3 * sizeof(float), 1);		// Size
		m_vertexArray.addBuffer(m_instanceBuffer, 3, 2, sizeof(UIInstanceData), group.m_offset + 5 * sizeof(float), 1);		// Origin
		m_vertexArray.addBuffer(m_instanceBuffer, 4, 4, sizeof(UIInstanceData), group.m_offset + 7 * sizeof(float), 1);		// Color
		m_vertexArray.addBuffer(m_instanceBuffer, 5, 4, sizeof(UIInstanceData), group.m_offset + 11 * sizeof(float), 1);	// Texture rect
		m_vertexArray.addBuffer(m_instanceBuffer, 6, 1, sizeof(UIInstanceData), group.m_offset + 15 * sizeof(float), 1);	// Index

		// Draw elements
		m_vertexArray.draw(group.m_instances);

		if (hasClipping)
			glCheck(glDisable(GL_SCISSOR_TEST));

		if (group.m_transparent)
			glCheck(glDisable(GL_BLEND));
	}
}


///////////////////////////////////////////////////////////
void UISystem::getRenderQuads(
	UIElement* element,
	std::vector<std::vector<UIQuad>>& quads,
	std::vector<UIRenderData>& renderData,
	std::vector<UIQuad>& transparentQuads,
	std::vector<UIRenderData>& transparentRenderData,
	const Vector4f& clipRect,
	Uint32& index)
{
	// Skip the element if it is not visible
	const Vector2f& size = element->getPixelSize();
	if (element->isVisible() && element->getColor().a > 0.0f && size.x * size.y > 0.0f)
	{
		if (!element->isTransparent())
		{
			// Find group
			Uint32 group = 0;
			for (; group < renderData.size(); ++group)
			{
				if (renderData[group].m_texture == element->getTexture() &&
					renderData[group].m_shader == element->getShader() &&
					renderData[group].m_clipRect == element->getClipRect())
					break;
			}

			// If the group doesn't exist, add it
			if (group == renderData.size())
			{
				renderData.push_back(UIRenderData
					{
						element->getTexture(),
						element->getSrcBlend(),
						element->getDstBlend(),
						element->getColor(),
						element->getShader(),
						clipRect,
						0, 0, false,
						element->hasFlippedUv()
					});
				quads.push_back(std::vector<UIQuad>());
			}

			// Keep track of start index
			Uint32 start = quads[group].size();

			// Get quads
			element->getQuads(quads[group]);

			// Keep track of its group
			for (Uint32 i = start; i < quads[group].size(); ++i)
				quads[group][i].m_index = index;
		}
		else
		{
			// Keep track of start index
			Uint32 start = transparentQuads.size();

			// Get quads
			element->getQuads(transparentQuads);

			// Add transparent render data
			transparentRenderData.push_back(UIRenderData
				{
					element->getTexture(),
					element->getSrcBlend(),
					element->getDstBlend(),
					element->getColor(),
					element->getShader(),
					clipRect,
					start,
					transparentQuads.size() - start,
					false,
					element->hasFlippedUv()
				});

			// Set index
			for (Uint32 i = start; i < transparentQuads.size(); ++i)
				transparentQuads[i].m_index = index;
		}

		// Increment index
		++index;
	}

	// Get next clip rectangle
	Vector4f newClipRect = element->getClipRect();
	if (newClipRect.z * newClipRect.w == 0.0f)
		newClipRect = clipRect;

	// Call for children
	for (Uint32 i = 0; i < element->m_children.size(); ++i)
		getRenderQuads(element->m_children[i], quads, renderData, transparentQuads, transparentRenderData, newClipRect, index);
}


///////////////////////////////////////////////////////////
void UISystem::setWindow(Window* window)
{
	// Reset previous handles
	if (m_window)
	{
		m_window->removeListener<E_KeyEvent>(m_onKeyEventHandle);
		m_window->removeListener<E_MouseButton>(m_onMouseButtonHandle);
		m_window->removeListener<E_MouseMove>(m_onMouseMoveHandle);
		m_window->removeListener<E_MouseScroll>(m_onMouseScrollHandle);
		m_window->removeListener<E_TextInput>(m_onTextInputHandle);
	}

	// Set listeners
	m_window = window;
	m_onKeyEventHandle =	m_window->addListener<E_KeyEvent>(std::bind(&UISystem::onKeyEvent, this, std::placeholders::_1));
	m_onMouseButtonHandle = m_window->addListener<E_MouseButton>(std::bind(&UISystem::onMouseButton, this, std::placeholders::_1));
	m_onMouseMoveHandle =	m_window->addListener<E_MouseMove>(std::bind(&UISystem::onMouseMove, this, std::placeholders::_1));
	m_onMouseScrollHandle = m_window->addListener<E_MouseScroll>(std::bind(&UISystem::onMouseScroll, this, std::placeholders::_1));
	m_onTextInputHandle = m_window->addListener<E_TextInput>(std::bind(&UISystem::onTextInput, this, std::placeholders::_1));
}


///////////////////////////////////////////////////////////
bool UISystem::load(const std::string& fname)
{
	// Don't load a second time
	if (m_loaded) return true;


	// Load the xml document
	XmlDocument doc;
	if (!doc.load(fname))
	{
		LOG_WARNING("Failed to load UI XML file: %s", fname.c_str());
		return false;
	}

	// Get the main node
	XmlNode mainNode = doc.getFirstNode("ui");
	if (!mainNode.exists())
	{
		LOG_WARNING("Invalid UI XML file (must start with <ui>): %s", fname.c_str());
		return false;
	}

	// Font
	XmlNode fontNode = mainNode.getFirstNode("font");
	if (fontNode.exists())
	{
		Font* font;
		if (UIParser::parse(fontNode, font))
			Text::setDefaultFont(font);
	}


	// Keep track of parent elements
	std::stack<UIElement*> parentStack;
	std::stack<XmlNode> nodeStack;
	parentStack.push(this);
	nodeStack.push(XmlNode());

	// Iterate through ui elements
	XmlNode current = mainNode.getFirstNode();
	while (current.exists())
	{
		// Create an element
		UIElement* element = 0;

		// Create an element of the correct type
		if (strcmp(current.getName(), "ui_element") == 0)
			element = Pool<UIElement>::alloc();

		else if (strcmp(current.getName(), "button") == 0)
			element = Pool<Button>::alloc();

		else if (strcmp(current.getName(), "dropdown") == 0)
			element = Pool<Dropdown>::alloc();

		else if (strcmp(current.getName(), "h_list_view") == 0)
			element = Pool<HListView>::alloc();

		else if (strcmp(current.getName(), "v_list_view") == 0 || strcmp(current.getName(), "list_view") == 0)
			element = Pool<VListView>::alloc();

		else if (strcmp(current.getName(), "scroll_view") == 0)
			element = Pool<ScrollView>::alloc();

		else if (strcmp(current.getName(), "slider") == 0)
			element = Pool<Slider>::alloc();

		else if (strcmp(current.getName(), "text") == 0)
			element = Pool<Text>::alloc();

		else if (strcmp(current.getName(), "text_input") == 0)
			element = Pool<TextInput>::alloc();

		else
		{
			// Skip non UI elements
			current = current.getNextSibling();

			// If there are no more elements in the current level, go up 1 level
			if (!current.exists())
			{
				current = nodeStack.top();
				parentStack.pop();
				nodeStack.pop();
			}

			continue;
		}

		// Check if the element has an id
		XmlAttribute idAttrib = current.getFirstAttribute("id");
		std::string id = idAttrib.exists() ? idAttrib.getValue() : "";

		// Parse the element
		element->parse(current);

		// Check if the node has a margin attribute
		XmlAttribute marginsAttr = current.getFirstAttribute("margins");
		bool usesMargins = false;

		if (marginsAttr.exists() && parentStack.size() > 1)
		{
			Vector2f margins;
			if (UIParser::parse(marginsAttr, margins))
			{
				VListView* vlist;
				HListView* hlist;

				usesMargins = true;

				if (vlist = dynamic_cast<VListView*>(parentStack.top()))
					vlist->addChild(element, margins);
				else if (hlist = dynamic_cast<HListView*>(parentStack.top()))
					hlist->addChild(element, margins);
				else
					usesMargins = false;
			}
		}

		if (!usesMargins)
			parentStack.top()->addChild(element);


		// Add the element to the map
		if (id.size() > 0)
		{
			element->setId(id);
			m_elements[id] = element;
		}

		// Add child elements
		XmlNode childrenNode = current.getFirstNode();

		// Go to next sibling
		current = current.getNextSibling();

		if (childrenNode.exists())
		{
			// Add current node to parent queue
			parentStack.push(element);
			nodeStack.push(current);

			// Go to first child element
			current = childrenNode;
		}

		// Go up one level if the current node is 0
		if (!current.exists() && parentStack.size())
		{
			current = nodeStack.top();
			parentStack.pop();
			nodeStack.pop();
		}
	}


	m_loaded = true;
	return true;
}


///////////////////////////////////////////////////////////
UIElement* UISystem::getElement(const std::string& id) const
{
	auto it = m_elements.find(id);
	return it == m_elements.end() ? 0 : it->second;
}


///////////////////////////////////////////////////////////
void UISystem::onKeyEvent(const E_KeyEvent& e)
{
	if (m_hovered && m_hovered != this)
		m_hovered->onKeyEvent(e);

	if (m_focused && m_focused != m_hovered && m_focused != this)
		m_focused->onKeyEvent(e);
}


///////////////////////////////////////////////////////////
bool UISystem::relayMouseMove(UIElement* element, const E_MouseMove& e)
{
	// Relay to children first, back to front
	for (int i = element->m_children.size() - 1; i >= 0; --i)
	{
		if (relayMouseMove(element->m_children[i], e))
			return true;
	}

	// Skip if element does not handle mouse events
	if (!element->handlesMouseEvents() || !element->isVisible()) return false;

	Vector2f p = element->getLocalCoordinate(Vector2f(e.m_x, e.m_y));
	p += element->m_pixelSize * element->m_origin;

	// The point is now in the element's local coordinate space
	// Check if the point is inside the element
	const Vector2f& size = element->getPixelSize();
	if (p.x > 0.0f && p.x < size.x && p.y > 0.0f && p.y < size.y)
	{
		// Check if this element has hover
		if (!element->hasHover())
		{
			// Send event for mouse leave previous area
			if (m_hovered)
			{
				m_hovered->m_hasHover = false;
				m_hovered->onMouseLeave(e);
			}

			// Send event for entering the new element
			element->m_hasHover = true;
			element->onMouseEnter(e);

			// Update the current hovered element
			m_hovered = element;

			// Has hover for a UI system is different, it tells if any of its children
			// has hover
			m_hasHover = true;
		}

		// Send event for mouse move
		if (element != this)
			element->onMouseMove(e);

		return true;
	}

	return false;
}


///////////////////////////////////////////////////////////
void UISystem::onMouseMove(const E_MouseMove& e)
{
	if (m_hovered)
	{
		Vector2f p = m_hovered->getLocalCoordinate(Vector2f(e.m_x, e.m_y));
		p += m_hovered->m_pixelSize * m_hovered->m_origin;

		const Vector2f& size = m_hovered->getPixelSize();
		if (p.x < 0.0f || p.x > size.x || p.y < 0.0f || p.y > size.y)
		{
			m_hovered->m_hasHover = false;
			m_hovered->onMouseLeave(e);

			m_hovered = 0;

			// Has hover for a UI system is different, it tells if any of its children
			// has hover
			m_hasHover = false;
		}
	}

	// Relay the event
	relayMouseMove(this, e);

	// Pass move event to focused element
	if (m_focused && m_focused != this)
		m_focused->onMouseMove(e);
}


///////////////////////////////////////////////////////////
void UISystem::onMouseButton(const E_MouseButton& e)
{
	// Check current hovered, and change it to focus if needed
	if (e.m_action == InputAction::Press && m_hovered != m_focused)
	{
		// Send event for losing focus
		if (m_focused)
		{
			m_focused->m_hasFocus = false;
			m_focused->onLoseFocus();
		}

		if (m_hovered)
		{
			// Send event for gaining focus
			m_hovered->m_hasFocus = true;
			m_hovered->onGainFocus();
		}

		// Update current focused element
		m_focused = m_hovered;
	}

	// Send event for mouse button
	if (m_focused && m_focused != this)
		m_focused->onMouseButton(e);

	if (m_hovered && m_hovered != m_focused && m_hovered != this)
		m_hovered->onMouseButton(e);
}


///////////////////////////////////////////////////////////
void UISystem::onMouseScroll(const E_MouseScroll& e)
{
	// Send event for hovered element
	if (m_hovered && m_hovered != this)
		m_hovered->onMouseScroll(e);
}


///////////////////////////////////////////////////////////
void UISystem::onTextInput(const E_TextInput& e)
{
	// Send event for focused element
	if (m_focused && m_focused != this)
		m_focused->onTextInput(e);
}


}