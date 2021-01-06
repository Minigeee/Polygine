#ifndef POLY_WINDOW_H
#define POLY_WINDOW_H

#include <poly/Core/DataTypes.h>

#include <poly/Engine/Events.h>
#include <poly/Engine/Input.h>

#include <poly/Math/Vector2.h>

// Tell GLFW to not include opengl
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief An enum defining the standard cursor types
///
///////////////////////////////////////////////////////////
enum class Cursor
{
	Arrow		= 0x00036001,	//!< The default arrow cursor
	IBeam		= 0x00036002,	//!< The text input I-beam cursor
	CrossHair	= 0x00036003,	//!< The crosshair cursor
	Hand		= 0x00036004,	//!< The hand shaped cursor
	HResize		= 0x00036005,	//!< The horizontal resize cursor
	VResize		= 0x00036006	//!< The vertical resize cursor
};


///////////////////////////////////////////////////////////
/// \brief An enum defining cursor modes
///
///////////////////////////////////////////////////////////
enum class CursorMode
{
	Normal		= 0x00034001,	//!< The cursor is fully visible and unrestricted cursor
	Hidden		= 0x00034002,	//!< The cursor is not visible and is unrestricted, meaning that it can leave the window
	Disabled	= 0x00034003	//!< The cursor is not visible and restricted, meaning that it has unlimited movement within the window
};


///////////////////////////////////////////////////////////
/// \brief Handles window creation, management, and user input
///
///////////////////////////////////////////////////////////
class Window :
	public EventSystem<E_KeyEvent, E_MouseButton, E_MouseMove, E_MouseScroll, E_TextInput>
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Window();

	///////////////////////////////////////////////////////////
	/// \brief Construct a new window using the given settings
	///
	/// \param w Width of the window in pixels
	/// \param h Height of the window in pixels
	/// \param title The title string
	/// \param fullscreen Should the window be fullscreen or not
	/// \param multisample The multisampling factor, where a factor of 1 would turn off antialiasing
	///
	/// \see create
	///
	///////////////////////////////////////////////////////////
	Window(Uint32 w, Uint32 h, const std::string& title, bool fullscreen = false, int multisample = 1);

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Closes the window, and if it is the last existing window,
	/// GLFW is terminated.
	///
	///////////////////////////////////////////////////////////
	~Window();

#ifndef DOXYGEN_SKIP
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&);
	Window& operator=(Window&&);
#endif

	///////////////////////////////////////////////////////////
	/// \brief Poll events for all windows
	///
	/// This will process and send events for all input listeners.
	///
	///////////////////////////////////////////////////////////
	static void pollEvents();

	///////////////////////////////////////////////////////////
	/// \brief Create a new window using the given settings
	///
	/// \param w Width of the window in pixels
	/// \param h Height of the window in pixels
	/// \param title The title string
	/// \param fullscreen Should the window be fullscreen or not
	/// \param multisample The multisampling factor, where a factor of 1 would turn off antialiasing
	///
	///////////////////////////////////////////////////////////
	bool create(Uint32 w, Uint32 h, const std::string& title, bool fullscreen = false, int multisample = 1);

	///////////////////////////////////////////////////////////
	/// \brief Check if the window is open
	///
	/// \return True if the window is open
	///
	///////////////////////////////////////////////////////////
	bool isOpen() const;

	///////////////////////////////////////////////////////////
	/// \brief Close the window
	///
	/// Use this function to close the window immediately
	///
	///////////////////////////////////////////////////////////
	void close();

	///////////////////////////////////////////////////////////
	/// \brief Display everything that has been rendered
	///
	/// Swaps the front buffer with the back buffer
	///
	///////////////////////////////////////////////////////////
	void display();

	///////////////////////////////////////////////////////////
	/// \brief Set the window resolution
	///
	/// If the window is in fullscreen mode, the ressolution is
	/// changed. Otherwise, the size of the window is changed as well.
	///
	/// \param w Width of the new resolution
	/// \param h Height of the new resolution
	///
	///////////////////////////////////////////////////////////
	void setResolution(Uint32 w, Uint32 h);

	///////////////////////////////////////////////////////////
	/// \brief Set the window resolution
	///
	/// If the window is in fullscreen mode, the ressolution is
	/// changed. Otherwise, the size of the window is changed as well.
	///
	/// \param resolution The new resolution
	///
	///////////////////////////////////////////////////////////
	void setResolution(const Vector2u& resolution);

	///////////////////////////////////////////////////////////
	/// \brief Set the window title
	///
	/// \param title The new title
	///
	///////////////////////////////////////////////////////////
	void setTitle(const std::string& title);

	///////////////////////////////////////////////////////////
	/// \brief Set the cursor type to one of the standard types for thiw window
	///
	/// \param cursor A standard system cursor type
	///
	///////////////////////////////////////////////////////////
	void setCursor(Cursor cursor);

	///////////////////////////////////////////////////////////
	/// \brief Set the cursor mode for this window
	///
	/// \param mode The cursor mode
	///
	///////////////////////////////////////////////////////////
	void setCursorMode(CursorMode mode);

	///////////////////////////////////////////////////////////
	/// \brief Get the window resolution
	///
	/// \return The window resolution
	///
	///////////////////////////////////////////////////////////
	Vector2u getResolution() const;

	///////////////////////////////////////////////////////////
	/// \brief Set the window title
	///
	/// \return The window title
	///
	///////////////////////////////////////////////////////////
	const std::string& getTitle() const;

private:
	GLFWwindow* m_window;						//!< GLFW window pointer
	GLFWcursor* m_cursor;						//!< The current cursor
	std::string m_title;						//!< Window title

	static Uint32 numWindows;					//!< The number of existing windows
	static GLFWcursor* s_standardCursors[6];	//!< The standard cursors
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::Window
/// \ingroup Graphics
///
/// Thw Window class provides functions to create and manage
/// windows, and to handle user input. It uses GLFW internally
/// for cross-platform window functionality. Having multiple
/// windows opened at once is supported and should be pretty
/// straight forward to handle.
///
/// To create a new window, use the constructor or call create().
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Window window;
///
/// // Create a new window
/// window.create(1280, 720, "My Game");
///
/// // Add an event listener
/// window.addListener<E_KeyEvent>(
///		[&](const E_KeyEvent& e)
///		{
///			// This will be run every time a key event occurs
///			if (e.m_action == InputAction::Press)
///				std::cout << "Key pressed: " << (int)e.m_key << '\n';
///		}
/// );
///
/// // Game loop
/// while (window.isOpen())
/// {
///		// Poll events for all existing windows
///		Window::pollEvents();
///
///		// Display (swap buffers)
///		window.display();
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////