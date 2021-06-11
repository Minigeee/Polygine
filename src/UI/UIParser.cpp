#include <poly/Graphics/Texture.h>

#include <poly/UI/Button.h>
#include <poly/UI/Dropdown.h>
#include <poly/UI/Font.h>
#include <poly/UI/ListView.h>
#include <poly/UI/ScrollView.h>
#include <poly/UI/Slider.h>
#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>
#include <poly/UI/UIParser.h>

namespace poly
{


///////////////////////////////////////////////////////////
HashMap<std::string, Font*> UIParser::s_fonts;

///////////////////////////////////////////////////////////
HashMap<std::string, Texture*> UIParser::s_textures;

///////////////////////////////////////////////////////////
HashMap<std::string, Shader*> UIParser::s_shaders;

///////////////////////////////////////////////////////////
HashMap<std::string, UIParser::CallbackData> UIParser::s_funcs;

///////////////////////////////////////////////////////////
HashMap<std::string, Vector3u> g_namedColors;


///////////////////////////////////////////////////////////
bool getNamedColor(const std::string& color, Vector4f& out)
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

		LOG("%d", g_namedColors.size());
	}

	auto it = g_namedColors.find(color);
	if (it != g_namedColors.end())
	{
		const Vector3u& c = it->second;
		out = Vector4f(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0f);

		return true;
	}

	return false;
}


///////////////////////////////////////////////////////////
bool UIParser::parse(XmlNode node, UIElement*& out)
{
	if (strcmp(node.getName(), "ui_element") == 0)
		out = Pool<UIElement>::alloc();

	else if (strcmp(node.getName(), "button") == 0)
		out = Pool<Button>::alloc();

	else if (strcmp(node.getName(), "dropdown") == 0)
		out = Pool<Dropdown>::alloc();

	else if (strcmp(node.getName(), "h_list_view") == 0)
		out = Pool<HListView>::alloc();

	else if (strcmp(node.getName(), "v_list_view") == 0 || strcmp(node.getName(), "list_view") == 0)
		out = Pool<VListView>::alloc();

	else if (strcmp(node.getName(), "scroll_view") == 0)
		out = Pool<ScrollView>::alloc();

	else if (strcmp(node.getName(), "slider") == 0)
		out = Pool<Slider>::alloc();

	else if (strcmp(node.getName(), "text") == 0)
		out = Pool<Text>::alloc();

	else if (strcmp(node.getName(), "text_input") == 0)
		out = Pool<TextInput>::alloc();

	else
		return false;

	return true;
}


