// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "GameHostMetal.hpp"
#include "GameWindowCocoa.hpp"
#include "OpenGLContextCocoa.hpp"
#include "KeyboardCocoa.hpp"
#include "MouseCocoa.hpp"
#include "../RenderSystem/GraphicsContext.hpp"
#include "../RenderSystem/GraphicsCommandQueueImmediate.hpp"
#include "../RenderSystem.GL4/GraphicsContextGL4.hpp"
#include "../RenderSystem.GL4/GraphicsDeviceGL4.hpp"
#include "../Application/SystemEvents.hpp"
#include "Pomdog/Application/Game.hpp"
#include "Pomdog/Application/GameClock.hpp"
#include "Pomdog/Audio/AudioEngine.hpp"
#include "Pomdog/Content/AssetManager.hpp"
#include "Pomdog/Signals/Event.hpp"
#include "Pomdog/Signals/ScopedConnection.hpp"
#include "Pomdog/Graphics/GraphicsCommandQueue.hpp"
#include "Pomdog/Graphics/GraphicsDevice.hpp"
#include "Pomdog/Graphics/PresentationParameters.hpp"
#include "Pomdog/Graphics/Viewport.hpp"
#include "Pomdog/Input/KeyState.hpp"
#include "Pomdog/Logging/Log.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/FileSystem.hpp"
#include "Pomdog/Utility/PathHelper.hpp"
#include "Pomdog/Utility/StringHelper.hpp"
#include <utility>
#include <vector>
#include <mutex>
#include <thread>

namespace Pomdog {
namespace Detail {
namespace Cocoa {
namespace {



} // unnamed namespace
//-----------------------------------------------------------------------
#pragma mark - GameHostMetal::Impl
//-----------------------------------------------------------------------
class GameHostMetal::Impl final {
public:
    Impl(MTKView* metalView,
        std::shared_ptr<GameWindowCocoa> const& window,
        std::shared_ptr<EventQueue> const& eventQueue,
        PresentationParameters const& presentationParameters);

    ~Impl();

    void Run(std::weak_ptr<Game> const& game,
        std::function<void()> const& onCompleted);

    void Exit();

    std::shared_ptr<GameWindow> GetWindow();

    std::shared_ptr<GameClock> GetClock(std::shared_ptr<GameHost> && gameHost);

    std::shared_ptr<GraphicsDevice> GetGraphicsDevice();

    std::shared_ptr<GraphicsCommandQueue> GetGraphicsCommandQueue();

    std::shared_ptr<AssetManager> GetAssetManager(std::shared_ptr<GameHost> && gameHost);

    std::shared_ptr<AudioEngine> GetAudioEngine();

    std::shared_ptr<Keyboard> GetKeyboard();

    std::shared_ptr<Mouse> GetMouse();

private:
    void GameLoop();

    void RenderFrame();

    void DoEvents();

    void ProcessSystemEvents(Event const& event);

    void ClientSizeChanged();

    void GameWillExit();

    static CVReturn DisplayLinkCallback(
        CVDisplayLinkRef displayLink,
        CVTimeStamp const* now,
        CVTimeStamp const* outputTime,
        CVOptionFlags flagsIn,
        CVOptionFlags* flagsOut,
        void* displayLinkContext);

private:
    GameClock clock;
    ScopedConnection systemEventConnection;
    std::mutex renderMutex;
    std::atomic_bool viewLiveResizing;
    std::function<void()> onCompleted;

    std::weak_ptr<Game> weakGame;
    std::shared_ptr<EventQueue> eventQueue;
    std::shared_ptr<GameWindowCocoa> window;
    std::shared_ptr<GraphicsDevice> graphicsDevice;
    std::shared_ptr<Detail::GraphicsContext> graphicsContext;
    std::shared_ptr<GraphicsCommandQueue> graphicsCommandQueue;
    std::shared_ptr<AudioEngine> audioEngine;
    std::unique_ptr<Pomdog::AssetManager> assetManager;
    std::shared_ptr<KeyboardCocoa> keyboard;
    std::shared_ptr<MouseCocoa> mouse;

