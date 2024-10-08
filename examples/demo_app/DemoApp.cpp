﻿#include <poly/Audio/AudioFileWriter.h>
#include <poly/Audio/AudioRecorder.h>
#include <poly/Audio/AudioStream.h>
#include <poly/Audio/Music.h>
#include <poly/Audio/Sound.h>

#include <poly/Core/Allocate.h>
#include <poly/Core/Clock.h>
#include <poly/Core/Logger.h>
#include <poly/Core/Profiler.h>
#include <poly/Core/Sleep.h>

#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

#include <poly/Graphics/Animation.h>
#include <poly/Graphics/Billboard.h>
#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Components.h>
#include <poly/Graphics/FrameBuffer.h>

#include <poly/Graphics/Image.h>
#include <poly/Graphics/Lighting.h>
#include <poly/Graphics/Model.h>
#include <poly/Graphics/Octree.h>

#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/ParticleSystem.h>
#include <poly/Graphics/PostProcess.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Shadows.h>
#include <poly/Graphics/Skeleton.h>
#include <poly/Graphics/Skybox.h>
#include <poly/Graphics/Terrain.h>
#include <poly/Graphics/Texture.h>
#include <poly/Graphics/UniformBuffer.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>
#include <poly/Graphics/Window.h>

#include <poly/Math/Functions.h>
#include <poly/Math/Noise.h>
#include <poly/Math/Transform.h>

#include <poly/Network/Packet.h>
#include <poly/Network/TcpSocket.h>
#include <poly/Network/UdpSocket.h>

#include <poly/Physics/BoxCollider.h>
#include <poly/Physics/Components.h>
#include <poly/Physics/Events.h>
#include <poly/Physics/Physics.h>
#include <poly/Physics/Shapes.h>

#include <poly/UI/Button.h>
#include <poly/UI/Dropdown.h>
#include <poly/UI/Font.h>
#include <poly/UI/ListView.h>
#include <poly/UI/ScrollView.h>
#include <poly/UI/Slider.h>
#include <poly/UI/Text.h>
#include <poly/UI/TextInput.h>
#include <poly/UI/UIElement.h>
#include <poly/UI/UIParser.h>
#include <poly/UI/UISystem.h>

#include <iostream>
#include <thread>

using namespace poly;


void onTestPress(UIElement* elem)
{
    Button* btn = (Button*)elem;
    std::cout << btn->getString().c_str() << '\n';
}

void onSubmit(const Utf32String& text)
{
    std::cout << text.c_str() << '\n';
}

///////////////////////////////////////////////////////////
float remap(float val, float min1, float max1, float min2, float max2)
{
	return min2 + (((val - min1) / (max1 - min1)) * (max2 - min2));
}


///////////////////////////////////////////////////////////
float getDistToSphere(float r, float mu, float r1)
{
    r = r < r1 ? r : r1;
    return -r * mu + sqrt(r * r * (mu * mu - 1.0f) + r1 * r1);
}


void testBuffer()
{
    Uint32 testBuffer;
    Uint32* test = &testBuffer;
    glCheck(glGenVertexArrays(1, test));
}


