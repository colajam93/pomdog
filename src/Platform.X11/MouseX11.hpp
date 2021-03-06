// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Input/Mouse.hpp"
#include "Pomdog/Input/MouseState.hpp"
#include <X11/Xlib.h>

namespace Pomdog {
namespace Detail {
namespace X11 {

class MouseX11 final : public Mouse {
public:
    MouseState GetState() const override;

    void HandleEvent(XEvent & event);

private:
    MouseState mouseState;
};

} // namespace X11
} // namespace Detail
} // namespace Pomdog
