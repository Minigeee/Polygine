#include <poly/Core/Logger.h>
#include <poly/Core/Sleep.h>

#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>
#include <poly/Graphics/Window.h>

#include <poly/Math/Transform.h>

#include <iostream>

#include <glad/glad.h>

using namespace poly;

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

    Shader shader;
    shader.load("shaders/default.vert", Shader::Vertex);
    shader.load("shaders/default.frag", Shader::Fragment);
    shader.compile();

    Vector3f p(0.0f);
    float r = 0.0f;

    // Game loop
    while (window.isOpen())
    {
        // Poll events for all existing windows
        Window::pollEvents();

        p.x += 0.001f;
        r += 0.5f;

        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        shader.setUniform("transform", toTransformMatrix(p, Quaternion(normalize(Vector3f(1, 0, 1)), r), 1.0f));
        vao.draw();

        // Display (swap buffers)
        window.display();
    }

    return 0;
}