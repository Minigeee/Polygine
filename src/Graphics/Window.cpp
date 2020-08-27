#include <poly/Core/Logger.h>

#include <poly/Graphics/Window.h>

#include <glad/glad.h>


namespace poly
{

///////////////////////////////////////////////////////////

void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->sendEvent(E_KeyEvent((Keyboard)key, (InputAction)action));
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->sendEvent(E_MouseButton((Mouse)button, (InputAction)action));
}

void onMouseMove(GLFWwindow* window, double x, double y)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->sendEvent(E_MouseMove((float)x, (float)y));
}

void onMouseScroll(GLFWwindow* window, double dx, double dy)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->sendEvent(E_MouseScroll((float)dx, (float)dy));
}

///////////////////////////////////////////////////////////

Uint32 Window::numWindows = 0;

Window::Window() :
	m_window	(0)
{

}

Window::Window(Window&& other) :
	m_window	(other.m_window),
	m_title		(std::move(other.m_title))
{
	other.m_window = 0;

	// Update window pointer
	glfwSetWindowUserPointer(m_window, this);

}

Window& Window::operator=(Window&& other)
{
	if (&other != this)
	{
		m_window = other.m_window;
		m_title = std::move(other.m_title);

		other.m_window = 0;

		// Update window pointer
		glfwSetWindowUserPointer(m_window, this);
	}

	return *this;
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

	// Setup input callbacks
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, onKeyEvent);
	glfwSetMouseButtonCallback(m_window, onMouseButton);
	glfwSetCursorPosCallback(m_window, onMouseMove);
	glfwSetScrollCallback(m_window, onMouseScroll);

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