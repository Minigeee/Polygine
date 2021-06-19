#include <poly/Core/Logger.h>

#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Image.h>
#include <poly/Graphics/Window.h>

// Tell GLFW to not include opengl
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <Windows.h>
#endif
#include <GLFW/glfw3native.h>

#define WINDOW_CAST(x) reinterpret_cast<GLFWwindow*>(x)
#define CURSOR_CAST(x) reinterpret_cast<GLFWcursor*>(x)

namespace poly
{


///////////////////////////////////////////////////////////
Window* Window::s_current = 0;

///////////////////////////////////////////////////////////
Uint32 Window::numWindows = 0;

///////////////////////////////////////////////////////////
void* Window::s_standardCursors[6] = { 0, 0, 0, 0, 0, 0 };


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
	m_window			(0),
	m_cursor			(0),
	m_isVsyncEnabled	(true)
{

}


///////////////////////////////////////////////////////////
Window::Window(Uint32 w, Uint32 h, const std::string& title, bool fullscreen, int multisample) :
	m_window			(0),
	m_cursor			(0),
	m_isVsyncEnabled	(true)
{
	create(w, h, title, fullscreen, multisample);
}


///////////////////////////////////////////////////////////
Window::Window(Window&& other) :
	m_window			(other.m_window),
	m_title				(std::move(other.m_title)),
	m_isVsyncEnabled	(other.m_isVsyncEnabled)
{
	other.m_window = 0;

	// Update window pointer
	glfwSetWindowUserPointer(WINDOW_CAST(m_window), this);
}


///////////////////////////////////////////////////////////
Window& Window::operator=(Window&& other)
{
	if (&other != this)
	{
		m_window = other.m_window;
		m_title = std::move(other.m_title);
		m_isVsyncEnabled = other.m_isVsyncEnabled;

		other.m_window = 0;

		// Update window pointer
		glfwSetWindowUserPointer(WINDOW_CAST(m_window), this);

		glfwSwapInterval(m_isVsyncEnabled ? 1 : 0);
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
	GLFWwindow* window = glfwCreateWindow(w, h, title.c_str(), monitor, 0);
	m_window = window;
	if (!m_window)
	{
		LOG_ERROR("Failed to create window");

		// Terminate GLFW if there are no other windows
		if (!numWindows)
			glfwTerminate();

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
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, onKeyEvent);
	glfwSetMouseButtonCallback(window, onMouseButton);
	glfwSetCursorPosCallback(window, onMouseMove);
	glfwSetScrollCallback(window, onMouseScroll);
	glfwSetCharCallback(window, onTextInput);
	glfwSetWindowSizeCallback(window, onWindowResize);

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
	return (bool)m_window && !glfwWindowShouldClose(WINDOW_CAST(m_window));
}


///////////////////////////////////////////////////////////
void Window::close()
{
	if (m_window)
	{
		// Destroy window
		glfwDestroyWindow(WINDOW_CAST(m_window));

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

	glfwSwapBuffers(WINDOW_CAST(m_window));
}


///////////////////////////////////////////////////////////
void Window::setResolution(Uint32 w, Uint32 h)
{
	// Check if window is open
	if (!m_window) return;

	glfwSetWindowSize(WINDOW_CAST(m_window), w, h);
}


///////////////////////////////////////////////////////////
void Window::setResolution(const Vector2u& resolution)
{
	// Check if window is open
	if (!m_window) return;

	glfwSetWindowSize(WINDOW_CAST(m_window), resolution.x, resolution.y);
}


///////////////////////////////////////////////////////////
void Window::setTitle(const std::string& title)
{
	// Check if window is open
	if (!m_window) return;

	m_title = title;
	glfwSetWindowTitle(WINDOW_CAST(m_window), title.c_str());
}


///////////////////////////////////////////////////////////
void Window::setIcon(Image* icon)
{
	if (!icon)
		glfwSetWindowIcon(WINDOW_CAST(m_window), 0, 0);

	else
	{
		GLFWimage image;
		image.width = icon->getWidth();
		image.height = icon->getHeight();
		image.pixels = (Uint8*)icon->getData();

		glfwSetWindowIcon(WINDOW_CAST(m_window), 1, &image);
	}
}


///////////////////////////////////////////////////////////
void Window::setCursor(Cursor cursor)
{
	// Check if the cursor has been loaded yet
	void*& loadedCursor = s_standardCursors[(int)cursor - (int)Cursor::Arrow];
	if (cursor != Cursor::Arrow && !loadedCursor)
		loadedCursor = glfwCreateStandardCursor((int)cursor);

	// Set cursor
	m_cursor = loadedCursor;
	glfwSetCursor(WINDOW_CAST(m_window), CURSOR_CAST(m_cursor));
}


///////////////////////////////////////////////////////////
void Window::setCursorMode(CursorMode mode)
{
	// Change input mode
	glfwSetInputMode(WINDOW_CAST(m_window), GLFW_CURSOR, (int)mode);
}


///////////////////////////////////////////////////////////
void Window::setClipboard(const std::string& str)
{
	glfwSetClipboardString(WINDOW_CAST(m_window), str.c_str());
}


///////////////////////////////////////////////////////////
void Window::setVsyncEnabled(bool enabled)
{
	m_isVsyncEnabled = enabled;
	glfwSwapInterval(m_isVsyncEnabled ? 1 : 0);
}


///////////////////////////////////////////////////////////
WindowHandle Window::getNativeHandle() const
{
#ifdef WIN32
	return glfwGetWin32Window(WINDOW_CAST(m_window));
#endif
}


///////////////////////////////////////////////////////////
Vector2u Window::getResolution() const
{
	// Check if window is open
	if (!m_window) return Vector2u(0);

	int w, h;
	glfwGetWindowSize(WINDOW_CAST(m_window), &w, &h);
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
	return std::string(glfwGetClipboardString(WINDOW_CAST(m_window)));
}


///////////////////////////////////////////////////////////
bool Window::isVsyncEnabled() const
{
	return m_isVsyncEnabled;
}


///////////////////////////////////////////////////////////
Vector2f Window::getCursorPos() const
{
	return m_cursorPos;
}


///////////////////////////////////////////////////////////
bool Window::isKeyPressed(Keyboard key) const
{
	int state = glfwGetKey(WINDOW_CAST(m_window), (int)key);
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


///////////////////////////////////////////////////////////
bool Window::hasContext()
{
	return (bool)glfwGetCurrentContext();
}


}