#include <poly/Core/Clock.h>
#include <poly/Core/Logger.h>
#include <poly/Core/Sleep.h>

#include <poly/Graphics/Animation.h>
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

    Matrix4f t = toTransformMatrix(Vector3f(0.0f, 0.0f, 5.0f), Vector3f(0.0f), Vector3f(1.0f));
    Bone* bone = skeleton.getBone("Armature_Chest");
    // bone->setTransform(t);

    Animation walk;
    walk.load("models/character/character.dae", "Armature");
    skeleton.setAnimation(&walk);

    Shader shader;
    shader.load("shaders/animated.vert", Shader::Vertex);
    shader.load("shaders/default.frag", Shader::Fragment);
    shader.compile();

    Camera camera;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Clock clock;
    float time = 0.0f;

    // Game loop
    while (window.isOpen())
    {
        // Poll events for all existing windows
        Window::pollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float elapsed = clock.restart().toSeconds();
        time += elapsed;

        camera.setPosition(0.0f, 5.0f, 15.0f);
        Matrix4f projView = camera.getProjMatrix() * camera.getViewMatrix();
        Matrix4f transform = toTransformMatrix(Vector3f(0.0f), Quaternion(Vector3f(0, 1, 0), time * 50.0f), Vector3f(1.0f));

        shader.bind();
        shader.setUniform("u_projView", projView);
        shader.setUniform("u_transform", transform);
        material.apply(&shader);
        skeleton.update(elapsed);
        skeleton.apply(&shader);
        vao.draw();

        // Display (swap buffers)
        window.display();
    }

    return 0;
}