///////////////////////////////////////////////////////////
bool UIParser::parse(XmlAttribute attr, bool& out)
{
	std::string value = attr.getValue();
	std::transform(value.begin(), value.end(), value.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (value == "true")
	{
		out = true;
		return true;
	}
	else if (value == "false")
	{
		out = false;
		return true;
	}
	else
		return false;
}


///////////////////////////////////////////////////////////
bool UIParser::parse(XmlAttribute attr, int& v)
{
	char* c = attr.getValue();

	char* e;
	v = std::strtoul(c, &e, 10);

	return *e == 0;
}


///////////////////////////////////////////////////////////
bool UIParser::parse(XmlAttribute attr, float& v)
{
	char* c = attr.getValue();

	char* e;
	v = std::strtof(c, &e);

	return *e == 0;
}


///////////////////////////////////////////////////////////
bool UIParser::parse(XmlAttribute attr, Vector2f& out)
{
	char* c = attr.getValue();

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

			// Reset separator
			*c = ' ';

			// Catch errors
			if (*e != ' ')
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
bool UIParser::parse(XmlAttribute attr, Vector3f& out)
{
	char* c = attr.getValue();

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

			// Reset separator
			*c = ' ';

			// Catch errors
			if (*e != ' ')
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
bool UIParser::parse(XmlAttribute attr, Vector4f& out)
{
	char* c = attr.getValue();

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

			// Reset separator
			*c = ' ';

			// Catch errors
			if (*e != ' ')
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
bool UIParser::parse(XmlNode node, Font*& out)
{
	std::string fname;

	XmlAttribute fileAttr = node.getFirstAttribute("file");
	if (fileAttr.exists())
		fname = fileAttr.getValue();

	// A file name must exist
	if (!fname.size())
		return false;

	// Check if the font has been loaded yet
	auto it = s_fonts.find(fname);
	if (it == s_fonts.end())
	{
		Font* font = Pool<Font>::alloc();
		if (font->load(fname))
		{
			out = font;

			// Add to loaded fonts
			s_fonts[fname] = font;
		}
		else
		{
			Pool<Font>::free(font);
			return false;
		}
	}
	else
		out = it->second;

	return true;
}


///////////////////////////////////////////////////////////
bool UIParser::parse(XmlAttribute attr, Texture*& out)
{
	std::string fname = attr.getValue();

	auto it = s_textures.find(fname);
	if (it == s_textures.end())
	{
		Texture* texture = Pool<Texture>::alloc();
		if (texture->load(fname))
		{
			out = texture;

			// Add to loaded textures
			s_textures[fname] = texture;
		}
		else
		{
			Pool<Texture>::free(texture);
			return false;
		}
	}
	else
		out = it->second;

	return true;
}


///////////////////////////////////////////////////////////
bool UIParser::parse(XmlNode node, Shader*& out)
{
	std::string vert, geom, frag;

	XmlAttribute vertAttr = node.getFirstAttribute("vert");
	if (vertAttr.exists())
		vert = vertAttr.getValue();
	else
		// The vertex shader is the one required file
		return false;

	XmlAttribute geomAttr = node.getFirstAttribute("geom");
	if (geomAttr.exists())
		geom = geomAttr.getValue();

	XmlAttribute fragAttr = node.getFirstAttribute("frag");
	if (fragAttr.exists())
		frag = fragAttr.getValue();

	std::string combined = vert + ';' + geom + ';' + frag;

	// Check if the font has been loaded yet
	auto it = s_shaders.find(combined);
	if (it == s_shaders.end())
	{
		Shader* shader = Pool<Shader>::alloc();

		if (vert.size() && !shader->load(vert, Shader::Vertex))
		{
			Pool<Shader>::free(shader);
			return false;
		}

		if (geom.size() && !shader->load(geom, Shader::Geometry))
		{
			Pool<Shader>::free(shader);
			return false;
		}

		if (frag.size() && !shader->load(frag, Shader::Fragment))
		{
			Pool<Shader>::free(shader);
			return false;
		}

		shader->compile();

		// Set output and add to map
		out = shader;
		s_shaders[combined] = shader;
	}
	else
		out = it->second;

	return true;
}


///////////////////////////////////////////////////////////
bool UIParser::parse(XmlAttribute attr, UIPosition& pos)
{
	char* c = attr.getValue();

	if (strcmp(c, "top_left") == 0)
		pos = UIPosition::TopLeft;
	else if (strcmp(c, "top_center") == 0)
		pos = UIPosition::TopCenter;
	else if (strcmp(c, "top_right") == 0)
		pos = UIPosition::TopRight;

	else if (strcmp(c, "left") == 0)
		pos = UIPosition::Left;
	else if (strcmp(c, "center") == 0)
		pos = UIPosition::Center;
	else if (strcmp(c, "right") == 0)
		pos = UIPosition::Right;

	else if (strcmp(c, "bot_left") == 0)
		pos = UIPosition::BotLeft;
	else if (strcmp(c, "bot_center") == 0)
		pos = UIPosition::BotCenter;
	else if (strcmp(c, "bot_right") == 0)
		pos = UIPosition::BotRight;

	else
		return false;

	return true;
}


///////////////////////////////////////////////////////////
bool UIParser::parseColor(XmlAttribute attr, Vector4f& out)
{
	char* c = attr.getValue();
	Vector4f color4;
	Vector3f color3;

	// Hex color
	if (c[0] == '#')
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

	// Color with alpha
	else if (parse(attr, color4))
	{
		out = color4;
		return true;
	}

	else if (parse(attr, color3))
	{
		out = Vector4f(color3, 1.0f);
		return true;
	}

	else if (getNamedColor(attr.getValue(), color4))
	{
		out = color4;
		return true;
	}

	return false;
}


}