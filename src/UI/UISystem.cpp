#include <poly/Core/ObjectPool.h>
#include <poly/Core/Profiler.h>

#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Window.h>

#include <poly/Math/Functions.h>

#include <poly/UI/Button.h>
#include <poly/UI/Dropdown.h>
#include <poly/UI/ListView.h>
#include <poly/UI/ScrollView.h>
#include <poly/UI/Slider.h>
#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>
#include <poly/UI/UISystem.h>

#include <rapidxml.hpp>

#include <stack>

namespace poly
{


///////////////////////////////////////////////////////////
HashMap<std::string, Vector3u> g_namedColors;


///////////////////////////////////////////////////////////
const Vector4f& getNamedColor(const std::string& color)
{
	if (!g_namedColors.size())
	{

		g_namedColors["light_salmon"]				= Vector3u(255, 160, 122);
		g_namedColors["salmon"]						= Vector3u(250, 128, 114);
		g_namedColors["dark_salmon"]				= Vector3u(233, 150, 122);
		g_namedColors["light_coral"]				= Vector3u(240, 128, 128);
		g_namedColors["indian_red"]					= Vector3u(205, 92, 92);
		g_namedColors["crimson"]					= Vector3u(220, 20, 60);
		g_namedColors["firebrick"]					= Vector3u(178, 34, 34);
		g_namedColors["red"]						= Vector3u(255, 0, 0);
		g_namedColors["dark_red"]					= Vector3u(139, 0, 0);

		g_namedColors["coral"]						= Vector3u(255, 127, 80);
		g_namedColors["tomato"]						= Vector3u(255, 99, 71);
		g_namedColors["orange_red"]					= Vector3u(255, 69, 0);
		g_namedColors["gold"]						= Vector3u(255, 215, 0);
		g_namedColors["orange"]						= Vector3u(255, 165, 0);
		g_namedColors["dark_orange"]				= Vector3u(255, 140, 0);

		g_namedColors["light_yellow"]				= Vector3u(255, 255, 224);
		g_namedColors["lemon_chiffon"]				= Vector3u(255, 250, 205);
		g_namedColors["light_goldenrod_yellow"]		= Vector3u(250, 250, 210);
		g_namedColors["papaya_whip"]				= Vector3u(255, 239, 213);
		g_namedColors["moccasin"]					= Vector3u(255, 228, 181);
		g_namedColors["peach_puff"]					= Vector3u(255, 218, 185);
		g_namedColors["pale_goldenrod"]				= Vector3u(238, 232, 170);
		g_namedColors["khaki"]						= Vector3u(240, 230, 140);
		g_namedColors["dark_khaki"]					= Vector3u(189, 183, 107);
		g_namedColors["yellow"]						= Vector3u(255, 255, 0);

		g_namedColors["lawn_green"]					= Vector3u(124, 252, 0);
		g_namedColors["chartreuse"]					= Vector3u(127, 255, 0);
		g_namedColors["lime_green"]					= Vector3u(50, 205, 50);
		g_namedColors["lime"]						= Vector3u(0, 255, 0);
		g_namedColors["forest_green"]				= Vector3u(34, 139, 34);
		g_namedColors["green"]						= Vector3u(0, 128, 0);
		g_namedColors["dark_green"]					= Vector3u(0, 100, 0);
		g_namedColors["green_yellow"]				= Vector3u(173, 255, 47);
		g_namedColors["yellow_green"]				= Vector3u(154, 205, 50);
		g_namedColors["spring_green"]				= Vector3u(0, 255, 127);
		g_namedColors["medium_spring_green"]		= Vector3u(0, 250, 154);
		g_namedColors["light_green"]				= Vector3u(144, 238, 144);
		g_namedColors["pale_green"]					= Vector3u(152, 251, 152);
		g_namedColors["dark_sea_green"]				= Vector3u(143, 188, 143);
		g_namedColors["medium_sea_green"]			= Vector3u(60, 179, 113);
		g_namedColors["sea_green"]					= Vector3u(46, 139, 87);
		g_namedColors["olive"]						= Vector3u(128, 128, 0);
		g_namedColors["dark_olive_green"]			= Vector3u(85, 107, 47);
		g_namedColors["olive_drab"]					= Vector3u(107, 142, 35);

		g_namedColors["light_cyan"]					= Vector3u(224, 255, 255);
		g_namedColors["cyan"]						= Vector3u(0, 255, 255);
		g_namedColors["aqua"]						= Vector3u(0, 255, 255);
		g_namedColors["aquamarine"]					= Vector3u(127, 255, 212);
		g_namedColors["medium_aquamarine"]			= Vector3u(102, 205, 170);
		g_namedColors["pale_turquoise"]				= Vector3u(175, 238, 238);
		g_namedColors["turquoise"]					= Vector3u(64, 224, 208);
		g_namedColors["medium_turquoise"]			= Vector3u(72, 209, 204);
		g_namedColors["dark_turquoise"]				= Vector3u(0, 206, 209);
		g_namedColors["light_sea_green"]			= Vector3u(32, 178, 170);
		g_namedColors["cadet_blue"]					= Vector3u(95, 158, 160);
		g_namedColors["dark_cyan"]					= Vector3u(0, 139, 139);
		g_namedColors["teal"]						= Vector3u(0, 128, 128);

		g_namedColors["poweder_blue"]				= Vector3u(176, 224, 230);
		g_namedColors["light_blue"]					= Vector3u(173, 216, 230);
		g_namedColors["light_sky_blue"]				= Vector3u(135, 206, 250);
		g_namedColors["sky_blue"]					= Vector3u(135, 206, 235);
		g_namedColors["deep_sky_blue"]				= Vector3u(0, 191, 255);
		g_namedColors["light_steel_blue"]			= Vector3u(176, 196, 222);
		g_namedColors["dodger_blue"]				= Vector3u(30, 144, 255);
		g_namedColors["cornflower_blue"]			= Vector3u(100, 149, 237);
		g_namedColors["steel_blue"]					= Vector3u(70, 130, 180);
		g_namedColors["royal_blue"]					= Vector3u(65, 105, 225);
		g_namedColors["blue"]						= Vector3u(0, 0, 255);
		g_namedColors["medium_blue"]				= Vector3u(0, 0, 205);
		g_namedColors["dark_blue"]					= Vector3u(0, 0, 139);
		g_namedColors["navy"]						= Vector3u(0, 0, 128);
		g_namedColors["midnight_blue"]				= Vector3u(25, 25, 112);
		g_namedColors["midnight_slate_blue"]		= Vector3u(123, 104, 238);
		g_namedColors["slate_blue"]					= Vector3u(106, 90, 205);
		g_namedColors["dark_slate_blue"]			= Vector3u(72, 61, 139);

		g_namedColors["lavender"]					= Vector3u(230, 230, 250);
		g_namedColors["thistle"]					= Vector3u(216, 191, 216);
		g_namedColors["plum"]						= Vector3u(221, 160, 221);
		g_namedColors["violet"]						= Vector3u(238, 130, 238);
		g_namedColors["orchid"]						= Vector3u(218, 112, 214);
		g_namedColors["fuchsia"]					= Vector3u(255, 0, 255);
		g_namedColors["magenta"]					= Vector3u(255, 0, 255);
		g_namedColors["medium_orchid"]				= Vector3u(186, 85, 211);
		g_namedColors["medium_purple"]				= Vector3u(147, 112, 219);
		g_namedColors["blue_violet"]				= Vector3u(138, 43, 226);
		g_namedColors["dark_violet"]				= Vector3u(148, 0, 211);
		g_namedColors["dark_orchid"]				= Vector3u(153, 50, 204);
		g_namedColors["dark_magenta"]				= Vector3u(139, 0, 139);
		g_namedColors["purple"]						= Vector3u(128, 0, 128);
		g_namedColors["indigo"]						= Vector3u(75, 0, 130);

		g_namedColors["pink"]						= Vector3u(255, 192, 203);
		g_namedColors["light_pink"]					= Vector3u(255, 182, 193);
		g_namedColors["hot_pink"]					= Vector3u(255, 105, 180);
		g_namedColors["deep_pink"]					= Vector3u(255, 20, 147);
		g_namedColors["pale_violet_red"]			= Vector3u(219, 112, 147);
		g_namedColors["medium_violet_red"]			= Vector3u(199, 21, 133);

		g_namedColors["white"]						= Vector3u(255, 255, 255);
		g_namedColors["snow"]						= Vector3u(255, 250, 250);
		g_namedColors["honeydew"]					= Vector3u(240, 255, 240);
		g_namedColors["mint_cream"]					= Vector3u(245, 255, 250);
		g_namedColors["azure"]						= Vector3u(240, 255, 255);
		g_namedColors["alice_blue"]					= Vector3u(240, 248, 255);
		g_namedColors["ghost_white"]				= Vector3u(248, 248, 255);
		g_namedColors["white_smoke"]				= Vector3u(245, 245, 245);
		g_namedColors["seashell"]					= Vector3u(255, 245, 238);
		g_namedColors["beige"]						= Vector3u(245, 245, 220);
		g_namedColors["old_lace"]					= Vector3u(253, 245, 230);
		g_namedColors["floral_white"]				= Vector3u(255, 250, 240);
		g_namedColors["ivory"]						= Vector3u(255, 255, 240);
		g_namedColors["antique_white"]				= Vector3u(250, 235, 215);
		g_namedColors["linen"]						= Vector3u(250, 240, 230);
		g_namedColors["lavender_blush"]				= Vector3u(255, 240, 245);
		g_namedColors["misty_rose"]					= Vector3u(255, 228, 225);

		g_namedColors["gainsboro"]					= Vector3u(220, 220, 220);
		g_namedColors["light_gray"]					= Vector3u(211, 211, 211);
		g_namedColors["silver"]						= Vector3u(192, 192, 192);
		g_namedColors["dark_gray"]					= Vector3u(169, 169, 169);
		g_namedColors["gray"]						= Vector3u(128, 128, 128);
		g_namedColors["dim_gray"]					= Vector3u(105, 105, 105);
		g_namedColors["light_slate_gray"]			= Vector3u(119, 136, 153);
		g_namedColors["slate_gray"]					= Vector3u(112, 128, 144);
		g_namedColors["dark_slate_gray"]			= Vector3u(47, 79, 79);
		g_namedColors["black"]						= Vector3u(0, 0, 0);

		g_namedColors["cornsilk"]					= Vector3u(255, 248, 220);
		g_namedColors["blanched_almond"]			= Vector3u(255, 235, 205);
		g_namedColors["bisque"]						= Vector3u(255, 228, 196);
		g_namedColors["navajo_white"]				= Vector3u(255, 222, 173);
		g_namedColors["wheat"]						= Vector3u(245, 222, 179);
		g_namedColors["burlywood"]					= Vector3u(222, 184, 135);
		g_namedColors["tan"]						= Vector3u(210, 180, 140);
		g_namedColors["rosy_brown"]					= Vector3u(188, 143, 143);
		g_namedColors["sandy_brown"]				= Vector3u(244, 164, 96);
		g_namedColors["goldenrod"]					= Vector3u(218, 165, 32);
		g_namedColors["peru"]						= Vector3u(205, 133, 63);
		g_namedColors["chocolate"]					= Vector3u(210, 105, 30);
		g_namedColors["saddle_brown"]				= Vector3u(139, 69, 19);
		g_namedColors["sienna"]						= Vector3u(160, 82, 45);
		g_namedColors["brown"]						= Vector3u(165, 42, 42);
		g_namedColors["maroon"]						= Vector3u(128, 0, 0);
	}

	auto it = g_namedColors.find(color);
	if (it != g_namedColors.end())
	{
		const Vector3u& c = it->second;
		return Vector4f(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0f);
	}

	return Vector4f(1.0f);
}


///////////////////////////////////////////////////////////
bool uiXmlParseFloat(char* c, float& v)
{
	char* e;
	v = std::strtof(c, &e);

	return *e != 0;
}


///////////////////////////////////////////////////////////
bool uiXmlParseVec2(char* c, Vector2f& out)
{
	float* v = &out.x;

	Uint32 index = 0;
	char* start = c;

	while (*c != 0)
	{
		if (*c == ' ' || *c == ',')
		{
			// Null terminator
			*c = 0;

			// Parse string
			char* e;
			v[index++] = std::strtof(start, &e);

			// Catch errors
			if (*e != 0)
				return false;

			while (*++c == ' ' || *c == ',');
			start = c;
		}

		++c;
	}

	// Parse last number
	char* e;
	v[index] = std::strtof(start, &e);

	return *e == 0;
}


///////////////////////////////////////////////////////////
bool uiXmlParseVec3(char* c, Vector3f& out)
{
	float* v = &out.x;

	Uint32 index = 0;
	char* start = c;

	while (*c != 0)
	{
		if (*c == ' ' || *c == ',')
		{
			// Null terminator
			*c = 0;

			// Parse string
			char* e;
			v[index++] = std::strtof(start, &e);

			// Catch errors
			if (*e != 0)
				return false;

			while (*++c == ' ' || *c == ',');
			start = c;
		}

		++c;
	}

	// Parse last number
	char* e;
	v[index] = std::strtof(start, &e);

	return *e == 0;
}


///////////////////////////////////////////////////////////
bool uiXmlParseVec4(char* c, Vector4f& out)
{
	float* v = &out.x;

	Uint32 index = 0;
	char* start = c;

	while (*c != 0)
	{
		if (*c == ' ' || *c == ',')
		{
			// Null terminator
			*c = 0;

			// Parse string
			char* e;
			v[index++] = std::strtof(start, &e);

			// Catch errors
			if (*e != 0)
				return false;

			while (*++c == ' ' || *c == ',');
			start = c;
		}

		++c;
	}

	// Parse last number
	char* e;
	v[index] = std::strtof(start, &e);

	return *e == 0;
}


///////////////////////////////////////////////////////////
bool uiXmlParseHexColor(char* c, Vector4f& out)
{
	// Parse hex color
	Uint32 r, g, b, a = 255;

	Uint32 len = strlen(c);
	char* e;
	char temp;

	if (c[0] != '#' || len < 7)
		return false;

	temp = c[3]; c[3] = 0;
	r = std::strtoul(c + 1, &e, 16);
	if (*e != 0) return false;

	c[3] = temp; temp = c[5]; c[5] = 0;
	g = std::strtoul(c + 3, &e, 16);
	if (*e != 0) return false;

	c[5] = temp;
	if (len == 9)
	{
		temp = c[7];
		c[7] = 0;
	}
	b = std::strtoul(c + 5, &e, 16);
	if (*e != 0) return false;

	if (len == 9)
	{
		c[7] = temp;
		a = std::strtoul(c + 7, &e, 16);
		if (*e != 0) return false;
	}

	out.r = r / 255.0f;
	out.g = g / 255.0f;
	out.b = b / 255.0f;
	out.a = a / 255.0f;

	return true;
}


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
UISystem::~UISystem()
{
	for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end(); ++it)
		Pool<Texture>::free(it->second);
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
	// Load file
	std::ifstream f(fname.c_str(), std::ios::binary);
	if (!f.is_open())
	{
		LOG_WARNING("Could not open UI XML file: %s", fname.c_str());
		return false;
	}

