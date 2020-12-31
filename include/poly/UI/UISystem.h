#ifndef POLY_UI_SYSTEM_H
#define POLY_UI_SYSTEM_H

#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <poly/UI/UIElement.h>

namespace poly
{


///////////////////////////////////////////////////////////
struct UIRenderData
{
	Texture* m_texture;
	Uint32 m_offset;
	Uint32 m_instances;
};


///////////////////////////////////////////////////////////
struct UIInstanceData
{
	Vector2f m_position;
	float m_rotation;
	Vector2f m_size;
	Vector2f m_origin;
	Vector4f m_color;
	Vector4f m_textureRect;
	float m_index;
};


///////////////////////////////////////////////////////////
/// \brief TODO
///
///////////////////////////////////////////////////////////
class UISystem :
	public UIElement
{
public:
	UISystem();

	UISystem(Uint32 w, Uint32 h);

	void update(float dt);

	void render(FrameBuffer& target);

private:
	void getRenderQuads(
		UIElement* element,
		std::vector<std::vector<UIQuad>>& quads,
		std::vector<UIRenderData>& renderData,
		std::vector<UIQuad>& transparentData,
		Uint32& index
	);

	Shader& getShader();

	static Shader s_shader;

private:
	VertexArray m_vertexArray;
	VertexBuffer m_instanceBuffer;						//!< The instance buffer that stores instance transform data
	Uint32 m_instanceBufferOffset;						//!< The offset of the valid range of the instance buffer
};

}

#endif