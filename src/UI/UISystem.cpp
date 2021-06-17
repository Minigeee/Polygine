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
#include <poly/UI/ProgressBar.h>
#include <poly/UI/ScrollView.h>
#include <poly/UI/Slider.h>
#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>
#include <poly/UI/UIParser.h>
#include <poly/UI/UISystem.h>

#include <rapidxml.hpp>

#include <stack>

namespace poly
{


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
UISystem::UISystem(const std::string& fname) :
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

	load(fname);
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

			// Handle text differently
			Text* text = 0;
			if (text = dynamic_cast<Text*>(element))
			{
				// Add a new group every time the text changes color
				Vector4f color = transparentQuads[start].m_color;
				Uint32 prevGroupStart = start;

				for (Uint32 i = start + 1; i < transparentQuads.size(); ++i)
				{
					// If the colors don't match, end the previous group
					if (transparentQuads[i].m_color != color)
					{
						transparentRenderData.push_back(UIRenderData
							{
								element->getTexture(),
								element->getSrcBlend(),
								element->getDstBlend(),
								color,
								element->getShader(),
								clipRect,
								prevGroupStart,
								i - prevGroupStart,
								true,
								element->hasFlippedUv()
							});

						color = transparentQuads[i].m_color;
						prevGroupStart = i;
					}
				}

				// Add the last group
				transparentRenderData.push_back(UIRenderData
					{
						element->getTexture(),
						element->getSrcBlend(),
						element->getDstBlend(),
						color,
						element->getShader(),
						clipRect,
						prevGroupStart,
						transparentQuads.size() - prevGroupStart,
						true,
						element->hasFlippedUv()
					});
			}
			else
			{
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
						true,
						element->hasFlippedUv()
					});
			}

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
	// Attempt to unload if the system already has elements in it
	if (m_children.size())
	{
		// If the system was loaded, then free all current elements
		if (m_loaded)
		{
			// Loop through elements with ids and unload them
			for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
			{
				UIElement* element = it->second;
				if (element == this) continue;

				if (dynamic_cast<Dropdown*>(element))
					Pool<Dropdown>::free(dynamic_cast<Dropdown*>(element));

				else if (dynamic_cast<Button*>(element))
					Pool<Button>::free(dynamic_cast<Button*>(element));

				else if (dynamic_cast<HListView*>(element))
					Pool<HListView>::free(dynamic_cast<HListView*>(element));

				else if (dynamic_cast<VListView*>(element))
					Pool<VListView>::free(dynamic_cast<VListView*>(element));

				else if (dynamic_cast<ScrollView*>(element))
					Pool<ScrollView>::free(dynamic_cast<ScrollView*>(element));

				else if (dynamic_cast<Slider*>(element))
					Pool<Slider>::free(dynamic_cast<Slider*>(element));

				else if (dynamic_cast<Text*>(element))
					Pool<Text>::free(dynamic_cast<Text*>(element));

				else if (dynamic_cast<TextInput*>(element))
					Pool<TextInput>::free(dynamic_cast<TextInput*>(element));

				else if (dynamic_cast<ProgressBar*>(element))
					Pool<ProgressBar>::free(dynamic_cast<ProgressBar*>(element));

				else
					Pool<UIElement>::free(element);
			}

			// Clear children
			m_children.clear();

			// Clear elements
			m_elements.clear();
		}

		// Otherwise, can't load over the current elements
		else
			return false;
	}


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

	// Check if there is a templates section
	UITemplateMap templates;
	XmlNode templatesNode = mainNode.getFirstNode("templates");

	if (templatesNode.exists())
	{
		XmlNode current = templatesNode.getFirstNode();

		// Loop through and get all elements with an id
		while (current.exists())
		{
			// Find an id attribute
			XmlAttribute idAttr = current.getFirstAttribute("id");
			if (idAttr.exists())
				templates[idAttr.getValue()] = current;

			current = current.getNextSibling();
		}
	}

	// Parse the ui system using the root element
	parse(mainNode, templates);

	// Add all elements with an id to the list
	std::queue<UIElement*> elementQueue;
	elementQueue.push(this);

	while (elementQueue.size())
	{
		UIElement* element = elementQueue.front();
		elementQueue.pop();

		// Add the current element to id map if it has an id
		if (element->getId().size())
			m_elements[element->getId()] = element;

		const std::vector<UIElement*>& children = element->getChildren();
		for (Uint32 i = 0; i < children.size(); ++i)
			elementQueue.push(children[i]);
	}

	LOG("Loaded UI XML file: %s", fname.c_str());

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