#include <poly/Core/Logger.h>
#include <poly/Core/Sleep.h>

#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>
#include <poly/Graphics/Window.h>

#include <iostream>

using namespace poly;

#include <glad/glad.h>
int main()
{
    Window window;

    // Create a new window
    window.create(1280, 720, "My Game");

    // Add an event listener
    window.addListener<E_KeyEvent>(
        [&](const E_KeyEvent& e)
        {
            // This will be run every time a key event occurs
            if (e.m_action == InputAction::Press)
                std::cout << "Key pressed: " << (int)e.m_key << '\n';
        }
    );

    std::vector<float> vertices =
    {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,

        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    VertexBuffer buffer;
    buffer.create(vertices);

    VertexArray vao;
    vao.addBuffer(buffer, 0, 3);

    // Game loop
    while (window.isOpen())
    {
        // Poll events for all existing windows
        Window::pollEvents();

        vao.draw();

        // Display (swap buffers)
        window.display();
    }

    return 0;
}