	f.seekg(0, std::ios::end);
	Uint32 size = (Uint32)f.tellg();
	f.seekg(0);

	// Allocate and read data
	char* data = (char*)malloc(size + 1);
	f.read(data, size);
	data[size] = 0;

	// Close file
	f.close();

	// Parse the file
	rapidxml::xml_document<> doc;
	doc.parse<0>(data);

	// Get the main node
	rapidxml::xml_node<>* mainNode = doc.first_node("ui");
	if (!mainNode)
	{
		LOG_WARNING("Invalid UI XML file (must start with <ui>): %s", fname.c_str());
		free(data);
		return false;
	}

	// Keep track of parent elements
	std::stack<UIElement*> parentStack;
	std::stack<rapidxml::xml_node<>*> nodeStack;
	parentStack.push(this);
	nodeStack.push(0);

	// Iterate through ui elements
	rapidxml::xml_node<>* current = mainNode->first_node("ui_element");
	while (current)
	{
		// Create an element
		UIElement* element = 0;
		Uint32 type = 0;

		// Check the element type
		rapidxml::xml_attribute<>* typeAttrib = current->first_attribute("type");
		if (!typeAttrib)
		{
			element = Pool<UIElement>::alloc();
			type = TypeInfo::getId<UIElement>();
		}
		else if (strcmp(typeAttrib->value(), "button") == 0)
		{
			element = Pool<Button>::alloc();
			type = TypeInfo::getId<Button>();
		}
		else if (strcmp(typeAttrib->value(), "dropdown") == 0)
		{
			element = Pool<Dropdown>::alloc();
			type = TypeInfo::getId<Dropdown>();
		}
		else if (strcmp(typeAttrib->value(), "h_list_view") == 0)
		{
			element = Pool<HListView>::alloc();
			type = TypeInfo::getId<HListView>();
		}
		else if (strcmp(typeAttrib->value(), "v_list_view") == 0 || strcmp(typeAttrib->value(), "list_view") == 0)
		{
			element = Pool<VListView>::alloc();
			type = TypeInfo::getId<VListView>();
		}
		else if (strcmp(typeAttrib->value(), "scroll_view") == 0)
		{
			element = Pool<ScrollView>::alloc();
			type = TypeInfo::getId<ScrollView>();
		}
		else if (strcmp(typeAttrib->value(), "slider") == 0)
		{
			element = Pool<Slider>::alloc();
			type = TypeInfo::getId<Slider>();
		}
		else if (strcmp(typeAttrib->value(), "text") == 0)
		{
			element = Pool<Text>::alloc();
			type = TypeInfo::getId<Text>();
		}
		else if (strcmp(typeAttrib->value(), "text_input") == 0)
		{
			element = Pool<TextInput>::alloc();
			type = TypeInfo::getId<TextInput>();
		}

		// Check if the element has an id
		rapidxml::xml_attribute<>* idAttrib = current->first_attribute("id");
		std::string id = idAttrib ? idAttrib->value() : "";

		// Position
		rapidxml::xml_attribute<>* posAttr = current->first_attribute("position");
		if (posAttr)
		{
			Vector2f pos;
			if (uiXmlParseVec2(posAttr->value(), pos))
				element->setPosition(pos);
		}

		// Rotation
		rapidxml::xml_attribute<>* rotAttr = current->first_attribute("rotation");
		if (rotAttr)
		{
			float rot;
			if (uiXmlParseFloat(rotAttr->value(), rot))
				element->setRotation(rot);
		}

		// Size
		rapidxml::xml_attribute<>* sizeAttr = current->first_attribute("size");
		if (sizeAttr)
		{
			Vector2f size;
			if (uiXmlParseVec2(sizeAttr->value(), size))
				element->setSize(size);
		}

		// Rel Size
		rapidxml::xml_attribute<>* relSizeAttr = current->first_attribute("rel_size");
		if (relSizeAttr)
		{
			Vector2f size;
			if (uiXmlParseVec2(relSizeAttr->value(), size))
				element->setRelSize(size);
		}

		// Width
		rapidxml::xml_attribute<>* widthAttr = current->first_attribute("width");
		if (widthAttr)
		{
			float width;
			if (uiXmlParseFloat(widthAttr->value(), width))
				element->setWidth(width);
		}

		// Height
		rapidxml::xml_attribute<>* heightAttr = current->first_attribute("height");
		if (heightAttr)
		{
			float height;
			if (uiXmlParseFloat(heightAttr->value(), height))
				element->setHeight(height);
		}

		// Rel Width
		rapidxml::xml_attribute<>* relWidthAttr = current->first_attribute("rel_width");
		if (relWidthAttr)
		{
			float relWidth;
			if (uiXmlParseFloat(relWidthAttr->value(), relWidth))
				element->setRelWidth(relWidth);
		}

		// Rel Height
		rapidxml::xml_attribute<>* relHeightAttr = current->first_attribute("rel_height");
		if (relHeightAttr)
		{
			float relHeight;
			if (uiXmlParseFloat(relHeightAttr->value(), relHeight))
				element->setRelHeight(relHeight);
		}

		// Origin
		rapidxml::xml_attribute<>* originAttr = current->first_attribute("origin");
		if (originAttr)
		{
			Vector2f origin;
			if (uiXmlParseVec2(originAttr->value(), origin))
				element->setOrigin(origin);

			else
			{
				if (strcmp(originAttr->value(), "top_left") == 0)
					element->setOrigin(UIPosition::TopLeft);
				else if (strcmp(originAttr->value(), "top_center") == 0)
					element->setOrigin(UIPosition::TopCenter);
				else if (strcmp(originAttr->value(), "top_right") == 0)
					element->setOrigin(UIPosition::TopRight);

				else if (strcmp(originAttr->value(), "left") == 0)
					element->setOrigin(UIPosition::Left);
				else if (strcmp(originAttr->value(), "center") == 0)
					element->setOrigin(UIPosition::Center);
				else if (strcmp(originAttr->value(), "right") == 0)
					element->setOrigin(UIPosition::Right);

				else if (strcmp(originAttr->value(), "bot_left") == 0)
					element->setOrigin(UIPosition::BotLeft);
				else if (strcmp(originAttr->value(), "bot_center") == 0)
					element->setOrigin(UIPosition::BotCenter);
				else if (strcmp(originAttr->value(), "bot_right") == 0)
					element->setOrigin(UIPosition::BotRight);
			}
		}

		// Anchor
		rapidxml::xml_attribute<>* anchorAttr = current->first_attribute("anchor");
		if (anchorAttr)
		{
			Vector2f anchor;
			if (uiXmlParseVec2(anchorAttr->value(), anchor))
				element->setAnchor(anchor);

			else
			{
				if (strcmp(anchorAttr->value(), "top_left") == 0)
					element->setAnchor(UIPosition::TopLeft);
				else if (strcmp(anchorAttr->value(), "top_center") == 0)
					element->setAnchor(UIPosition::TopCenter);
				else if (strcmp(anchorAttr->value(), "top_right") == 0)
					element->setAnchor(UIPosition::TopRight);

				else if (strcmp(anchorAttr->value(), "left") == 0)
					element->setAnchor(UIPosition::Left);
				else if (strcmp(anchorAttr->value(), "center") == 0)
					element->setAnchor(UIPosition::Center);
				else if (strcmp(anchorAttr->value(), "right") == 0)
					element->setAnchor(UIPosition::Right);

				else if (strcmp(anchorAttr->value(), "bot_left") == 0)
					element->setAnchor(UIPosition::BotLeft);
				else if (strcmp(anchorAttr->value(), "bot_center") == 0)
					element->setAnchor(UIPosition::BotCenter);
				else if (strcmp(anchorAttr->value(), "bot_right") == 0)
					element->setAnchor(UIPosition::BotRight);
			}
		}

		// Color
		rapidxml::xml_attribute<>* colorAttr = current->first_attribute("color");
		if (colorAttr)
		{
			Vector4f color4;
			Vector3f color3;
			char* c = colorAttr->value();

			if (c[0] == '#' && uiXmlParseHexColor(c, color4))
				element->setColor(color4);

			else if (uiXmlParseVec4(c, color4))
				element->setColor(color4);

			else if (uiXmlParseVec3(c, color3))
				element->setColor(Vector4f(color3, 1.0f));

			else
				element->setColor(getNamedColor(c));
		}

		// Texture
		rapidxml::xml_attribute<>* textureAttr = current->first_attribute("texture");
		if (textureAttr)
		{
			Texture* texture = Pool<Texture>::alloc();
			std::string fname = textureAttr->value();

			// Check if it has been loaded first
			auto it = m_loadedTextures.find(fname);
			if (it == m_loadedTextures.end())
			{
				if (texture->load(fname))
				{
					m_loadedTextures[fname] = texture;
					element->setTexture(texture);
				}
			}
			else
				element->setTexture(it->second);
		}

		// Texture Rect
		rapidxml::xml_attribute<>* textureRectAttr = current->first_attribute("texture_rect");
		if (textureRectAttr)
		{
			Vector4f textureRect;
			if (uiXmlParseVec4(textureRectAttr->value(), textureRect))
				element->setTextureRect(textureRect);
		}


		parentStack.top()->addChild(element);

		// Add the element to the map
		if (id.size() > 0)
			m_elements[id] = element;

		// Add child elements
		rapidxml::xml_node<>* childrenNode = current->first_node("ui_element");

		// Go to next sibling
		current = current->next_sibling("ui_element");

		if (childrenNode)
		{
			// Add current node to parent queue
			parentStack.push(element);
			nodeStack.push(current);

			// Go to first child element
			current = childrenNode;
		}

		// Go up one level if the current node is 0
		if (!current && parentStack.size())
		{
			current = nodeStack.top();
			parentStack.pop();
			nodeStack.pop();
		}
	}

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