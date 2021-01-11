#include <poly/Core/Clock.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Components.h>
#include <poly/Graphics/PostProcess.h>
#include <poly/Graphics/Skybox.h>
#include <poly/Graphics/Terrain.h>
#include <poly/Graphics/Window.h>

#include <poly/Math/Noise.h>

#include <poly/UI/Font.h>
#include <poly/UI/ScrollView.h>
#include <poly/UI/Text.h>
#include <poly/UI/UISystem.h>

#include "BrushPanel.h"
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

    Vector2u renderTargetSize;
    renderTargetSize.y = FrameBuffer::Default.getHeight();
    renderTargetSize.x = (Uint32)(renderTargetSize.y * 16.0f / 9.0f);

    // Post processing
    FrameBuffer framebuffers[2];
    Texture textures[4];
    for (Uint32 i = 0; i < 2; ++i)
    {
        framebuffers[i].create(renderTargetSize.x, renderTargetSize.y);
        framebuffers[i].attachColor(&textures[2 * i], PixelFormat::Rgb, GLType::Uint16);
        framebuffers[i].attachDepth(&textures[2 * i + 1]);
    }

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

    // Main panel
    ScrollView panel;
    panel.setWidth(200.0f);
    panel.setRelHeight(1.0f);
    panel.setColor(0.15f, 0.15f, 0.18f, 1.0f);
    ui.addChild(&panel);

    ListView listView;
    panel.addChild(&listView);

    UIElement seperators[2];
    for (Uint32 i = 0; i < 2; ++i)
    {
        seperators[i].setPosition(3.0f, 0.0f);
        seperators[i].setSize(194.0f, 1.0f);
        seperators[i].setColor(0.25f, 0.25f, 0.3f, 1.0f);
    }

    // Brush panel
    BrushPanel brushPanel;
    brushPanel.setRadius(10.0f);
    brushPanel.setStrength(0.1f);
    brushPanel.setGradient(1.5f);
    listView.addChild(&brushPanel);
    listView.addChild(&seperators[0], Vector2f(8.0f, 0.0f));

    // Render view
    UIElement renderTarget;
    {
        renderTarget.setPosition(200.0f, 0.0f);
        renderTarget.setSize((float)renderTargetSize.x, (float)renderTargetSize.y);
        renderTarget.setTexture(&textures[0]);
        renderTarget.setFlippedUv(true);
        ui.addChild(&renderTarget);
    }

    ///////////////////////////////////////////////////////////

    // Resize event
    window.addListener<E_WindowResize>(
        [&](const E_WindowResize& e)
        {
            renderTargetSize.y = FrameBuffer::Default.getHeight();
            renderTargetSize.x = (Uint32)(renderTargetSize.y * 16.0f / 9.0f);

            for (Uint32 i = 0; i < 2; ++i)
            {
                framebuffers[i].reset();
                framebuffers[i].create(renderTargetSize.x, renderTargetSize.y);
                framebuffers[i].attachColor(&textures[2 * i], PixelFormat::Rgb, GLType::Uint16);
                framebuffers[i].attachDepth(&textures[2 * i + 1]);
            }

            renderTarget.setSize((float)renderTargetSize.x, (float)renderTargetSize.y);
        }
    );


    Clock clock;

    // Game loop
    while (window.isOpen())
    {
        float elapsed = clock.restart().toSeconds();

        // Poll events
        Window::pollEvents();

        // Render stuff
        scene.render(camera, framebuffers[0]);
        fxaa.render(framebuffers[0], framebuffers[1]);
        colorAdjust.render(framebuffers[1], framebuffers[0]);

        ui.update(elapsed);
        ui.render(FrameBuffer::Default, false);

        // Swap buffers
        window.display();
    }
}