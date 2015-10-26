// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Signals/EventQueue.hpp"
#include "Pomdog/Application/GameHost.hpp"
#include <memory>

#import <MetalKit/MTKView.h>

namespace Pomdog {

class Game;
struct PresentationParameters;

namespace Detail {
namespace Cocoa {

class GameWindowCocoa;

class GameHostMetal final: public GameHost {
public:
    GameHostMetal(
        MTKView* metalView,
        std::shared_ptr<GameWindowCocoa> const& window,
        std::shared_ptr<EventQueue> const& eventQueue,
        PresentationParameters const& presentationParameters);

    ~GameHostMetal();

    void Run(
        std::weak_ptr<Game> const& game,
        std::function<void()> const& onCompleted);

    void Exit() override;

    std::shared_ptr<GameWindow> GetWindow() override;

    std::shared_ptr<GameClock> GetClock() override;

    std::shared_ptr<GraphicsDevice> GetGraphicsDevice() override;

    std::shared_ptr<GraphicsCommandQueue> GetGraphicsCommandQueue() override;

    std::shared_ptr<AudioEngine> GetAudioEngine() override;

    std::shared_ptr<AssetManager> GetAssetManager() override;

    std::shared_ptr<Keyboard> GetKeyboard() override;

    std::shared_ptr<Mouse> GetMouse() override;

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace Cocoa
} // namespace Detail
} // namespace Pomdog
