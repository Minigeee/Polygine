#include <poly/Core/Logger.h>

#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Window.h>


namespace poly
{


///////////////////////////////////////////////////////////
Window* Window::s_current = 0;

///////////////////////////////////////////////////////////
Uint32 Window::numWindows = 0;

///////////////////////////////////////////////////////////
GLFWcursor* Window::s_standardCursors[6] = { 0, 0, 0, 0, 0, 0 };


///////////////////////////////////////////////////////////
void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	Window::setCurrent(win);

	win->sendEvent(E_KeyEvent((Keyboard)key, (InputAction)action));

}


///////////////////////////////////////////////////////////
void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	Window::setCurrent(win);

	win->sendEvent(E_MouseButton((Mouse)button, (InputAction)action));
}


///////////////////////////////////////////////////////////
void onMouseMove(GLFWwindow* window, double x, double y)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	Window::setCurrent(win);

	win->sendEvent(E_MouseMove((float)x, (float)y));
}


///////////////////////////////////////////////////////////
void onMouseScroll(GLFWwindow* window, double dx, double dy)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	Window::setCurrent(win);

	win->sendEvent(E_MouseScroll((float)dx, (float)dy));
}


///////////////////////////////////////////////////////////
void onTextInput(GLFWwindow* window, Uint32 c)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	Window::setCurrent(win);

	win->sendEvent(E_TextInput(c));
}


///////////////////////////////////////////////////////////
void onWindowResize(GLFWwindow* window, int w, int h)
{
	Window* win = (Window*)glfwGetWindowUserPointer(window);
	win->sendEvent(E_WindowResize((Uint32)w, (Uint32)h));
}


///////////////////////////////////////////////////////////
E_WindowResize::E_WindowResize(Uint32 w, Uint32 h) :
	m_width		(w),
	m_height	(h)
{

}


///////////////////////////////////////////////////////////
Window::Window() :
	m_window(0),
	m_cursor(0)
{

}


///////////////////////////////////////////////////////////
Window::Window(Uint32 w, Uint32 h, const std::string& title, bool fullscreen, int multisample) :
	m_window(0),
	m_cursor(0)
{
	create(w, h, title, fullscreen, multisample);
}


///////////////////////////////////////////////////////////
Window::Window(Window&& other) :
	m_window(other.m_window),
	m_title(std::move(other.m_title))
{
	other.m_window = 0;

	// Update window pointer
	glfwSetWindowUserPointer(m_window, this);

}


///////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////
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
bool Window::create(Uint32 w, Uint32 h, const std::string& title, bool fullscreen, int multisample)
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

	// Msaa options
	if (multisample > 1)
		glfwWindowHint(GLFW_SAMPLES, multisample);

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

	// Msaa options
	if (multisample > 1)
		glCheck(glEnable(GL_MULTISAMPLE));

	// Update member variables
	m_title = title;
	++numWindows;

	// Update default framebuffer
	FrameBuffer::Default.m_size.x = w;
	FrameBuffer::Default.m_size.y = h;
	glCheck(glViewport(0, 0, w, h));

	// Setup input callbacks
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, onKeyEvent);
	glfwSetMouseButtonCallback(m_window, onMouseButton);
	glfwSetCursorPosCallback(m_window, onMouseMove);
	glfwSetScrollCallback(m_window, onMouseScroll);
	glfwSetCharCallback(m_window, onTextInput);
	glfwSetWindowSizeCallback(m_window, onWindowResize);

	// glfwGetCursorPos() is buggy
	addListener<E_MouseMove>([&](const E_MouseMove& e) { m_cursorPos = Vector2f(e.m_x, e.m_y); });

	// Resize default framebuffer on window resize
	addListener<E_WindowResize>(
		[&](const E_WindowResize& e)
		{
			FrameBuffer::Default.m_size.x = e.m_width;
			FrameBuffer::Default.m_size.y = e.m_height;
		}
	);

	return true;
}


///////////////////////////////////////////////////////////
bool Window::isOpen() const
{
	// The pointer exists, the window is open
	return (bool)m_window && !glfwWindowShouldClose(m_window);
}


///////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////
void Window::pollEvents()
{
	glfwPollEvents();
}


///////////////////////////////////////////////////////////
void Window::display()
{
	// Check if window is open
	if (!m_window) return;

	glfwSwapBuffers(m_window);
}


///////////////////////////////////////////////////////////
void Window::setResolution(Uint32 w, Uint32 h)
{
	// Check if window is open
	if (!m_window) return;

	glfwSetWindowSize(m_window, w, h);
}


///////////////////////////////////////////////////////////
void Window::setResolution(const Vector2u& resolution)
{
	// Check if window is open
	if (!m_window) return;

	glfwSetWindowSize(m_window, resolution.x, resolution.y);
}


///////////////////////////////////////////////////////////
void Window::setTitle(const std::string& title)
{
	// Check if window is open
	if (!m_window) return;

	m_title = title;
	glfwSetWindowTitle(m_window, title.c_str());
}


///////////////////////////////////////////////////////////
void Window::setCursor(Cursor cursor)
{
	// Check if the cursor has been loaded yet
	GLFWcursor*& loadedCursor = s_standardCursors[(int)cursor - (int)Cursor::Arrow];
	if (cursor != Cursor::Arrow && !loadedCursor)
		loadedCursor = glfwCreateStandardCursor((int)cursor);

	// Set cursor
	m_cursor = loadedCursor;
	glfwSetCursor(m_window, m_cursor);
}


///////////////////////////////////////////////////////////
void Window::setCursorMode(CursorMode mode)
{
	// Change input mode
	glfwSetInputMode(m_window, GLFW_CURSOR, (int)mode);
}


///////////////////////////////////////////////////////////
void Window::setClipboard(const std::string& str)
{
	glfwSetClipboardString(m_window, str.c_str());
}


///////////////////////////////////////////////////////////
Vector2u Window::getResolution() const
{
	// Check if window is open
	if (!m_window) return Vector2u(0);

	int w, h;
	glfwGetWindowSize(m_window, &w, &h);
	return Vector2u(w, h);
}


///////////////////////////////////////////////////////////
const std::string& Window::getTitle() const
{
	return m_title;
}


///////////////////////////////////////////////////////////
std::string Window::getClipboard() const
{
	return std::string(glfwGetClipboardString(m_window));
}


///////////////////////////////////////////////////////////
Vector2f Window::getCursorPos() const
{
	return m_cursorPos;
}


///////////////////////////////////////////////////////////
bool Window::isKeyPressed(Keyboard key) const
{
	int state = glfwGetKey(m_window, (int)key);
	return state != GLFW_RELEASE;
}


///////////////////////////////////////////////////////////
void Window::setCurrent(Window* window)
{
	s_current = window;
}


///////////////////////////////////////////////////////////
Window* Window::getCurrent()
{
	return s_current;
}


}