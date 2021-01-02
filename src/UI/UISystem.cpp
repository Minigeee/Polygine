#include <poly/Core/Profiler.h>

#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Window.h>

#include <poly/Math/Functions.h>

#include <poly/UI/UISystem.h>

namespace poly
{


///////////////////////////////////////////////////////////
Shader UISystem::s_shader;


///////////////////////////////////////////////////////////
UISystem::UISystem() :
	m_window				(0),
	m_instanceBufferOffset	(0),
	m_hovered				(0),
	m_focused				(0)
{
	m_isVisible = false;

	m_vertexArray.setNumVertices(1);
	m_vertexArray.setDrawMode(DrawMode::Points);
	m_instanceBuffer.create((UIInstanceData*)0, 65536, BufferUsage::Stream);
}


///////////////////////////////////////////////////////////
UISystem::UISystem(Uint32 w, Uint32 h) :
	m_window				(0),
	m_instanceBufferOffset	(0),
	m_hovered				(0),
	m_focused				(0)
{
	m_isVisible = false;
	m_pixelSize = Vector2f(w, h);

	m_vertexArray.setNumVertices(1);
	m_vertexArray.setDrawMode(DrawMode::Points);
	m_instanceBuffer.create((UIInstanceData*)0, 65536, BufferUsage::Stream);
}


///////////////////////////////////////////////////////////
void UISystem::update(float dt)
{

}


///////////////////////////////////////////////////////////
void UISystem::render(FrameBuffer& target, bool overlay)
{
	START_PROFILING_FUNC;

	// Bind framebuffer
	target.bind();

	// Clear only the depth buffer (so the UI can be overlayed)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if (overlay)
		glCheck(glClear(GL_DEPTH_BUFFER_BIT));
	else
		glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Enable blending
	glCheck(glEnable(GL_BLEND));

	std::vector<UIRenderData> renderData;
	std::vector<UIQuad> transparentData;
	std::vector<std::vector<UIQuad>> quads;

	// Get all render quads
	Uint32 index = 0;
	getRenderQuads(this, quads, renderData, transparentData, index);

	// Calculate size
	Uint32 numInstances = transparentData.size();
	for (Uint32 i = 0; i < quads.size(); ++i)
		numInstances += quads[i].size();


	// Stream instance data
	Uint32 size = numInstances * sizeof(UIInstanceData);
	int flags = (int)MapBufferFlags::Write | (int)MapBufferFlags::Unsynchronized;

	// Choose different flags based on how much space is left
	if (m_instanceBufferOffset + size > m_instanceBuffer.getSize())
	{
		flags |= (int)MapBufferFlags::InvalidateBuffer;
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
	Uint32 prevOffset = 0;
	for (Uint32 i = 0; i < transparentData.size(); ++i)
	{
		if (transparentData[i].m_texture != currentTexture || i == 0)
		{
			// Finish previous group
			if (i != 0)
				renderData.back().m_instances = i - prevOffset;

			// Add a new group every time texture changes
			currentTexture = transparentData[i].m_texture;
			renderData.push_back(UIRenderData{ currentTexture, transparentData[i].m_blendFactor, m_instanceBufferOffset, transparentData.size() - i });

			// Update previous offset
			prevOffset = i;
		}

		UIQuad& quad = transparentData[i];
		data[numInstancesMapped] = UIInstanceData
		{
			quad.m_position,
			quad.m_rotation,
			quad.m_size,
			quad.m_origin,
			quad.m_color,
			quad.m_textureRect,
			1.0f - (float)quad.m_index / index,
		};

		// Update number of instances mapped
		numInstancesMapped += 1;
		m_instanceBufferOffset += sizeof(UIInstanceData);
	}

	// Unmap buffer
	m_instanceBuffer.unmap();


	// Bind shader
	Shader& shader = getShader();
	shader.bind();
	shader.setUniform("u_targetSize", Vector2f(target.getWidth(), target.getHeight()));

	// Render all data
	for (Uint32 i = 0; i < renderData.size(); ++i)
	{
		UIRenderData& group = renderData[i];

		// Bind texture
		if (group.m_texture)
		{
			shader.setUniform("u_texture", *group.m_texture);
			shader.setUniform("u_hasTexture", true);
		}
		else
			shader.setUniform("u_hasTexture", false);

		// Set the blend factor
		glCheck(glBlendFunc((GLenum)group.m_blendFactor, GL_ONE_MINUS_SRC_ALPHA));

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
	}
}


///////////////////////////////////////////////////////////
void UISystem::getRenderQuads(
	UIElement* element,
	std::vector<std::vector<UIQuad>>& quads,
	std::vector<UIRenderData>& renderData,
	std::vector<UIQuad>& transparentData,
	Uint32& index)
{
	// Skip the element if it is not visible
	if (element->isVisible())
	{
		if (!element->isTransparent())
		{
			// Find group
			Uint32 group = 0;
			for (; group < renderData.size() && renderData[group].m_texture != element->getTexture(); ++group);

			// If the group doesn't exist, add it
			if (group == renderData.size())
			{
				renderData.push_back(UIRenderData{ element->getTexture(), element->getBlendFactor(), 0, 0 });
				quads.push_back(std::vector<UIQuad>());
			}

			// Keep track of start index
			Uint32 start = quads[group].size();

			// Get quads
			element->getQuads(quads[group]);

			// Keep track of its group
			for (Uint32 i = start; i < quads.size(); ++i)
			{
				quads[group][i].m_group = group;
				quads[group][i].m_index = index;
			}
		}
		else
		{
			// Keep track of start index
			Uint32 start = transparentData.size();

			// Get quads
			element->getQuads(transparentData);

			// Set index
			for (Uint32 i = start; i < transparentData.size(); ++i)
				transparentData[i].m_index = index;
		}

		// Increment index
		++index;
	}

	// Call for children
	for (Uint32 i = 0; i < element->m_children.size(); ++i)
		getRenderQuads(element->m_children[i], quads, renderData, transparentData, index);
}


///////////////////////////////////////////////////////////
Shader& UISystem::getShader()
{
	if (!s_shader.getId())
	{
		s_shader.load("shaders/ui.vert", Shader::Vertex);
		s_shader.load("shaders/ui.geom", Shader::Geometry);
		s_shader.load("shaders/ui.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
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
	m_onTextInputHandle =	m_window->addListener<E_TextInput>(std::bind(&UISystem::onTextInput, this, std::placeholders::_1));
}


///////////////////////////////////////////////////////////
void UISystem::onKeyEvent(const E_KeyEvent& e)
{
	if (m_hovered && m_hovered != this)
		m_hovered->onKeyEvent(e);

	if (m_focused && m_focused != this)
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

	// Update transforms
	element->updateProperties();

	// Adjust for translation
	Vector2f p = Vector2f(e.m_x, e.m_y) - element->m_absPosition;
	// Adjust for rotation
	float angle = rad(element->m_absRotation);
	float ca = cos(angle);
	float sa = sin(angle);
	p = Vector2f(p.x * ca - p.y * sa, p.x * sa + p.y * ca);
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
	// Relay the event
	relayMouseMove(this, e);
}


///////////////////////////////////////////////////////////
void UISystem::onMouseButton(const E_MouseButton& e)
{
	// Check current hovered, and change it to focus if needed
	if (m_hovered && m_hovered != m_focused)
	{
		// Send event for losing focus
		if (m_focused)
		{
			m_focused->m_hasFocus = false;
			m_focused->onLoseFocus();
		}

		// Send event for gaining focus
		m_hovered->m_hasFocus = true;
		m_hovered->onGainFocus();

		// Update current focused element
		m_focused = m_hovered;
	}

	// Send event for mouse button
	if (m_focused && m_focused != this)
		m_focused->onMouseButton(e);
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