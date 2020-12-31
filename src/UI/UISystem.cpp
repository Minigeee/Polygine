#include <poly/Core/Profiler.h>

#include <poly/Graphics/GLCheck.h>

#include <poly/UI/UISystem.h>

namespace poly
{


///////////////////////////////////////////////////////////
Shader UISystem::s_shader;


///////////////////////////////////////////////////////////
UISystem::UISystem() :
	m_instanceBufferOffset	(0)
{
	m_isVisible = false;

	m_vertexArray.setNumVertices(1);
	m_vertexArray.setDrawMode(DrawMode::Points);
	m_instanceBuffer.create((UIInstanceData*)0, 65536, BufferUsage::Stream);
}


///////////////////////////////////////////////////////////
UISystem::UISystem(Uint32 w, Uint32 h) :
	m_instanceBufferOffset	(0)
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
void UISystem::render(FrameBuffer& target)
{
	START_PROFILING_FUNC;

	// Bind framebuffer
	target.bind();

	// Clear buffers
	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

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
	UIInstanceData* data = (UIInstanceData * )m_instanceBuffer.map(m_instanceBufferOffset, size, flags);

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
				(float)quad.m_index / index,
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
			renderData.push_back(UIRenderData{ currentTexture, m_instanceBufferOffset, transparentData.size() - i });

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
			(float)quad.m_index / index,
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

		m_vertexArray.bind();
		m_vertexArray.addBuffer(m_instanceBuffer, 0, 2, sizeof(UIInstanceData), group.m_offset + 0 * sizeof(float));	// Position
		m_vertexArray.addBuffer(m_instanceBuffer, 1, 1, sizeof(UIInstanceData), group.m_offset + 2 * sizeof(float));	// Rotation
		m_vertexArray.addBuffer(m_instanceBuffer, 2, 2, sizeof(UIInstanceData), group.m_offset + 3 * sizeof(float));	// Size
		m_vertexArray.addBuffer(m_instanceBuffer, 3, 2, sizeof(UIInstanceData), group.m_offset + 5 * sizeof(float));	// Origin
		m_vertexArray.addBuffer(m_instanceBuffer, 4, 4, sizeof(UIInstanceData), group.m_offset + 7 * sizeof(float));	// Color
		m_vertexArray.addBuffer(m_instanceBuffer, 5, 4, sizeof(UIInstanceData), group.m_offset + 11 * sizeof(float));	// Texture rect
		m_vertexArray.addBuffer(m_instanceBuffer, 6, 1, sizeof(UIInstanceData), group.m_offset + 15 * sizeof(float));	// Index

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
				renderData.push_back(UIRenderData{ element->getTexture(), 0, 0 });
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
			for (Uint32 i = start; i < quads.size(); ++i)
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


}