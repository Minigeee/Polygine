#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/GLCheck.h>

namespace poly
{

namespace priv
{

///////////////////////////////////////////////////////////
Uint32 drawBuffers[] =
{
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,
	GL_COLOR_ATTACHMENT9,
	GL_COLOR_ATTACHMENT10,
	GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12,
	GL_COLOR_ATTACHMENT13,
	GL_COLOR_ATTACHMENT14,
	GL_COLOR_ATTACHMENT15,
	GL_COLOR_ATTACHMENT16,
	GL_COLOR_ATTACHMENT17,
	GL_COLOR_ATTACHMENT18,
	GL_COLOR_ATTACHMENT19
};

}


///////////////////////////////////////////////////////////
FrameBuffer FrameBuffer::Default;

///////////////////////////////////////////////////////////
Uint32 FrameBuffer::currentBound = 0;


///////////////////////////////////////////////////////////
FrameBuffer::FrameBuffer() :
	m_id			(0),
	m_size			(0),
	m_multisampled	(false),
	m_depthTexture	(0),
	m_depthId		(0)
{

}


///////////////////////////////////////////////////////////
FrameBuffer::~FrameBuffer()
{
	if (m_id)
		glCheck(glDeleteFramebuffers(1, &m_id));

	if (m_colorIds.size())
		glCheck(glDeleteRenderbuffers(m_colorIds.size(), &m_colorIds[0]));

	if (m_depthId)
		glCheck(glDeleteRenderbuffers(1, &m_depthId));

	m_id = 0;
	m_depthTexture = 0;
	m_depthId = 0;
}


///////////////////////////////////////////////////////////
void FrameBuffer::bind(Uint32 z)
{
	if (currentBound != m_id)
	{
		// Bind framebuffer
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_id));

		// Change viewport size
		glCheck(glViewport(0, 0, m_size.x, m_size.y));

		currentBound = m_id;
	}

	// Bind the correct z-slice if 3d framebuffer
	if (m_size.z)
	{
		for (Uint32 i = 0; i < m_colorTextures.size(); ++i)
		{
			Texture* texture = m_colorTextures[i];
			texture->bind();
			glCheck(glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_3D, texture->getId(), 0, z));
		}

		if (m_depthTexture)
		{
			m_depthTexture->bind();
			glCheck(glFramebufferTexture3D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_3D, m_depthTexture->getId(), 0, z));
		}
	}
}


///////////////////////////////////////////////////////////
void FrameBuffer::create(Uint32 w, Uint32 h, Uint32 d, bool multisampled)
{
	// Only create framebuffer if it doesn't exist already
	if (!m_id)
	{
		glCheck(glGenFramebuffers(1, &m_id));

		m_size = Vector3u(w, h, d);
		m_multisampled = multisampled;
	}
}


///////////////////////////////////////////////////////////
void FrameBuffer::attachColor(Texture* texture, PixelFormat fmt, GLType dtype, TextureFilter filter, TextureWrap wrap)
{
	// Bind the framebuffer
	if (m_id && currentBound == m_id)
		bind();

	// Get the color attachment index
	Uint32 index = m_colorTextures.size() + m_colorIds.size();

	// Decide to use texture or renderbuffer attachment
	if (texture)
	{
		// Create an empty texture
		texture->create(0, fmt, m_size.x, m_size.y, m_size.z, dtype, filter, wrap, m_multisampled);

		// Attach to color attachment target using correct number of dimensions
		if (m_size.z == 0)
		{
			Uint32 target = m_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target, texture->getId(), 0));
		}
		else
			glCheck(glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_3D, texture->getId(), 0, 0));

		// Add the color texture
		m_colorTextures.push_back(texture);
	}
	else
	{
		// Create a renderbuffer
		Uint32 id = 0;
		glCheck(glGenRenderbuffers(1, &id));
		glCheck(glBindRenderbuffer(GL_RENDERBUFFER, id));

		if (m_multisampled)
			glCheck(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, (GLenum)fmt, m_size.x, m_size.y));
		else
			glCheck(glRenderbufferStorage(GL_RENDERBUFFER, (GLenum)fmt, m_size.x, m_size.y));

		glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, id));

		// Add renderbuffer id
		m_colorIds.push_back(id);
	}

	// Specify color attachments to draw
	glDrawBuffers(m_colorTextures.size() + m_colorIds.size(), priv::drawBuffers);
}


///////////////////////////////////////////////////////////
void FrameBuffer::attachDepth(Texture* texture, GLType dtype, TextureFilter filter, TextureWrap wrap)
{
	// Bind the framebuffer
	if (m_id && currentBound == m_id)
		bind();

	// Decide to use texture or renderbuffer attachment
	if (texture)
	{
		// Create an empty texture
		texture->create(0, PixelFormat::Depth, m_size.x, m_size.y, m_size.z, dtype, filter, wrap, m_multisampled);

		// Attach to depth attachment target using correct number of dimensions
		if (m_size.z == 0)
		{
			Uint32 target = m_multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, texture->getId(), 0));
		}
		else
			glCheck(glFramebufferTexture3D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_3D, texture->getId(), 0, 0));

		// Set the depth texture
		m_depthTexture = texture;
	}
	else
	{
		// Create a renderbuffer
		Uint32 id = 0;
		glCheck(glGenRenderbuffers(1, &id));
		glCheck(glBindRenderbuffer(GL_RENDERBUFFER, id));

		if (m_multisampled)
			glCheck(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, m_size.x, m_size.y));
		else
			glCheck(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_size.x, m_size.y));

		glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id));

		// Set depth renderbuffer
		m_depthId = id;
	}
}


///////////////////////////////////////////////////////////
void FrameBuffer::blitTo(FrameBuffer& target)
{
	ASSERT(m_id, "Can't blit a framebuffer using an uninitialized source framebuffer");
	ASSERT(target.m_id, "Can't blit a framebuffer using an uninitialized source framebuffer");

	// Bind the framebuffers
	glCheck(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id));
	glCheck(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.m_id));
	currentBound = target.m_id;

	// Get buffer bits
	Uint32 flags = 0;
	if (m_colorTextures.size() || m_colorIds.size())
		flags |= GL_COLOR_BUFFER_BIT;
	if (m_depthTexture || m_depthId)
		flags |= GL_DEPTH_BUFFER_BIT;

	// Blit the framebuffer
	glCheck(glBlitFramebuffer(0, 0, m_size.x, m_size.y, 0, 0, target.m_size.x, target.m_size.y, flags, GL_NEAREST));
}


///////////////////////////////////////////////////////////
Uint32 FrameBuffer::getId() const
{
	return m_id;
}


///////////////////////////////////////////////////////////
Uint32 FrameBuffer::getWidth() const
{
	return m_size.x;
}


///////////////////////////////////////////////////////////
Uint32 FrameBuffer::getHeight() const
{
	return m_size.y;
}


///////////////////////////////////////////////////////////
Uint32 FrameBuffer::getDepth() const
{
	return m_size.z;
}


///////////////////////////////////////////////////////////
bool FrameBuffer::isMultisampled() const
{
	return m_multisampled;
}


///////////////////////////////////////////////////////////
Uint32 FrameBuffer::getNumColorTextures() const
{
	return m_colorTextures.size();
}


///////////////////////////////////////////////////////////
Texture* FrameBuffer::getColorTexture(Uint32 index) const
{
	return m_colorTextures[index];
}


///////////////////////////////////////////////////////////
Texture* FrameBuffer::getDepthTexture() const
{
	return m_depthTexture;
}


}