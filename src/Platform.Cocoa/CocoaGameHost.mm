﻿//
//  Copyright (C) 2013-2014 mogemimi.
//  Distributed under the MIT License. See LICENSE.md or
//  http://enginetrouble.net/pomdog/license for details.
//

#include "CocoaGameHost.hpp"
#include "CocoaGameWindow.hpp"
#include "CocoaOpenGLContext.hpp"
#include "KeyboardCocoa.hpp"
#include "MouseCocoa.hpp"
#include "../RenderSystem/PresentationParameters.hpp"
#include "../RenderSystem.GL4/GraphicsContextGL4.hpp"
#include "../RenderSystem.GL4/GraphicsDeviceGL4.hpp"
#include "Pomdog/Application/Game.hpp"
#include "Pomdog/Application/GameClock.hpp"
#include "Pomdog/Audio/AudioEngine.hpp"
#include "Pomdog/Content/AssetManager.hpp"
#include "Pomdog/Event/Event.hpp"
#include "Pomdog/Event/ScopedConnection.hpp"
#include "Pomdog/Graphics/GraphicsContext.hpp"
#include "Pomdog/Graphics/GraphicsDevice.hpp"
#include "Pomdog/Graphics/Viewport.hpp"
#include "Pomdog/Input/KeyState.hpp"
#include "Pomdog/Logging/Log.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/StringFormat.hpp"
#include <OpenGL/OpenGL.h>
#include <utility>
#include <vector>
#include <mutex>
#include <thread>

namespace Pomdog {
namespace Details {
namespace Cocoa {
namespace {

//-----------------------------------------------------------------------
#pragma mark - OpenGL Helper Functions
//-----------------------------------------------------------------------
static NSOpenGLPixelFormat* CreatePixelFormat(DepthFormat depthFormat)
{
	std::vector<NSOpenGLPixelFormatAttribute> attributes =
	{
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAColorSize, 24,
		NSOpenGLPFAAlphaSize, 8,
		NSOpenGLPFAAllowOfflineRenderers,
	};
	
	switch (depthFormat) {
	case DepthFormat::Depth16:
		attributes.push_back(NSOpenGLPFADepthSize);
		attributes.push_back(16);
		break;
	case DepthFormat::Depth24Stencil8:
		attributes.push_back(NSOpenGLPFADepthSize);
		attributes.push_back(24);
		attributes.push_back(NSOpenGLPFAStencilSize);
		attributes.push_back(8);
		break;
	case DepthFormat::Depth32:
		attributes.push_back(NSOpenGLPFADepthSize);
		attributes.push_back(32);
		break;
	case DepthFormat::None:
		break;
	}
	
	attributes.push_back(0);
	return [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes.data()];
}
//-----------------------------------------------------------------------
static std::shared_ptr<CocoaOpenGLContext> CreateOpenGLContext(DepthFormat depthFormat)
{
	auto pixelFormat = CreatePixelFormat(depthFormat);
	return std::make_shared<CocoaOpenGLContext>(pixelFormat);
}
//-----------------------------------------------------------------------
static std::shared_ptr<GraphicsContext> CreateGraphicsContext(
	std::shared_ptr<CocoaOpenGLContext> const& openGLContext,
	std::weak_ptr<GameWindow> && gameWindow,
	RenderSystem::PresentationParameters const& presentationParameters,
	std::shared_ptr<GraphicsDevice> const& graphicsDevice)
{
	POMDOG_ASSERT(openGLContext);
	POMDOG_ASSERT(!gameWindow.expired());
	using RenderSystem::GL4::GraphicsContextGL4;

	auto nativeContext = std::make_unique<GraphicsContextGL4>(openGLContext, std::move(gameWindow));
	return std::make_shared<GraphicsContext>(std::move(nativeContext), presentationParameters, graphicsDevice);
}

}// unnamed namespace
//-----------------------------------------------------------------------
#pragma mark - CocoaGameHost::Impl
//-----------------------------------------------------------------------
class CocoaGameHost::Impl final {
public:
	Impl(std::shared_ptr<CocoaGameWindow> const& window,
		std::shared_ptr<SystemEventDispatcher> const& dipatcher,
		RenderSystem::PresentationParameters const& presentationParameters);

	~Impl() = default;

	void Run(Game & game);

	void Exit();

	std::shared_ptr<Pomdog::GameWindow> Window();

	std::shared_ptr<Pomdog::GameClock> Clock(std::shared_ptr<GameHost> && gameHost);

	std::shared_ptr<Pomdog::GraphicsContext> GraphicsContext();

	std::shared_ptr<Pomdog::GraphicsDevice> GraphicsDevice();

	std::shared_ptr<Pomdog::AssetManager> AssetManager(std::shared_ptr<GameHost> && gameHost);

	std::shared_ptr<Pomdog::AudioEngine> AudioEngine();

	std::shared_ptr<Pomdog::Keyboard> Keyboard();

	std::shared_ptr<Pomdog::Mouse> Mouse();

private:
	void RenderFrame(Game & game);
	
	void DoEvents();
	
	void ProcessSystemEvents(Event const& event);

