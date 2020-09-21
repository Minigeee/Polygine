#include <poly/Core/Clock.h>
#include <poly/Core/Logger.h>
#include <poly/Core/Sleep.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Image.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Texture.h>
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
        // Front
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         
        // Back
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Right
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,

        // Left
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Top
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

        // Bottom
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f
    };

    VertexBuffer buffer;
    buffer.create(vertices);

    VertexArray vao;
    vao.addBuffer(buffer, 0, 3);

    Shader shader;
    shader.load("shaders/default.vert", Shader::Vertex);
    shader.load("shaders/default.frag", Shader::Fragment);
    shader.compile();

    Camera camera;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Clock clock;

    // Game loop
    while (window.isOpen())
    {
        // Poll events for all existing windows
        Window::pollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.setPosition(sin(clock.getElapsedTime().toSeconds()) * 1.5f, 0.0f, 5.0f);
        Matrix4f projView = camera.getProjMatrix() * camera.getViewMatrix();

        shader.bind();
        shader.setUniform("m_projView", projView);
        vao.draw();

        // Display (swap buffers)
        window.display();
    }

    return 0;
}