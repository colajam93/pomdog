// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Application/GameWindow.hpp"
#include "Pomdog/Math/Rectangle.hpp"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <string>
#include <memory>

namespace Pomdog {
namespace Detail {
namespace X11 {

class X11Context;

class GameWindowX11 final : public GameWindow {
public:
    GameWindowX11(
        const std::shared_ptr<X11Context const>& x11Context,
        GLXFBConfig framebufferConfig,
        int width,
        int height);

    ~GameWindowX11();

    bool GetAllowUserResizing() const override;

    void SetAllowUserResizing(bool allowResizing) override;

    std::string GetTitle() const override;

    void SetTitle(const std::string& title) override;

    Rectangle GetClientBounds() const override;

    void SetClientBounds(const Rectangle& clientBounds) override;

    bool IsMouseCursorVisible() const override;

    void SetMouseCursorVisible(bool visible) override;

    void SetMouseCursor(MouseCursor cursor) override;

    ::Display* NativeDisplay() const;

    ::Window NativeWindow() const;

    GLXFBConfig GetFramebufferConfig() const;

    bool IsMinimized() const;

    void ProcessEvent(::XEvent & event);

private:
    std::shared_ptr<X11Context const> x11Context;
    int framebufferConfigID;
    ::Colormap colormap;
    ::Window window;
    std::string title;
    Rectangle clientBounds;
    bool isMinimized;
};

} // namespace X11
} // namespace Detail
} // namespace Pomdog
