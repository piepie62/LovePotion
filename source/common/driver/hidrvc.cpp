#include "common/driver/hidrvc.h"

using namespace love::common::driver;

Hidrv::Hidrv() : hysteresis(false),
                 events(),
                 buttonStates()
{}

uint64_t Hidrv::GetButtonPressed()
{
    return this->buttonStates.pressed;
}

uint64_t Hidrv::GetButtonReleased()
{
    return this->buttonStates.released;
}

uint64_t Hidrv::GetButtonHeld()
{
    return this->buttonStates.held;
}

void Hidrv::SendFocus(bool focus)
{
    auto & event = this->events.emplace_back();

    event.type = TYPE_WINDOWEVENT;
    event.subType = focus ? TYPE_FOCUS_GAINED : TYPE_FOCUS_LOST;
}

void Hidrv::SendLowMemory()
{
    auto & event = this->events.emplace_back();

    event.type = TYPE_LOWMEMORY;
}

void Hidrv::SendQuit()
{
    auto & event = this->events.emplace_back();

    event.type = TYPE_QUIT;
}

/* onlu fires on Switch */
void Hidrv::SendResize(int width, int height)
{
    auto & event = this->events.emplace_back();

    event.type = TYPE_WINDOWEVENT;
    event.subType = TYPE_RESIZE;

    event.size.width  = width;
    event.size.height = height;
}