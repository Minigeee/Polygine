#include <poly/Core/Clock.h>
#include <poly/Core/Profiler.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Camera.h>
#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
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
#include "ColorSelector.h"
#include "EditSystem.h"
#include "FilePanel.h"
#include "RenderView.h"

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

    // Setup scene
    Entity dirLight = scene.createEntity<DirLightComponent>();
    {
        DirLightComponent* light = dirLight.get<DirLightComponent>();
        light->m_diffuse = Vector3f(1.0f, 1.0f, 0.9f);
        light->m_specular = 0.2f * light->m_diffuse;
        light->m_direction = Vector3f(0.0f, -1.0f, 2.0f);
    }

    // Setup terrain
    terrain.create(4000.0f, 200.0f, 1.5f);

    // Setup render systems
    scene.addRenderSystem(&terrain);
    scene.addRenderSystem(&skybox);

    Vector2u renderTargetSize;
    renderTargetSize.y = FrameBuffer::Default.getHeight();
    renderTargetSize.x = (Uint32)(renderTargetSize.y * 16.0f / 9.0f);

    // Post processing
    FrameBuffer framebuffers[3];
    Texture textures[6];
    for (Uint32 i = 0; i < 3; ++i)
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
    font.load("examples/fonts/segoeui/segoeui.ttf");
    Text::setDefaultFont(&font);

    ColorSelector colorSelector;
    BrushPanel brushPanel(&colorSelector);
    EditSystem editSystem(&terrain, &brushPanel);

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

    UIElement separators[2];
    for (Uint32 i = 0; i < 2; ++i)
    {
        separators[i].setPosition(3.0f, 0.0f);
        separators[i].setSize(194.0f, 1.0f);
        separators[i].setColor(0.25f, 0.25f, 0.3f, 1.0f);
    }

    // Color selector
    ui.addChild(&colorSelector);

    // Import/export dialog
    ImportExportDialog importExportDialog;
    ui.addChild(&importExportDialog);

    // File panel
    FilePanel filePanel(&importExportDialog, &editSystem);
    listView.addChild(&filePanel);
    listView.addChild(&separators[0], Vector2f(8.0f, 0.0f));

    // Brush panel
    brushPanel.setRadius(5.0f);
    brushPanel.setStrength(0.02f);
    brushPanel.setGradient(5.0f);
    listView.addChild(&brushPanel);
    listView.addChild(&separators[1], Vector2f(8.0f, 0.0f));

    // Render view
    RenderView renderView(&camera, &framebuffers[0]);
    {
        renderView.setPosition(200.0f, 0.0f);
        renderView.setSize((float)renderTargetSize.x, (float)renderTargetSize.y);
        renderView.setTexture(&textures[4]);
        renderView.setFlippedUv(true);
        ui.addChild(&renderView);

        renderView.moveToBack();
    }

    ///////////////////////////////////////////////////////////
    
    // Brush events
    renderView.onBrushMove(
        [&](const Vector3f& p)
        {
            Vector2f pos = Vector2f(p.x, p.z);
            editSystem.moveBrush(pos);
        }
    );
    renderView.onBrushUp(std::bind(&EditSystem::finishStroke, &editSystem));

    // Resize event
    window.addListener<E_WindowResize>(
        [&](const E_WindowResize& e)
        {
            renderTargetSize.y = e.m_height;
            renderTargetSize.x = (Uint32)(renderTargetSize.y * 16.0f / 9.0f);

            for (Uint32 i = 0; i < 3; ++i)
            {
                framebuffers[i].reset();
                framebuffers[i].create(renderTargetSize.x, renderTargetSize.y);
                framebuffers[i].attachColor(&textures[2 * i], PixelFormat::Rgb, GLType::Uint16);
                framebuffers[i].attachDepth(&textures[2 * i + 1]);
            }

            renderView.setSize((float)renderTargetSize.x, (float)renderTargetSize.y);
        }
    );

    // Global hokeys
    window.addListener<E_KeyEvent>(
        [&](const E_KeyEvent& e)
        {
            if (e.m_action == InputAction::Release) return;

            if (Window::getCurrent()->isKeyPressed(Keyboard::LeftControl))
            {
                if (e.m_key == Keyboard::Z)
                    editSystem.undo();
                else if (e.m_key == Keyboard::Y)
                    editSystem.redo();

                else if (e.m_key == Keyboard::Tab)
                {
                    // Change color if in color tab
                    if (brushPanel.getMode() == 0)
                    {
                        Uint32 funcIndex = (brushPanel.getHeightFunc() + 1) % 3;
                        brushPanel.setHeightFunc(funcIndex);
                    }
                    else if (brushPanel.getMode() == 1)
                    {
                        Uint32 colorIndex = (brushPanel.getColorSlot() + 1) % 4;
                        brushPanel.setColorSlot(colorIndex);
                    }
                }
            }
            else
            {
                if (e.m_key == Keyboard::Tab)
                    brushPanel.setMode((brushPanel.getMode() + 1) % 3);

                else if (e.m_key == Keyboard::One)
                {
                    if (brushPanel.getMode() == 0)
                        brushPanel.setHeightFunc(0);
                    else if (brushPanel.getMode() == 1)
                        brushPanel.setColorSlot(0);
                }

                else if (e.m_key == Keyboard::Two)
                {
                    if (brushPanel.getMode() == 0)
                        brushPanel.setHeightFunc(1);
                    else if (brushPanel.getMode() == 1)
                        brushPanel.setColorSlot(1);
                }

                else if (e.m_key == Keyboard::Three)
                {
                    if (brushPanel.getMode() == 0)
                        brushPanel.setHeightFunc(2);
                    else if (brushPanel.getMode() == 1)
                        brushPanel.setColorSlot(2);
                }

                else if (e.m_key == Keyboard::Four)
                {
                    if (brushPanel.getMode() == 1)
                        brushPanel.setColorSlot(3);
                }
            }
        }
    );

    // Brush resize events
    window.addListener<E_MouseScroll>(
        [&](const E_MouseScroll& e)
        {
            if (Window::getCurrent()->isKeyPressed(Keyboard::LeftControl))
                brushPanel.setStrength(brushPanel.getStrength() + 0.005f * e.m_dy);
            else if (Window::getCurrent()->isKeyPressed(Keyboard::LeftShift))
                brushPanel.setRadius(brushPanel.getRadius() + e.m_dy);
            else if (Window::getCurrent()->isKeyPressed(Keyboard::LeftAlt))
                brushPanel.setGradient(brushPanel.getGradient() + 0.1f * e.m_dy);
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
        colorAdjust.render(framebuffers[1], framebuffers[2]);

        ui.update(elapsed);
        ui.render(FrameBuffer::Default, false);

        // Swap buffers
        window.display();
    }

    LOG("%d", Profiler::getData("EditSystem::moveBrush").mean().toMicroseconds());
}


/*

More features:

- Import/export terrain maps
- More height brush modes (set/assign, noise, multiply, etc.)
- Global noise
- View options (skybox, lighting, fog, etc.)
- Painting details (grass, rocks, etc.)
- Loading and placing models, and saving scene info
- Adding water to pockets of terrain
- Add wind
- Brush for removing terrain

*/