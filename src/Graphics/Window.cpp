#include <poly/Core/Logger.h>

#include <poly/Graphics/Window.h>

#include <glad/glad.h>

namespace poly
{

Uint32 Window::numWindows = 0;

///////////////////////////////////////////////////////////

Window::Window() :
	m_window	(0)
{

}

Window::~Window()
{
	if (!--numWindows)
		glfwTerminate();
}

///////////////////////////////////////////////////////////

bool Window::create(Uint32 w, Uint32 h, const std::string& title, bool fullscreen)
{
	LOG("Creating new window");
	LOG("Window resolution: %dx%d", w, h);
	LOG("Window title: \"%s\"", title.c_str());

	// Initialize GLFW
	if (!numWindows++)
		glfwInit();

	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* window = glfwCreateWindow(w, h, title.c_str(), monitor, 0);
	if (!window)
	{
		LOG_ERROR("Failed to create window");
		return false;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initalize GLAD");
		return false;
	}

	// Enable vsync by default
	glfwSwapInterval(1);

	return true;
}

///////////////////////////////////////////////////////////

}