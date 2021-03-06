#pragma once

#include "common/driver/hidrvc.h"

/*
** HID backend class for Nintendo 3DS
*/
namespace love::driver
{
    class Hidrv : public common::driver::Hidrv
    {
        public:
            Hidrv();

            bool Poll(LOVE_Event * event) override;

            bool IsDown(size_t button) override;

        private:
            circlePosition sticks[2];
            circlePosition oldSticks[2];

            touchPosition touchState;
            touchPosition oldTouchState;

            bool touchHeld;
    };
}