// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Graphics/SurfaceFormat.hpp"
#include "Pomdog/Graphics/DepthFormat.hpp"
#include "Pomdog/Basic/Export.hpp"
#include <functional>
#include <memory>

#import <AppKit/NSWindow.h>
#import <MetalKit/MTKView.h>

@class PomdogOpenGLView;

namespace Pomdog {

class Game;
class GameHost;

namespace Detail {
namespace Cocoa {

class GameHostCocoa;
class GameHostMetal;

} // namespace Cocoa
} // namespace Detail

namespace Cocoa {

class POMDOG_EXPORT Bootstrap final {
public:
    void SetView(PomdogOpenGLView* openGLView);

    void SetSurfaceFormat(SurfaceFormat surfaceFormat);

    void SetDepthFormat(DepthFormat depthFormat);

    void OnError(std::function<void(std::exception const&)> onError);

    void OnCompleted(std::function<void()> onCompleted);

    void Run(std::function<std::unique_ptr<Game>(
        std::shared_ptr<GameHost> const&)> const& createApp);

private:
    std::function<void()> onCompleted;
    std::function<void(std::exception const&)> onError;
    std::shared_ptr<Pomdog::Detail::Cocoa::GameHostCocoa> gameHost;
    std::shared_ptr<Game> game;
    __weak PomdogOpenGLView* openGLView = nil;
    SurfaceFormat surfaceFormat = SurfaceFormat::R8G8B8A8_UNorm;
    DepthFormat depthFormat = DepthFormat::Depth24Stencil8;
};

class POMDOG_EXPORT BootstrapMetal final {
public:
    void SetView(MTKView* view);

    void SetSurfaceFormat(SurfaceFormat surfaceFormat);

    void SetDepthFormat(DepthFormat depthFormat);

    void OnError(std::function<void(std::exception const&)> onError);

    void OnCompleted(std::function<void()> onCompleted);

    void Run(std::function<std::unique_ptr<Game>(
        std::shared_ptr<GameHost> const&)> const& createApp);

    void DrawableSizeWillChange(MTKView* view, CGSize const& size);

    void DrawInMTKView(MTKView* view);

private:
    std::function<void()> onCompleted;
    std::function<void(std::exception const&)> onError;
    std::shared_ptr<Pomdog::Detail::Cocoa::GameHostMetal> gameHost;
    std::shared_ptr<Game> game;
    __weak MTKView* view = nil;
    SurfaceFormat surfaceFormat = SurfaceFormat::R8G8B8A8_UNorm;
    DepthFormat depthFormat = DepthFormat::Depth24Stencil8;
};

} // namespace Cocoa
} // namespace Pomdog
