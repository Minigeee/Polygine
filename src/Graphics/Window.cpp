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
		// If this is the last window, terminate GLFW
		glfwTerminate();

	else
		// Otherwise, just close the window
		close();
}

///////////////////////////////////////////////////////////

bool Window::create(Uint32 w, Uint32 h, const std::string& title, bool fullscreen)
{
	// Don't create if window is already open
	if (m_window) return false;

	LOG("Creating new window");
	LOG("Window resolution: %dx%d", w, h);
	LOG("Window title: \"%s\"", title.c_str());

	// Initialize GLFW
	if (!numWindows)
		glfwInit();

	// Set window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : 0;
	m_window = glfwCreateWindow(w, h, title.c_str(), monitor, 0);
	if (!m_window)
	{
		LOG_ERROR("Failed to create window");

		// Terminate GLFW if there are no other windows
		if (!numWindows)
			glfwTerminate();

		return false;
	}
	glfwMakeContextCurrent(m_window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initalize GLAD");
		return false;
	}

	// Enable vsync by default
	glfwSwapInterval(1);

	// Update member variables
	m_title = title;
	++numWindows;

	return true;
}

///////////////////////////////////////////////////////////

bool Window::isOpen() const
{
	// The pointer exists, the window is open
	return (bool)m_window && !glfwWindowShouldClose(m_window);
}

void Window::close()
{
	if (m_window)
	{
		// Destroy window
		glfwDestroyWindow(m_window);

		// Reset pointer
		m_window = 0;
	}
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::display()
{
	glfwSwapBuffers(m_window);
}

///////////////////////////////////////////////////////////

void Window::setResolution(Uint32 w, Uint32 h)
{
	glfwSetWindowSize(m_window, w, h);
}

void Window::setResolution(const Vector2u& resolution)
{
	glfwSetWindowSize(m_window, resolution.x, resolution.y);
}

void Window::setTitle(const std::string& title)
{
	m_title = title;
	glfwSetWindowTitle(m_window, title.c_str());
}

///////////////////////////////////////////////////////////

Vector2u Window::getResolution() const
{
	int w, h;
	glfwGetWindowSize(m_window, &w, &h);
	return Vector2u(w, h);
}

const std::string& Window::getTitle() const
{
	return m_title;
}

///////////////////////////////////////////////////////////

}