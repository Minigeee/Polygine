#include <poly/Graphics/GLCheck.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
std::string getGLErrorMsg(Uint32 err)
{
	switch (err)
	{
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument";

	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE: A numeric argument is out of range";

	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state";

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete";

	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command";
	}

	return std::to_string(err) + ": Unknown OpenGL error";
}


}

}