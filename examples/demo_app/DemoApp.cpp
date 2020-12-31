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
#include <poly/Graphics/ParticleSystem.h>
#include <poly/Graphics/PostProcess.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Skeleton.h>
#include <poly/Graphics/Skybox.h>
#include <poly/Graphics/Terrain.h>
#include <poly/Graphics/Texture.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>
#include <poly/Graphics/Window.h>

#include <poly/Math/Noise.h>
#include <poly/Math/Transform.h>

#include <poly/UI/UISystem.h>
#include <poly/UI/UIElement.h>

#include <iostream>

#include <glad/glad.h>

using namespace poly;

int main()
{
    Window window;

    // Create a new window
    window.create(1280, 720, "My Game");

    // Add an event listener
    HashMap<Keyboard, bool> keyMap;
    window.addListener<E_KeyEvent>(
        [&](const E_KeyEvent& e)
        {
            // This will be run every time a key event occurs
            if (e.m_action == InputAction::Press)
            {
                std::cout << "Key pressed: " << (int)e.m_key << '\n';
                keyMap[e.m_key] = true;
            }
            else if (e.m_action == InputAction::Release)
                keyMap[e.m_key] = false;
        }
    );

    Model model;
    model.load("models/character/character.dae");

    Skeleton skeleton("models/character/character.dae");
    Animation animation("models/character/character.dae", "Armature");
    skeleton.setAnimation(&animation);

    Shader shader;
    shader.load("shaders/default.vert", Shader::Vertex);
    shader.load("shaders/default.frag", Shader::Fragment);
    shader.compile();

    Shader animShader;
    animShader.load("shaders/animated.vert", Shader::Vertex);
    animShader.load("shaders/default.frag", Shader::Fragment);
    animShader.compile();

    Camera camera;
    camera.setPosition(0.0f, 50.0f, 0.0f);
    camera.setRotation(0.0f, 0.0f);

    // Setup scene
    Scene scene;

    Terrain terrain;
    terrain.create(4000.0f, 200.0f, 1.5f);
    scene.addRenderSystem(&terrain);

    FractalNoise noise;
    noise.setFrequency(0.002f);
    noise.setOctaves(10);
    noise.setGain(0.5f);

    Image heightMap;
    float* heightMapData = (float*)malloc(1024 * 1024 * sizeof(float));
    noise.generateImage(heightMapData, 1024, 1024);
    heightMap.create(heightMapData, 1024, 1024, 1, GLType::Float, true);
    terrain.setHeightMap(heightMap);

    Image colorMap;
    noise.setOctaves(1);
    noise.setFrequency(0.05f);
    Vector3<Uint8>* colorMapData = (Vector3<Uint8>*)malloc(1024 * 1024 * sizeof(Vector3<Uint8>));
    for (Uint32 r = 0, i = 0; r < 1024; ++r)
    {
        for (Uint32 c = 0; c < 1024; ++c, ++i)
        {
            Vector3f color = Vector3f(0.169f, 0.431f, 0.188f);
            color.r += noise.generate(c, r) * 0.015f;
            color.b += noise.generate(c + 2000, r + 2000) * 0.015f;

            colorMapData[i] = Vector3<Uint8>(color * 255.0f);
        }
    }
    heightMap.create(colorMapData, 1024, 1024, 3, GLType::Uint8, true);
    terrain.setColorMap(heightMap);

    Octree octree;
    octree.create();
    scene.addRenderSystem(&octree);

    ProceduralSkybox skybox;
    terrain.setAmbientColor(skybox.getAmbientColor() * 0.3f);
    scene.addRenderSystem(&skybox);

    DirLightComponent sun;
    // sun.m_diffuse = Vector3f(0.08f, 0.15f, 0.25f) * 0.4f;
    sun.m_diffuse = Vector3f(0.9f, 0.9f, 0.8f);
    sun.m_specular = sun.m_diffuse * 0.2f;
    sun.m_direction.z = 2.0f;
    scene.createEntity(sun);

    TransformComponent t;
    t.m_position.y = 65.0f;
    t.m_scale = Vector3f(0.25f);
    RenderComponent r(&model, &animShader);
    scene.createEntity(t, r, AnimationComponent(&skeleton), DynamicTag());
    r.m_shader = &shader;
    t.m_position.x = 5.0f;
    scene.createEntity(t, r);
    t.m_position.x = -5.0f;
    scene.createEntity(t, r);

    Clock clock;
    float time = 0.0f;

    Texture texture[8];

    FrameBuffer multisampled;
    multisampled.create(1280, 720, 0, true);
    multisampled.bind();
    multisampled.attachColor(&texture[0], PixelFormat::Rgb, GLType::Uint16);
    multisampled.attachDepth();

    FrameBuffer framebuffer;
    framebuffer.create(1280, 720);
    framebuffer.bind();
    framebuffer.attachColor(&texture[1], PixelFormat::Rgb, GLType::Uint16);

    FrameBuffer uiTarget;
    uiTarget.create(1280, 720);
    uiTarget.bind();
    uiTarget.attachColor(&texture[2], PixelFormat::Rgba, GLType::Uint8);
    uiTarget.attachDepth();

    // Post process stuff
    ColorAdjust colorAdjust;

    UISystem ui(1280, 720);
    ui.setWindow(&window);

    UIElement elems[3];
    elems[0].setOrigin(UIPosition::Center);
    elems[0].setAnchor(UIPosition::Center);
    elems[0].setPosition(50.0f, 50.0f);
    elems[0].setRotation(45.0f);
    elems[0].setSize(50.0f, 50.0f);
    elems[0].setColor(1, 0, 0, 1);
    elems[1].setSize(50.0f, 50.0f);
    elems[1].setPosition(30.0f, 0.0f);
    elems[1].setColor(0, 1, 0, 0.2f);
    elems[2].setSize(50.0f, 50.0f);
    elems[2].setPosition(15.0f, 30.0f);
    elems[2].setColor(0, 0, 1, 0.5f);
    ui.addChild(&elems[0]);


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

        Vector3f move;
        if (keyMap[Keyboard::W])
            move += camera.getDirection();
        if (keyMap[Keyboard::S])
            move -= camera.getDirection();
        if (keyMap[Keyboard::A])
            move -= camera.getRight();
        if (keyMap[Keyboard::D])
            move += camera.getRight();
        if (keyMap[Keyboard::Space])
            move.y += 1.0f;
        if (keyMap[Keyboard::LeftShift])
            move.y -= 1.0f;

        if (length(move) != 0.0f)
            camera.move(normalize(move) * elapsed * 3.4f);

        // Render scene
        skeleton.update(elapsed);
        octree.update();
        scene.render(camera, multisampled);
        multisampled.blitTo(framebuffer);

        colorAdjust.render(framebuffer);
        ui.render(FrameBuffer::Default);

        // Display (swap buffers)
        window.display();
    }

    const ProfilerData& data = Profiler::getData("poly::UISystem::render");
    std::cout << data.mean().toMicroseconds() << '\n';

    return 0;
}

// TODO : Document SkyBox
// TODO : Improve particle system + document new version
// TODO : Improve post processing + document new version
// TODO : Change RenderState or remove it
// TODO : Renderable base class + Billboard
// TODO : Octree lod system
// TODO : Bloom effect