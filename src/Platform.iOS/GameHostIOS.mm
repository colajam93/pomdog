// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "GameHostIOS.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/Exception.hpp"

namespace Pomdog {
namespace Detail {
namespace iOS {

class GameHostIOS::Impl final {
};

//-----------------------------------------------------------------------
GameHostIOS::GameHostIOS(
    PomdogMetalView* openGLView,
    std::shared_ptr<GameWindowIOS> const& window,
    std::shared_ptr<EventQueue> const& eventQueue,
    PresentationParameters const& presentationParameters)
    : impl(std::make_unique<Impl>())
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
GameHostIOS::~GameHostIOS() = default;
//-----------------------------------------------------------------------
void GameHostIOS::Run(
    std::weak_ptr<Game> const& game,
    std::function<void()> const& onCompleted)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
void GameHostIOS::Exit()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GameWindow> GameHostIOS::Window()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GameClock> GameHostIOS::Clock()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GraphicsDevice> GameHostIOS::GraphicsDevice()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GraphicsCommandQueue> GameHostIOS::GraphicsCommandQueue()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::AudioEngine> GameHostIOS::AudioEngine()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::AssetManager> GameHostIOS::AssetManager()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::Keyboard> GameHostIOS::Keyboard()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::Mouse> GameHostIOS::Mouse()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
} // namespace iOS
} // namespace Detail
} // namespace Pomdog
