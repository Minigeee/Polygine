#include <poly/Core/Clock.h>
#include <poly/Core/Logger.h>
#include <poly/Core/Sleep.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Image.h>
#include <poly/Graphics/Model.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Skeleton.h>
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

    Model model;
    model.load("models/character/character.dae");
    VertexArray& vao = model.getVertexArray();
    const Material& material = model.getMaterial();

    Skeleton skeleton;
    skeleton.load("models/character/character.dae");

    Shader shader;
    shader.load("shaders/animated.vert", Shader::Vertex);
    shader.load("shaders/default.frag", Shader::Fragment);
    shader.compile();

    Camera camera;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Clock clock;

    // Game loop
    while (window.isOpen())
    {
        // Poll events for all existing windows
        Window::pollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.setPosition(sin(clock.getElapsedTime().toSeconds()) * 1.5f, 5.0f, 15.0f);
        Matrix4f projView = camera.getProjMatrix() * camera.getViewMatrix();

        shader.bind();
        shader.setUniform("u_projView", projView);
        shader.setUniform("u_transform", Matrix4f(1.0f));
        material.apply(&shader);
        skeleton.apply(&shader);
        vao.draw();

        // Display (swap buffers)
        window.display();
    }

    return 0;
}