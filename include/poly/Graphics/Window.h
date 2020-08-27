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

class Window :
	public EventSystem<E_KeyEvent, E_MouseButton, E_MouseMove, E_MouseScroll>
{
public:
	Window();
	~Window();

#ifndef DOXYGEN_SKIP
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&);
	Window& operator=(Window&&);
#endif

	bool create(Uint32 w, Uint32 h, const std::string& title, bool fullscreen = false);

	bool isOpen() const;

	void close();

	void pollEvents();

	void display();

	void setResolution(Uint32 w, Uint32 h);

	void setResolution(const Vector2u& resolution);

	void setTitle(const std::string& title);

	Vector2u getResolution() const;

	const std::string& getTitle() const;

private:
	GLFWwindow* m_window;
	std::string m_title;

	static Uint32 numWindows;
};

}

#endif