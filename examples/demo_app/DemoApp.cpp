#include <poly/Core/Clock.h>
#include <poly/Core/Logger.h>
#include <poly/Core/Profiler.h>
#include <poly/Core/Sleep.h>

#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

#include <poly/Graphics/Animation.h>
#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Components.h>
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

    Shader shader;
    shader.load("shaders/default.vert", Shader::Vertex);
    shader.load("shaders/default.frag", Shader::Fragment);
    shader.compile();

    Camera camera;
    camera.setPosition(0.0f, 5.0f, 15.0f);

    // Setup scene
    Scene scene;
    scene.setCamera(&camera);
    scene.createEntity(TransformComponent(), RenderComponent{ &model, &shader });

    Clock clock;
    float time = 0.0f;

    // Game loop
    while (window.isOpen())
    {
        // Poll events for all existing windows
        Window::pollEvents();

        float elapsed = clock.restart().toSeconds();
        time += elapsed;

        scene.system<TransformComponent>(
            [&](const Entity::Id& id, TransformComponent& t)
            {
                t.m_rotation.y = time * 60.0f;
            }
        );

        // Render scene
        scene.render();

        // Display (swap buffers)
        window.display();
    }

    return 0;
}