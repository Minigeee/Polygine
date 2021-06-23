#ifndef POLY_GL_CHECK_H
#define POLY_GL_CHECK_H

#include <poly/Core/Logger.h>

#include <poly/Graphics/OpenGL.h>
#include <poly/Graphics/Window.h>

#include <string>

namespace poly
{

namespace priv
{

std::string getGLErrorMsg(Uint32 err);

}

#ifndef NDEBUG
#define glCheck(expr) \
do \
{ \
	expr; \
	Uint32 err = glGetError(); \
	if (err != GL_NO_ERROR && Window::isContextActive()) \
		LOG_ERROR("[%s] - %s", STR(expr), poly::priv::getGLErrorMsg(err).c_str()); \
} while (false)
#else
#define glCheck(expr) (expr)
#endif

}

#endif