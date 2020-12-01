#include <poly/Core/Clock.h>
#include <poly/Core/Logger.h>
#include <poly/Core/Profiler.h>
#include <poly/Core/Sleep.h>

#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

#include <poly/Graphics/Animation.h>
#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Components.h>
#include <poly/Graphics/FrameBuffer.h>
#include <poly/Graphics/Image.h>
#include <poly/Graphics/Model.h>
#include <poly/Graphics/Octree.h>
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
    camera.setRotation(0.0f, 0.0f);

    // Setup scene
    Scene scene;
    Octree octree;
    octree.init(&scene);

    DirLightComponent sun;
    sun.m_direction.z = -1.0f;
    scene.createEntity(sun);

    TransformComponent t;
    t.m_scale = Vector3f(0.25f);
    RenderComponent r(&model, &shader);
    octree.add(scene.createEntity(t, r));
    t.m_position.x = 5.0f;
    octree.add(scene.createEntity(t, r), true);
    t.m_position.x = -5.0f;
    octree.add(scene.createEntity(t, r), true);

    Clock clock;
    float time = 0.0f;

    FrameBuffer framebuffer;
    framebuffer.create(1280, 720);
    framebuffer.bind();
    framebuffer.attachColor();
    framebuffer.attachDepth();


    bool mouseDown = false;
    window.addListener<E_MouseButton>(
        [&](const E_MouseButton& e)
        {
            if (e.m_button == Mouse::Left)
                mouseDown = e.m_action == InputAction::Press;
        }
    );

    Vector2f mousePos, cameraRot;
    bool firstRun = true;
    window.addListener<E_MouseMove>(
        [&](const E_MouseMove& e)
        {
            const float sensitivity = 0.1f;

            Vector2f pos = Vector2f(e.m_x, e.m_y);
            if (firstRun)
            {
                mousePos = pos;
                firstRun = false;
            }

            Vector2f delta = sensitivity * (pos - mousePos);
            mousePos = pos;

            if (!mouseDown) return;

            // Update camera
            cameraRot.x = fmod(cameraRot.x - delta.y, 360.0f);
            cameraRot.y = fmod(cameraRot.y + delta.x, 360.0f);
            if (cameraRot.x > 89.0f)
                cameraRot.x = 89.0f;
            else if (cameraRot.x < -89.0f)
                cameraRot.x = -89.0f;

            camera.setRotation(cameraRot);
        }
    );

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
        octree.update();
        octree.render(camera);

        // Display (swap buffers)
        window.display();
    }

    const ProfilerData& data = Profiler::getData("poly::Octree::render");
    std::cout << data.mean().toMicroseconds() << '\n';

    return 0;
}

// TODO : Copyable skeletons