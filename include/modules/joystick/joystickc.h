#pragma once

#include "common/module.h"

#include "objects/gamepad/gamepad.h"

#include <vector>
#include <list>

namespace love::common
{
    class Joystick : public Module
    {
        public:
            Joystick();

            ~Joystick();

            ModuleType GetModuleType() const { return M_JOYSTICK; }

            const char * GetName() const override { return "love.joystick"; }

            love::Gamepad * GetJoystickFromID(size_t index);

            size_t GetJoystickCount() const;

            love::Gamepad * AddGamepad(size_t index);

            void RemoveGamepad(love::Gamepad * gamepad);

            int GetIndex(const love::Gamepad * gamepad);

        private:
            std::vector<love::Gamepad *> active;

            std::list<love::Gamepad *> gamepads;
    };
}
