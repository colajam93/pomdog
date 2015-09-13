// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Signals/EventQueue.hpp"
#include "Pomdog/Application/GameHost.hpp"
#include <functional>
#include <memory>

@class PomdogMetalView;

namespace Pomdog {

class Game;
struct PresentationParameters;

namespace Detail {
namespace iOS {

class GameWindowIOS;

class GameHostIOS final : public GameHost {
public:
    GameHostIOS(
        PomdogMetalView* openGLView,
        std::shared_ptr<GameWindowIOS> const& window,
        std::shared_ptr<EventQueue> const& eventQueue,
        PresentationParameters const& presentationParameters);

    ~GameHostIOS();

    void Run(
        std::weak_ptr<Game> const& game,
        std::function<void()> const& onCompleted);

    void Exit() override;

    std::shared_ptr<Pomdog::GameWindow> Window() override;

    std::shared_ptr<Pomdog::GameClock> Clock() override;

    std::shared_ptr<Pomdog::GraphicsDevice> GraphicsDevice() override;

    std::shared_ptr<Pomdog::GraphicsCommandQueue> GraphicsCommandQueue() override;

    std::shared_ptr<Pomdog::AudioEngine> AudioEngine() override;

    std::shared_ptr<Pomdog::AssetManager> AssetManager() override;

    std::shared_ptr<Pomdog::Keyboard> Keyboard() override;

    std::shared_ptr<Pomdog::Mouse> Mouse() override;

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace iOS
} // namespace Detail
} // namespace Pomdog