    __weak MTKView* metalView;
    Duration presentationInterval;
    bool exitRequest;
};
//-----------------------------------------------------------------------
GameHostMetal::Impl::Impl(
    MTKView* metalViewIn,
    std::shared_ptr<GameWindowCocoa> const& windowIn,
    std::shared_ptr<EventQueue> const& eventQueueIn,
    PresentationParameters const& presentationParameters)
    : viewLiveResizing(false)
    , eventQueue(eventQueueIn)
    , window(windowIn)
    , metalView(metalViewIn)
    , presentationInterval(Duration(1) / 60)
    , exitRequest(false)
{
    POMDOG_ASSERT(window);
    window->SetView(metalView);

//    POMDOG_ASSERT(metalView != nil);
//    [metalView setEventQueue: eventQueue];
//    [metalView setOpenGLContext:openGLContext];

//    using Detail::GL4::GraphicsDeviceGL4;
//    graphicsDevice = std::make_shared<Pomdog::GraphicsDevice>(std::make_unique<GraphicsDeviceGL4>());
//
//    graphicsContext = CreateGraphicsContext(openGLContext, window, presentationParameters, graphicsDevice);
//    graphicsCommandQueue = std::make_shared<Pomdog::GraphicsCommandQueue>(
//        std::make_unique<GraphicsCommandQueueImmediate>(graphicsContext));

    // Create subsystems
    audioEngine = std::make_shared<Pomdog::AudioEngine>();
    keyboard = std::make_shared<KeyboardCocoa>();
    mouse = std::make_shared<MouseCocoa>();

    // Connect to system event signal
    POMDOG_ASSERT(eventQueue);
    systemEventConnection = eventQueue->Connect(
        [this](Event const& event) { ProcessSystemEvents(event); });

    Detail::AssetLoaderContext loaderContext;
    loaderContext.RootDirectory = PathHelper::Join(FileSystem::GetResourceDirectoryPath(), "Content");
    loaderContext.GraphicsDevice = graphicsDevice;
    assetManager = std::make_unique<Pomdog::AssetManager>(std::move(loaderContext));

    POMDOG_ASSERT(presentationParameters.PresentationInterval > 0);
    presentationInterval = Duration(1) / presentationParameters.PresentationInterval;
}
//-----------------------------------------------------------------------
GameHostMetal::Impl::~Impl()
{
//    if (metalView != nil) {
//        [metalView setEventQueue:{}];
//    }

    systemEventConnection.Disconnect();
    assetManager.reset();
    keyboard.reset();
    mouse.reset();
    audioEngine.reset();
    graphicsCommandQueue.reset();
    graphicsContext.reset();
    graphicsDevice.reset();
    window.reset();
    eventQueue.reset();
    metalView = nil;
}
//-----------------------------------------------------------------------
void GameHostMetal::Impl::Run(std::weak_ptr<Game> const& weakGameIn,
    std::function<void()> const& onCompletedIn)
{
    POMDOG_ASSERT(!weakGameIn.expired());
    POMDOG_ASSERT(onCompletedIn);
    weakGame = weakGameIn;
    onCompleted = onCompletedIn;

    POMDOG_ASSERT(!weakGame.expired());
    auto game = weakGame.lock();

    game->Initialize();

    if (exitRequest) {
        GameWillExit();
        return;
    }
}
//-----------------------------------------------------------------------
void GameHostMetal::Impl::GameWillExit()
{
//    if (openGLView != nil) {
//        [openGLView setRenderCallback: []{}];
//    }

    if (window) {
        window->SetView(nil);
    }

    if (onCompleted) {
        dispatch_async(dispatch_get_main_queue(), [=] {
            onCompleted();
        });
    }
}
//-----------------------------------------------------------------------
void GameHostMetal::Impl::Exit()
{
    exitRequest = true;
    GameWillExit();
}
//-----------------------------------------------------------------------
CVReturn GameHostMetal::Impl::DisplayLinkCallback(
    CVDisplayLinkRef displayLink,
    CVTimeStamp const* now,
    CVTimeStamp const* outputTime,
    CVOptionFlags flagsIn,
    CVOptionFlags* flagsOut,
    void* displayLinkContext)
{
    auto gameHost = reinterpret_cast<GameHostMetal::Impl*>(displayLinkContext);
    POMDOG_ASSERT(gameHost != nullptr);
    gameHost->GameLoop();
    return kCVReturnSuccess;
}
//-----------------------------------------------------------------------
void GameHostMetal::Impl::GameLoop()
{
    POMDOG_ASSERT(!exitRequest);
    POMDOG_ASSERT(!weakGame.expired());

    std::lock_guard<std::mutex> lock(renderMutex);

    auto game = weakGame.lock();
    POMDOG_ASSERT(game);

    clock.Tick();
    DoEvents();

    if (exitRequest) {
        return;
    }

    game->Update();

    if (!viewLiveResizing.load()) {
        RenderFrame();
    }

    {
        auto elapsedTime = clock.GetElapsedTime();

        if (elapsedTime < presentationInterval) {
            lock.~lock_guard();
            auto sleepTime = (presentationInterval - elapsedTime);
            std::this_thread::sleep_for(sleepTime);
        }
    }
}
//-----------------------------------------------------------------------
void GameHostMetal::Impl::RenderFrame()
{
    POMDOG_ASSERT(window);
    POMDOG_ASSERT(!weakGame.expired());

    bool skipRender = (!window || window->IsMinimized()
        || [NSApp isHidden] == YES);

    if (skipRender) {
        return;
    }

    auto game = weakGame.lock();

    POMDOG_ASSERT(game);

    game->Draw();
}
//-----------------------------------------------------------------------
void GameHostMetal::Impl::DoEvents()
{
    eventQueue->Emit();
}
//-----------------------------------------------------------------------
void GameHostMetal::Impl::ProcessSystemEvents(Event const& event)
{
    if (event.Is<WindowShouldCloseEvent>())
    {
        Log::Internal("WindowShouldCloseEvent");
        this->Exit();
    }
    else if (event.Is<WindowWillCloseEvent>())
    {
        Log::Internal("WindowWillCloseEvent");
    }
    else if (event.Is<ViewWillStartLiveResizeEvent>())
    {
        auto rect = window->GetClientBounds();
        Log::Internal(StringHelper::Format(
            "ViewWillStartLiveResizeEvent: {w: %d, h: %d}",
            rect.Width, rect.Height));
    }
    else if (event.Is<ViewDidEndLiveResizeEvent>())
    {
        auto rect = window->GetClientBounds();
        Log::Internal(StringHelper::Format(
            "ViewDidEndLiveResizeEvent: {w: %d, h: %d}",
            rect.Width, rect.Height));
    }
    else if (auto keyEvent = event.As<InputKeyEvent>())
    {
        POMDOG_ASSERT(keyboard);
        keyboard->SetKey(keyEvent->Key, keyEvent->State);
    }
    else if (auto inputTextEvent = event.As<InputTextEvent>())
    {
        POMDOG_ASSERT(keyboard);
        keyboard->Keyboard::TextInput(inputTextEvent->text);
    }
    else if (auto mousePositionEvent = event.As<MousePositionEvent>())
    {
        POMDOG_ASSERT(mouse);
        mouse->Position(mousePositionEvent->Position);
    }
    else if (auto mouseButtonEvent = event.As<MouseButtonEvent>())
    {
        POMDOG_ASSERT(mouse);
        if (mouseButtonEvent->Button == MouseButtons::Left) {
            mouse->LeftButton(mouseButtonEvent->State == MouseButtonState::Up
                ? ButtonState::Released
                : ButtonState::Pressed);
        }
        else if (mouseButtonEvent->Button == MouseButtons::Right) {
            mouse->RightButton(mouseButtonEvent->State == MouseButtonState::Up
                ? ButtonState::Released
                : ButtonState::Pressed);
        }
        else if (mouseButtonEvent->Button == MouseButtons::Middle) {
            mouse->MiddleButton(mouseButtonEvent->State == MouseButtonState::Up
                ? ButtonState::Released
                : ButtonState::Pressed);
        }
        else if (mouseButtonEvent->Button == MouseButtons::XButton1) {
            mouse->XButton1(mouseButtonEvent->State == MouseButtonState::Up
                ? ButtonState::Released
                : ButtonState::Pressed);
        }
        else if (mouseButtonEvent->Button == MouseButtons::XButton2) {
            mouse->XButton2(mouseButtonEvent->State == MouseButtonState::Up
                ? ButtonState::Released
                : ButtonState::Pressed);
        }
        mouse->Position(mouseButtonEvent->Position);
    }
    else if (auto scrollWheelEvent = event.As<ScrollWheelEvent>())
    {
        POMDOG_ASSERT(mouse);
        mouse->WheelDelta(scrollWheelEvent->ScrollingDeltaY);
    }
}
//-----------------------------------------------------------------------
void GameHostMetal::Impl::ClientSizeChanged()
{
    auto bounds = window->GetClientBounds();
    window->ClientSizeChanged(bounds.Width, bounds.Height);
}
//-----------------------------------------------------------------------
std::shared_ptr<GameWindow> GameHostMetal::Impl::GetWindow()
{
    return window;
}
//-----------------------------------------------------------------------
std::shared_ptr<GameClock> GameHostMetal::Impl::GetClock(std::shared_ptr<GameHost> && gameHost)
{
    std::shared_ptr<GameClock> sharedClock(gameHost, &clock);
    return std::move(sharedClock);
}
//-----------------------------------------------------------------------
std::shared_ptr<GraphicsDevice> GameHostMetal::Impl::GetGraphicsDevice()
{
    return graphicsDevice;
}
//-----------------------------------------------------------------------
std::shared_ptr<GraphicsCommandQueue> GameHostMetal::Impl::GetGraphicsCommandQueue()
{
    return graphicsCommandQueue;
}
//-----------------------------------------------------------------------
std::shared_ptr<AudioEngine> GameHostMetal::Impl::GetAudioEngine()
{
    return audioEngine;
}
//-----------------------------------------------------------------------
std::shared_ptr<AssetManager> GameHostMetal::Impl::GetAssetManager(std::shared_ptr<GameHost> && gameHost)
{
    std::shared_ptr<AssetManager> sharedAssetManager(gameHost, assetManager.get());
    return std::move(sharedAssetManager);
}
//-----------------------------------------------------------------------
std::shared_ptr<Keyboard> GameHostMetal::Impl::GetKeyboard()
{
    return keyboard;
}
//-----------------------------------------------------------------------
std::shared_ptr<Mouse> GameHostMetal::Impl::GetMouse()
{
    return mouse;
}
//-----------------------------------------------------------------------
#pragma mark - GameHostMetal
//-----------------------------------------------------------------------
GameHostMetal::GameHostMetal(
    MTKView* metalView,
    std::shared_ptr<GameWindowCocoa> const& window,
    std::shared_ptr<EventQueue> const& eventQueue,
    PresentationParameters const& presentationParameters)
    : impl(std::make_unique<Impl>(metalView, window, eventQueue, presentationParameters))
{}
//-----------------------------------------------------------------------
GameHostMetal::~GameHostMetal() = default;
//-----------------------------------------------------------------------
void GameHostMetal::Run(std::weak_ptr<Game> const& game,
    std::function<void()> const& onCompleted)
{
    POMDOG_ASSERT(impl);
    impl->Run(game, onCompleted);
}
//-----------------------------------------------------------------------
void GameHostMetal::Exit()
{
    POMDOG_ASSERT(impl);
    impl->Exit();
}
//-----------------------------------------------------------------------
std::shared_ptr<GameWindow> GameHostMetal::GetWindow()
{
    POMDOG_ASSERT(impl);
    return impl->GetWindow();
}
//-----------------------------------------------------------------------
std::shared_ptr<GameClock> GameHostMetal::GetClock()
{
    POMDOG_ASSERT(impl);
    return impl->GetClock(shared_from_this());
}
//-----------------------------------------------------------------------
std::shared_ptr<GraphicsDevice> GameHostMetal::GetGraphicsDevice()
{
    POMDOG_ASSERT(impl);
    return impl->GetGraphicsDevice();
}
//-----------------------------------------------------------------------
std::shared_ptr<GraphicsCommandQueue> GameHostMetal::GetGraphicsCommandQueue()
{
    POMDOG_ASSERT(impl);
    return impl->GetGraphicsCommandQueue();
}
//-----------------------------------------------------------------------
std::shared_ptr<AudioEngine> GameHostMetal::GetAudioEngine()
{
    POMDOG_ASSERT(impl);
    return impl->GetAudioEngine();
}
//-----------------------------------------------------------------------
std::shared_ptr<AssetManager> GameHostMetal::GetAssetManager()
{
    POMDOG_ASSERT(impl);
    return impl->GetAssetManager(shared_from_this());
}
//-----------------------------------------------------------------------
std::shared_ptr<Keyboard> GameHostMetal::GetKeyboard()
{
    POMDOG_ASSERT(impl);
    return impl->GetKeyboard();
}
//-----------------------------------------------------------------------
std::shared_ptr<Mouse> GameHostMetal::GetMouse()
{
    POMDOG_ASSERT(impl);
    return impl->GetMouse();
}
//-----------------------------------------------------------------------
} // namespace Cocoa
} // namespace Detail
} // namespace Pomdog