	void ClientSizeChanged();

private:
	GameClock clock;
	std::mutex renderMutex;
	std::atomic_bool viewLiveResizing;

	//std::weak_ptr<Game> game;
	std::shared_ptr<CocoaGameWindow> gameWindow;

	std::shared_ptr<SystemEventDispatcher> systemEventDispatcher;
	ScopedConnection systemEventConnection;
	
	std::shared_ptr<CocoaOpenGLContext> openGLContext;
	std::shared_ptr<Pomdog::GraphicsContext> graphicsContext;
	std::shared_ptr<Pomdog::GraphicsDevice> graphicsDevice;
	std::shared_ptr<Pomdog::AudioEngine> audioEngine;
	std::unique_ptr<Pomdog::AssetManager> assetManager;
	
	std::shared_ptr<KeyboardCocoa> keyboard;
	std::shared_ptr<MouseCocoa> mouse;
	
	bool exitRequest;
};
//-----------------------------------------------------------------------
CocoaGameHost::Impl::Impl(std::shared_ptr<CocoaGameWindow> const& window,
	std::shared_ptr<SystemEventDispatcher> const& eventDispatcher,
	RenderSystem::PresentationParameters const& presentationParameters)
	: viewLiveResizing(false)
	, gameWindow(window)
	, systemEventDispatcher(eventDispatcher)
	, exitRequest(false)
{
	openGLContext = CreateOpenGLContext(presentationParameters.DepthFormat);
	
	using Details::RenderSystem::GL4::GraphicsDeviceGL4;
	graphicsDevice = std::make_shared<Pomdog::GraphicsDevice>(std::make_unique<GraphicsDeviceGL4>());

	graphicsContext = CreateGraphicsContext(openGLContext, gameWindow, presentationParameters, graphicsDevice);
	
	audioEngine = std::make_shared<Pomdog::AudioEngine>();
	
	POMDOG_ASSERT(gameWindow);
	gameWindow->ResetGLContext(openGLContext);
	
	POMDOG_ASSERT(systemEventDispatcher);
	systemEventConnection = systemEventDispatcher->Connect([this](Event const& event){
		ProcessSystemEvents(event);
	});
	
	keyboard = std::make_shared<KeyboardCocoa>();
	mouse = std::make_shared<MouseCocoa>();
	gameWindow->BindToDelegate(mouse);
	
	{
		NSString* path = [[NSBundle mainBundle] resourcePath];
		std::string rootDirectory = [[path stringByAppendingPathComponent: @"Content"] UTF8String];
		Details::AssetLoaderContext loaderContext{rootDirectory, graphicsDevice};
		
		assetManager = std::make_unique<Pomdog::AssetManager>(std::move(loaderContext));
	}
}
//-----------------------------------------------------------------------
void CocoaGameHost::Impl::Run(Game & game)
{
	openGLContext->LockContext();
	openGLContext->MakeCurrentContext();
	game.Initialize();
	openGLContext->UnlockContext();

	if (!game.CompleteInitialize()) {
		return;
	}
	
	gameWindow->SetRenderCallbackOnLiveResizing([&] {
		std::lock_guard<std::mutex> lock(renderMutex);
		ClientSizeChanged();
		RenderFrame(game);
	});
	
	while (!exitRequest)
	{
		std::lock_guard<std::mutex> lock(renderMutex);

		clock.Tick();
		DoEvents();
		game.Update();
		
		if (!viewLiveResizing.load()) {
			RenderFrame(game);
		}

		auto const interval = DurationSeconds(1.0/60.0);
		auto elapsedTime = clock.ElapsedTime();
		
		if (elapsedTime < interval) {
			lock.~lock_guard();
			auto sleepTime = (interval - elapsedTime);
			std::this_thread::sleep_for(sleepTime);
		}
	}
	
	gameWindow->SetRenderCallbackOnLiveResizing();

	gameWindow->Close();
	//DoEvents();
}
//-----------------------------------------------------------------------
void CocoaGameHost::Impl::Exit()
{
	exitRequest = true;
}
//-----------------------------------------------------------------------
void CocoaGameHost::Impl::RenderFrame(Game & game)
{
	POMDOG_ASSERT(gameWindow);

	bool skipRender = (!gameWindow
		|| gameWindow->IsMinimized()
		|| [NSApp isHidden] == YES);

	if (skipRender) {
		return;
	}

	openGLContext->LockContext();
	openGLContext->MakeCurrentContext();

	game.Draw();

	openGLContext->UnlockContext();
}
//-----------------------------------------------------------------------
void CocoaGameHost::Impl::DoEvents()
{
	systemEventDispatcher->Tick();
}
//-----------------------------------------------------------------------
void CocoaGameHost::Impl::ProcessSystemEvents(Event const& event)
{
	if (event.Is<WindowShouldCloseEvent>())
	{
		Log::Internal("WindowShouldCloseEvent");
		this->Exit();
	}
	else if (event.Is<WindowWillCloseEvent>())
	{
		Log::Internal("WindowWillCloseEvent");
		///@todo Not implemented
	}
	else if (event.Is<ViewWillStartLiveResizeEvent>())
	{
		viewLiveResizing = true;

		auto rect = gameWindow->ClientBounds();
		Log::Internal(StringFormat("ViewWillStartLiveResizeEvent: {w: %d, h: %d}",
			rect.Width, rect.Height));
	}
	else if (event.Is<ViewDidEndLiveResizeEvent>())
	{
		viewLiveResizing = false;
		
		auto rect = gameWindow->ClientBounds();
		Log::Internal(StringFormat("ViewDidEndLiveResizeEvent: {w: %d, h: %d}",
			rect.Width, rect.Height));
	}
	else if (auto keyDownEvent = event.As<InputKeyDownEvent>())
	{
		//Log::Internal(StringFormat("KeyDown: %d", static_cast<int>(keyDownEvent->Key)));

		POMDOG_ASSERT(keyboard);
		keyboard->SetKey(keyDownEvent->Key, KeyState::Down);
	}
	else if (auto keyUpEvent = event.As<InputKeyUpEvent>())
	{
		//Log::Internal(StringFormat("KeyUp: %d", static_cast<int>(keyUpEvent->Key)));
		
		POMDOG_ASSERT(keyboard);
		keyboard->SetKey(keyUpEvent->Key, KeyState::Up);
	}
}
//-----------------------------------------------------------------------
void CocoaGameHost::Impl::ClientSizeChanged()
{
	openGLContext->LockContext();
	openGLContext->MakeCurrentContext();
	{
		POMDOG_ASSERT(openGLContext->NativeOpenGLContext() != nil);
		[openGLContext->NativeOpenGLContext() update];
	
		auto bounds = gameWindow->ClientBounds();
		gameWindow->ClientSizeChanged(bounds.Width, bounds.Height);
	}
	openGLContext->UnlockContext();
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GameWindow> CocoaGameHost::Impl::Window()
{
	return gameWindow;
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GameClock> CocoaGameHost::Impl::Clock(std::shared_ptr<GameHost> && gameHost)
{
	std::shared_ptr<Pomdog::GameClock> sharedClock(gameHost, &clock);
	return std::move(sharedClock);
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GraphicsContext> CocoaGameHost::Impl::GraphicsContext()
{
	return graphicsContext;
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GraphicsDevice> CocoaGameHost::Impl::GraphicsDevice()
{
	return graphicsDevice;
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::AudioEngine> CocoaGameHost::Impl::AudioEngine()
{
	return audioEngine;
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::AssetManager> CocoaGameHost::Impl::AssetManager(std::shared_ptr<GameHost> && gameHost)
{
	std::shared_ptr<Pomdog::AssetManager> sharedAssetManager(gameHost, assetManager.get());
	return std::move(sharedAssetManager);
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::Keyboard> CocoaGameHost::Impl::Keyboard()
{
	return keyboard;
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::Mouse> CocoaGameHost::Impl::Mouse()
{
	return mouse;
}
//-----------------------------------------------------------------------
#pragma mark - CocoaGameHost
//-----------------------------------------------------------------------
CocoaGameHost::CocoaGameHost(std::shared_ptr<CocoaGameWindow> const& window,
	std::shared_ptr<SystemEventDispatcher> const& eventDispatcher,
	RenderSystem::PresentationParameters const& presentationParameters)
	: impl(std::make_unique<Impl>(window, eventDispatcher, presentationParameters))
{}
//-----------------------------------------------------------------------
CocoaGameHost::~CocoaGameHost() = default;
//-----------------------------------------------------------------------
void CocoaGameHost::Run(Game & game)
{
	POMDOG_ASSERT(impl);
	impl->Run(game);
}
//-----------------------------------------------------------------------
void CocoaGameHost::Exit()
{
	POMDOG_ASSERT(impl);
	impl->Exit();
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GameWindow> CocoaGameHost::Window()
{
	POMDOG_ASSERT(impl);
	return impl->Window();
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GameClock> CocoaGameHost::Clock()
{
	POMDOG_ASSERT(impl);
	return impl->Clock(shared_from_this());
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GraphicsContext> CocoaGameHost::GraphicsContext()
{
	POMDOG_ASSERT(impl);
	return impl->GraphicsContext();
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::GraphicsDevice> CocoaGameHost::GraphicsDevice()
{
	POMDOG_ASSERT(impl);
	return impl->GraphicsDevice();
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::AudioEngine> CocoaGameHost::AudioEngine()
{
	POMDOG_ASSERT(impl);
	return impl->AudioEngine();
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::AssetManager> CocoaGameHost::AssetManager()
{
	POMDOG_ASSERT(impl);
	return impl->AssetManager(shared_from_this());
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::Keyboard> CocoaGameHost::Keyboard()
{
	POMDOG_ASSERT(impl);
	return impl->Keyboard();
}
//-----------------------------------------------------------------------
std::shared_ptr<Pomdog::Mouse> CocoaGameHost::Mouse()
{
	POMDOG_ASSERT(impl);
	return impl->Mouse();
}
//-----------------------------------------------------------------------
}// namespace Cocoa
}// namespace Details
}// namespace Pomdog
