// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Input/GamepadButtons.hpp"
#include "Pomdog/Input/GamepadDPad.hpp"
#include "Pomdog/Input/GamepadThumbSticks.hpp"

namespace Pomdog {

struct GamepadState {
    GamepadButtons Buttons;
    GamepadThumbSticks ThumbSticks;
    GamepadDPad DPad;
    bool IsConnected = false;
};

} // namespace Pomdog
