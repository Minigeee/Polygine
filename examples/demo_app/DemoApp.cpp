#include <poly/Core/Logger.h>
#include <poly/Core/Sleep.h>

#include <poly/Graphics/Window.h>

#include <iostream>
#include <math.h>

using namespace poly;

struct MsgEvent
{
    std::string m_message;
};


int main()
{
    Logger::init("game.log");

    Window window;
    window.create(1280, 720, "Demo App");

    sleep(5.0f);

    return 0;
}