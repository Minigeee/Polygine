#include <poly/Core/Logger.h>
#include <poly/Core/Sleep.h>

#include <poly/Graphics/Window.h>

#include <iostream>
#include <math.h>

using namespace poly;


int main()
{
    Logger::init("game.log");

    Window window;
    window.create(1280, 720, "Demo App");

    window.addListener<E_KeyEvent>(
        [&](const E_KeyEvent& e)
        {
            if (e.m_action == InputAction::Press)
                LOG("Key press: %d", e.m_key);
        }
    );

    window.addListener<E_MouseButton>(
        [&](const E_MouseButton& e)
        {
            if (e.m_action == InputAction::Press)
                LOG("Button press: %d", e.m_button);
        }
    );

    window.addListener<E_MouseScroll>(
        [&](const E_MouseScroll& e)
        {
            LOG("Mouse scroll: %f", e.m_dy);
        }
    );

    while (window.isOpen())
    {
        window.pollEvents();
        window.display();
    }

    window.close();

    return 0;
}