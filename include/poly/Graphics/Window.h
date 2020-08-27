#ifndef POLY_WINDOW_H
#define POLY_WINDOW_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Vector2.h>

// Tell GLFW to not include opengl
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace poly
{

class Window
{
public:
	Window();
	~Window();

#ifndef DOXYGEN_SKIP
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = default;
	Window& operator=(Window&&) = default;
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