int main()
{
    Logger::init("game.log");
    srand(time(NULL));

    Window window;

    // Create a new window
    window.create(1280, 720, "My Game");
    window.setVsyncEnabled(false);

    std::vector<Vertex> vertices =
    {
        // Front
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0.0f, 0.0f, 1.0f)),

        // Back
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)),
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)),
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)),
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)),
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0.0f, 0.0f, -1.0f)),

        // Right
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(1.0f, 0.0f, 0.0f)),

        // Right
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(-1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(-1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(-1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(-1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(-1.0f, 0.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(-1.0f, 0.0f, 0.0f)),

        // Top
        Vertex(Vector3f(-0.5f, 0.5f, -0.5f), Vector3f(0.0f, 1.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0.0f, 1.0f, 0.0f)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0.0f, 1.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(0.0f, 1.0f, 0.0f)),
        Vertex(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0.0f, 1.0f, 0.0f)),
        Vertex(Vector3f(0.5f, 0.5f, -0.5f), Vector3f(0.0f, 1.0f, 0.0f)),

        // Bottom
        Vertex(Vector3f(0.5f, -0.5f, -0.5f), Vector3f(0.0f, -1.0f, 0.0f)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.0f, -1.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0.0f, -1.0f, 0.0f)),
        Vertex(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.0f, -1.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, 0.5f), Vector3f(0.0f, -1.0f, 0.0f)),
        Vertex(Vector3f(-0.5f, -0.5f, -0.5f), Vector3f(0.0f, -1.0f, 0.0f))
    };

    Model box;
    box.addMesh(vertices);
    // box.getMesh()->m_material.setRenderMask(RenderPass::All & ~RenderPass::Default);

    Model model("examples/models/character/character_flat.dae");

    Skeleton skeleton;
    Animation animation("examples/models/character/character_flat.dae", "Armature");
    skeleton.load("examples/models/character/character_flat.dae");
    skeleton.setAnimation(&animation);
    // skeleton.setAnimationSpeed(0.0f);
    // skeleton.setAnimationTime(0.5f);

    Camera camera;
    camera.setPosition(0.0f, 50.0f, 0.0f);
    camera.setRotation(0.0f, 0.0f);
    camera.setFar(10000.0f);

    // UI
    Font font;
    font.load("examples/fonts/NotoSansCJKjp/NotoSansCJKjp-Regular.otf");
    Text::setDefaultFont(&font);

    UI_XML_CALLBACK(onTestPress);
    UI_XML_CALLBACK(onSubmit);

    UISystem ui;
    ui.setWindow(&window);
    ui.load("examples/ui.xml");

    Text* fpsCounter = (Text*)ui.getElement("fps_counter");

    // Load audio

    // Setup scene
    Scene scene;

    Terrain terrain;
    terrain.create(4000.0f, 200.0f);
    scene.addRenderSystem(&terrain);

    FractalNoise noise;
    noise.setFrequency(0.002f);
    noise.setOctaves(10);
    noise.setGain(0.5f);

    Image heightMap;
    float* heightMapData = (float*)MALLOC_DBG(1024 * 1024 * sizeof(float));
    noise.generateImage(heightMapData, 1024, 1024);

    heightMap.create(heightMapData, 1024, 1024, 1, GLType::Float, true);
    terrain.setHeightMap(heightMap);

    Image colorMap;
    noise.setOctaves(1);
    noise.setFrequency(0.05f);
    Vector3<Uint8>* colorMapData = (Vector3<Uint8>*)MALLOC_DBG(1024 * 1024 * sizeof(Vector3<Uint8>));
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
    colorMap.create(colorMapData, 1024, 1024, 3, GLType::Uint8, true);
    // terrain.setColorMap(colorMap);

    Octree octree;
    octree.create();

    ProceduralSkybox skybox;
    skybox.setZenithColor(Vector3f(0.25f, 0.5f, 0.9f));
    skybox.setHorizonColor(Vector3f(0.6f, 0.8f, 0.6f));
    scene.getExtension<Lighting>()->setAmbientColor(0.3f * skybox.getAmbientColor());

    scene.addRenderSystem(&skybox);
    scene.addRenderSystem(&octree);

    DirLightComponent sun;
    // sun.m_diffuse = Vector3f(0.08f, 0.15f, 0.25f) * 0.4f;
    sun.m_diffuse = Vector3f(0.9f, 0.8f, 0.45f);
    sun.m_specular = sun.m_diffuse * 0.2f;
    sun.m_direction.z = 2.0f;
    // sun.m_shadowDistance = 150.0f;
    // sun.m_shadowsEnabled = false;
    Entity sunEntity = scene.createEntity(sun);
    skybox.setDirLight(sunEntity);

    PointLightComponent light;
    light.m_diffuse = Vector3f(1.0f, 0.95f, 0.85f);
    light.m_specular = light.m_diffuse * 0.4f;
    TransformComponent lightT;
    lightT.m_position.y = 55.0f;

    // Activate physics extension
    Physics* physics = scene.getExtension<Physics>();
    physics->setGravity(0.0f, -12.0f, 0.0f);
    physics->setDebugRenderEnabled(true);

    TransformComponent t;
    t.m_position.y = 52.0f;
    t.m_scale = Vector3f(0.25f);
    RenderComponent r(&model);
    r.m_castsShadows = true;
    RigidBodyComponent rbody;
    rbody.m_position.y = 55.0f;
    rbody.m_mass = 65.0f;
    rbody.m_inertiaTensor = Vector3f(INFINITY);
    Entity player = scene.createEntity(&t, r, AnimationComponent(&skeleton), rbody, DynamicTag());
    CapsuleShape capsule(0.4f, 1.0f);
    Collider playerCollider = physics->addCollider(player, capsule, Vector3f(0.0f, 0.9f, 0.0f));
    playerCollider.setFrictionCoefficient(1.0f);

    t.m_scale = Vector3f(1.0f);

    for (Uint32 i = 0; i < 10; ++i)
    {
        RigidBodyComponent rbody;
        rbody.m_position = Vector3f(0.0f, 60.0f + 2 * i, -5.0f);
        rbody.m_mass = 10.0f;
        Entity boxEntity1 = scene.createEntity(t, RenderComponent(&box), rbody, DynamicTag());
        physics->addCollider(boxEntity1, BoxShape(1.0f, 1.0f, 1.0f));

        scene.removeEntity(boxEntity1);
    }


    Clock clock;
    float time = 0.0f;

    Texture texture[8];

    FrameBuffer framebuffers[2];
    for (Uint32 i = 0; i < 2; ++i)
    {
        framebuffers[i].create(1280, 720);
        framebuffers[i].attachColor(&texture[2 * i], PixelFormat::Rgb, GLType::Uint16);
        framebuffers[i].attachDepth(&texture[2 * i + 1]);
    }

    // Post process stuff
    ColorAdjust colorAdjust;
    Fog fog;
    fog.setCamera(&camera);
    fog.setDepthTexture(framebuffers[0].getDepthTexture());
    fog.setDirLight(sunEntity);
    fog.setColor(0.25f, 0.5f, 0.9f);
    fog.setSkyboxFog(false);

    Ssao ssao;
    ssao.setCamera(&camera);
    ssao.setDepthTexture(framebuffers[0].getDepthTexture());
    ssao.setIntensity(1.0f);
    ssao.setRadius(0.3f);

    Bloom bloom;
    bloom.setRadius(0.2f);
    bloom.setNumBlurs(3);
    bloom.setThresholdInterval(0.5f);

    Fxaa fxaa;

    LensFlare flare;
    flare.setScene(&scene);
    flare.setCamera(&camera);

    Reflections ssr;
    ssr.setCamera(&camera);
    ssr.setCubemap(&skybox);

    // Sky colors
    std::vector<float> angles =
    {
        55.0f,
        45.0f,
        35.0f,
        20.0f,
        10.0f,
        -10.0f,
        -90.0f
    };

    std::vector<Vector3f> zeniths =
    {
        Vector3f(0.25f, 0.5f, 0.9f),
        Vector3f(0.25f, 0.45f, 0.7f),
        Vector3f(0.2f, 0.4f, 0.6f),
        Vector3f(0.15f, 0.3f, 0.5f),
        Vector3f(0.1f, 0.2f, 0.3f),
        Vector3f(0.0f, 0.04f, 0.06f)
    };

    std::vector<Vector3f> horizons =
    {
        Vector3f(0.6f, 0.8f, 0.6f),
        Vector3f(0.6f, 0.8f, 0.6f),
        Vector3f(0.7f, 0.65f, 0.4f),
        Vector3f(0.6f, 0.55f, 0.25f),
        Vector3f(0.5f, 0.45f, 0.2f),
        Vector3f(0.01f, 0.05f, 0.1f)
    };

    std::vector<Vector3f> lightColors =
    {
        Vector3f(0.9f, 0.8f, 0.45f),
        Vector3f(0.8f, 0.7f, 0.4f),
        Vector3f(0.8f, 0.6f, 0.3f),
        Vector3f(0.7f, 0.55f, 0.3f),
        Vector3f(0.7f, 0.5f, 0.3f),
        Vector3f(0.02f, 0.06f, 0.12f)
    };

    std::thread thread(testBuffer);
    thread.join();


    HashSet<Entity::Id> touchingFeet;
    bool onGround = false;
    bool usedDoubleJump = false;
    Clock leftGroundClock;
    Clock dashClock;
    const float jumpSpeed = 6.0f;

    scene.addListener<E_PhysicsCollision>(
        [&](const E_PhysicsCollision& e)
        {
            int playerIndex = -1;
            if (e.m_entities[0] == player.getId())
                playerIndex = 0;
            else if (e.m_entities[1] == player.getId())
                playerIndex = 1;
            else
                return;

            if (e.m_type == CollisionEventType::Start)
            {
                for (Uint32 i = 0; i < e.m_numContacts; ++i)
                {
                    const ContactPoint& point = e.m_contacts[i];
                    if (point.m_pointOnColliders[playerIndex].y < -0.8f)
                    {
                        touchingFeet.insert(e.m_entities[(playerIndex + 1) % 2]);
                        onGround = true;
                        usedDoubleJump = false;
                    }
                }
            }
            else
            {
                auto it = touchingFeet.find(e.m_entities[(playerIndex + 1) % 2]);
                if (it != touchingFeet.end())
                    touchingFeet.erase(it);

                if (touchingFeet.size() == 0)
                {
                    leftGroundClock.restart();
                    onGround = false;
                }
            }
        }
    );


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

                if (keyMap[Keyboard::LeftControl])
                {
                    if (e.m_key == Keyboard::R)
                    {
                        ui.load("examples/ui.xml");
                        fpsCounter = (Text*)ui.getElement("fps_counter");
                    }
                }

                if (e.m_key == Keyboard::Space)
                {
                    if (onGround || leftGroundClock.getElapsedTime().toSeconds() < 0.2f)
                    {
                        player.get<RigidBodyComponent>()->m_linearVelocity.y = jumpSpeed;
                    }
                    else if (!usedDoubleJump)
                    {
                        player.get<RigidBodyComponent>()->m_linearVelocity.y = jumpSpeed;
                        usedDoubleJump = true;
                    }
                }

                if (e.m_key == Keyboard::LeftShift)
                {
                    if (dashClock.getElapsedTime() > 5.0f)
                        dashClock.restart();
                }
            }
            else if (e.m_action == InputAction::Release)
            {
                keyMap[e.m_key] = false;
            }
        }
    );

    window.addListener<E_WindowResize>(
        [&](const E_WindowResize& e)
        {
            for (Uint32 i = 0; i < 2; ++i)
            {
                framebuffers[i].reset();
                framebuffers[i].create(e.m_width, e.m_height);
                framebuffers[i].attachColor(&texture[2 * i], PixelFormat::Rgb, GLType::Uint16);
                framebuffers[i].attachDepth(&texture[2 * i + 1]);
            }
        }
    );

    bool leftDown = false, rightDown = false;
    window.addListener<E_MouseButton>(
        [&](const E_MouseButton& e)
        {
            if (e.m_button == Mouse::Left)
                leftDown = e.m_action == InputAction::Press;
            else if (e.m_button == Mouse::Right)
                rightDown = e.m_action == InputAction::Press;
        }
    );

    Vector2f mousePos, cameraRot, lightRot;
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

            if (leftDown)
            {
                // Update camera
                cameraRot.x = fmod(cameraRot.x - delta.y, 360.0f);
                cameraRot.y = fmod(cameraRot.y + delta.x, 360.0f);
                if (cameraRot.x > 89.0f)
                    cameraRot.x = 89.0f;
                else if (cameraRot.x < -89.0f)
                    cameraRot.x = -89.0f;

                camera.setRotation(cameraRot);
                player.get<RigidBodyComponent>()->m_rotation = Quaternion(0.0f, -cameraRot.y + 180.0f, 0.0f);
            }
            else if (rightDown)
            {
                // Update camera
                lightRot.x = fmod(lightRot.x + delta.y, 360.0f);
                lightRot.y = fmod(lightRot.y + delta.x, 360.0f);
                if (lightRot.x > 89.0f)
                    lightRot.x = 89.0f;
                else if (lightRot.x < -89.0f)
                    lightRot.x = -89.0f;

                lightRot.x = -lightRot.x;

                // Get index and interpolation factor
                Uint32 index = 0, index2 = 0;
                float factor = 0.0f;

                for (; index < angles.size(); ++index)
                {
                    if (lightRot.x > angles[index])
                        break;
                }

                if (index == 0)
                {
                    index2 = 0;
                    factor = 0.0f;
                }
                else if (index == zeniths.size())
                {
                    index2 = zeniths.size() - 1;
                    factor = 0.0f;
                    --index;
                }
                else
                {
                    index2 = index - 1;
                    factor = (lightRot.x - angles[index]) / (angles[index2] - angles[index]);
                }

                Vector3f zenith = zeniths[index] + factor * (zeniths[index2] - zeniths[index]);
                Vector3f horizon = horizons[index] + factor * (horizons[index2] - horizons[index]);
                Vector3f lightColor = lightColors[index] + factor * (lightColors[index2] - lightColors[index]);
                skybox.setZenithColor(zenith);
                skybox.setHorizonColor(horizon);
                fog.setColor(zenith.r, zenith.g, zenith.b);

                lightRot.x = -lightRot.x;
                float x = rad(lightRot.x > 10.0f ? lightRot.x + 180.0f : lightRot.x);
                float y = rad(lightRot.y - 90.0f);

                float cx = cos(x);
                float cy = cos(y);
                float sx = sin(x);
                float sy = sin(y);

                DirLightComponent* lightComponent = sunEntity.get<DirLightComponent>();
                lightComponent->m_direction = normalize(Vector3f(cy * cx, sx, sy * cx));
                lightComponent->m_diffuse = lightColor;
                lightComponent->m_specular = lightColor * 0.2f;

                scene.getExtension<Lighting>()->setAmbientColor(0.3f * skybox.getAmbientColor());
            }
        }
    );

    const Uint32 windowSize = 100;
    std::vector<float> fpsWindow;
    Uint32 windowIndex = 0;

    // Game loop
    while (window.isOpen())
    {
        START_PROFILING(GameLoop);

        // Poll events for all existing windows
        Window::pollEvents();

        float elapsed = clock.restart().toSeconds();
        time += elapsed;

        // Get lateral movement vector
        Vector3f move(0.0f);
        if (keyMap[Keyboard::W])
            move += camera.getDirection();
        if (keyMap[Keyboard::S])
            move -= camera.getDirection();
        if (keyMap[Keyboard::D])
            move += camera.getRightDir();
        if (keyMap[Keyboard::A])
            move -= camera.getRightDir();

        const float maxVelocity = 5.0f;

        // Get velocity
        RigidBodyComponent* body = player.get<RigidBodyComponent>();
        Vector3f velocity = body->m_linearVelocity * Vector3f(1, 0, 1);
        float velocityMag = length(velocity);
        Vector3f velocityDir = velocityMag == 0.0f ? Vector3f(0.0f) : velocity / velocityMag;

        // Calculate drag factor
        float coefficient = onGround ? 1000.0f : 50.0f;
        Vector3f drag = -velocityDir * 0.5f * coefficient * velocityMag * velocityMag;

        body->m_force += drag;

        if (length(move) > 0.0f)
        {
            // Reduce friction while moving
            playerCollider.setFrictionCoefficient(0.0f);

            // Get lateral direction
            move = normalize(Vector3f(move.x, 0.0f, move.z));

            Vector3f force = move * 0.5f * coefficient * maxVelocity * maxVelocity;
            body->m_force += force;
        }
        else
            playerCollider.setFrictionCoefficient(1.0f);

        scene.getExtension<Physics>()->update(elapsed);

        // Copy position and rotation
        scene.system<RigidBodyComponent, TransformComponent>(
            [&](const Entity::Id& id, const RigidBodyComponent& rbody, TransformComponent& t)
            {
                t.m_position = rbody.m_position;
                t.m_rotation = rbody.m_rotation;
            }
        );

        TransformComponent* t = player.get<TransformComponent>();
        camera.setPosition(t->m_position - 3.0f * camera.getDirection() + Vector3f(0.0f, 2.0f, 0.0f));

        const ProfilerData& data = Profiler::getData("main", "GameLoop");

        if (data.getAverages().size())
        {
            float fps = 1.0f / elapsed;
            if (windowIndex >= fpsWindow.size())
                fpsWindow.push_back(fps);
            else
                fpsWindow[windowIndex] = fps;

            windowIndex = (windowIndex + 1) % windowSize;

            float avg = 0.0f;
            for (Uint32 i = 0; i < fpsWindow.size(); ++i)
                avg += fpsWindow[i];

            fpsCounter->setString(std::to_string(std::lroundf(avg / fpsWindow.size())));
        }


        ui.update(elapsed);

        // Render scene
        skeleton.update(elapsed);
        octree.update();
        scene.getExtension<Shadows>()->render(camera);
        scene.render(camera, framebuffers[0]);
        // physics->render(camera, framebuffers[0]);

        ssr.setGBuffer(scene.getRenderer().getGBuffer(framebuffers[0]));

        ssr.render(framebuffers[0], framebuffers[1]);
        ssao.render(framebuffers[1], framebuffers[0]);
        fog.render(framebuffers[0], framebuffers[1]);
        bloom.render(framebuffers[1], framebuffers[0]);
        flare.render(framebuffers[0], framebuffers[1]);
        colorAdjust.render(framebuffers[1], framebuffers[0]);
        fxaa.render(framebuffers[0]);

        ui.render();

        STOP_PROFILING(GameLoop);

        scene.removeQueuedEntities();

        // Display (swap buffers)
        window.display();
    }

    {
        const ProfilerData& data = Profiler::getData("main", "GameLoop");
        std::cout << "Game loop: " << data.mean().toMicroseconds() << '\n';
    }
    {
        const ProfilerData& data = Profiler::getData("poly::Octree::update");
        std::cout << "Octree update: " << data.mean().toMicroseconds() << '\n';
    }
    {
        const ProfilerData& data = Profiler::getData("poly::Terrain::render");
        std::cout << "Terrain render: " << data.mean().toMicroseconds() << '\n';
    }

    return 0;
}

// TODO : Consider switching to a different physics engine in the future if there are too many bugs
// TODO : Handle rendering transparent objects in octree
// TODO : Add a custom scene render (for more customization in deferred pipeline)
// TODO : Environment cubemaps + integrate them into reflections effect
// TODO : Optimize height bounds map for LargeTerrain to handle even larger terrains (this is currently the limiting factor)