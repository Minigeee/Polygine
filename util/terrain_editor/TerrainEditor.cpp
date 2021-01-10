#include <poly/Engine/Scene.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Components.h>
#include <poly/Graphics/PostProcess.h>
#include <poly/Graphics/Skybox.h>
#include <poly/Graphics/Terrain.h>
#include <poly/Graphics/Window.h>

#include <poly/Math/Noise.h>

#include <poly/UI/Dropdown.h>
#include <poly/UI/Font.h>
#include <poly/UI/Text.h>
#include <poly/UI/UISystem.h>

#include "CameraSystem.h"

using namespace poly;


///////////////////////////////////////////////////////////
void main()
{
    // Create window
    Window window;
    window.create(1280, 720, "Terrain Editor");

    ///////////////////////////////////////////////////////////

    // Create scene components
    Scene scene;
    Camera camera;
    Terrain terrain;
    ProceduralSkybox skybox;
    UISystem ui;

    // Setup camera system
    camera.setFar(2000.0f);
    CameraSystem cameraSystem(&window, &camera, &ui);

    // Setup scene
    Entity dirLight = scene.createEntity<DirLightComponent>();
    {
        DirLightComponent* light = dirLight.get<DirLightComponent>();
        light->m_diffuse = Vector3f(1.0f, 1.0f, 0.9f);
        light->m_specular = 0.2f * light->m_diffuse;
        light->m_direction = Vector3f(0.0f, -1.0f, 2.0f);
    }

    // Setup terrain
    Image hmap, cmap;
    hmap.create(0, 1024, 1024, 1, GLType::Float);
    cmap.create(0, 1024, 1024, 3);
    memset(hmap.getData(), 0, 1024 * 1024 * sizeof(float));
    memset(cmap.getData(), 0xFF, 1024 * 1024 * 3);

    terrain.create(4000.0f, 200.0f, 1.5f);
    terrain.setHeightMap(hmap);
    terrain.setColorMap(cmap);

    // Setup render systems
    scene.addRenderSystem(&terrain);
    scene.addRenderSystem(&skybox);

    // Post processing
    FrameBuffer framebuffers[2];
    Texture textures[4];
    for (Uint32 i = 0; i < 2; ++i)
    {
        framebuffers[i].create(1280, 720);
        framebuffers[i].attachColor(&textures[2 * i], PixelFormat::Rgb, GLType::Uint16);
        framebuffers[i].attachDepth(&textures[2 * i + 1]);
    }

    // Resize event
    window.addListener<E_WindowResize>(
        [&](const E_WindowResize& e)
        {
            for (Uint32 i = 0; i < 2; ++i)
            {
                framebuffers[i].reset();
                framebuffers[i].create(e.m_width, e.m_height);
                framebuffers[i].attachColor(&textures[2 * i], PixelFormat::Rgb, GLType::Uint16);
                framebuffers[i].attachDepth(&textures[2 * i + 1]);
            }
        }
    );

    Fog fog;
    Fxaa fxaa;
    ColorAdjust colorAdjust;

    ///////////////////////////////////////////////////////////

    // Load font
    Font font;
    font.load("fonts/segoeui/segoeui.ttf");
    Text::setDefaultFont(&font);

    // Setup UI
    ui.setWindow(&window);

    Dropdown dropdown;
    dropdown.setPosition(30.0f, 30.0f);
    dropdown.setSize(100.0f, 20.0f);
    dropdown.setColor(0.2f, 0.2f, 0.25f, 1.0f);
    dropdown.setTextAlign(UIPosition::Left);
    dropdown.setTextOffset(5.0f, 0.0f);
    dropdown.setItemHeight(20.0f);
    dropdown.setItemColor(0.3f, 0.3f, 0.35f, 1.0f);
    dropdown.setString("Test");

    dropdown.onMouseEnterItem(
        [&](Button* item, const E_MouseMove& e)
        {
            item->setColor(0.4f, 0.4f, 0.45f, 1.0f);
        }
    );

    dropdown.onMouseLeaveItem(
        [&](Button* item, const E_MouseMove& e)
        {
            item->setColor(0.3f, 0.3f, 0.35f, 1.0f);
        }
    );

    dropdown.addItem("Test");
    dropdown.addItem("Hello");
    dropdown.addItem("World");

    ui.addChild(&dropdown);


    // Game loop
    while (window.isOpen())
    {
        // Poll events
        Window::pollEvents();

        // Render stuff
        scene.render(camera, framebuffers[0]);
        fxaa.render(framebuffers[0], framebuffers[1]);
        colorAdjust.render(framebuffers[1]);

        ui.render();

        // Swap buffers
        window.display();
    }
}