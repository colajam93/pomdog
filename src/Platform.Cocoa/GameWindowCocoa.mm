// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#include "GameWindowCocoa.hpp"
#include "OpenGLContextCocoa.hpp"
#include "CocoaWindowDelegate.hpp"
#include "Pomdog/Application/MouseCursor.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include <utility>

namespace Pomdog {
namespace Detail {
namespace Cocoa {
//-----------------------------------------------------------------------
GameWindowCocoa::GameWindowCocoa(NSWindow* nativeWindowIn,
    std::shared_ptr<EventQueue> const& eventQueueIn)
    : eventQueue(eventQueueIn)
    , nativeWindow(nativeWindowIn)
    , gameView(nil)
    , windowDelegate(nil)
    , isMouseCursorVisible(true)
{
    POMDOG_ASSERT(nativeWindow != nil);

    // Create WindowDelegate
    windowDelegate = [[CocoaWindowDelegate alloc] initWithEventQueue:eventQueue];
    [nativeWindow setDelegate:windowDelegate];
}
//-----------------------------------------------------------------------
GameWindowCocoa::~GameWindowCocoa()
{
    // Remove delegate from window
    [nativeWindow setDelegate:nil];

    windowDelegate = nil;
    nativeWindow = nil;
}
//-----------------------------------------------------------------------
bool GameWindowCocoa::AllowPlayerResizing() const
{
    NSUInteger styleMask = [nativeWindow styleMask];
    return (styleMask & NSResizableWindowMask) == NSResizableWindowMask;
}
//-----------------------------------------------------------------------
void GameWindowCocoa::AllowPlayerResizing(bool allowResizing)
{
    POMDOG_ASSERT(nativeWindow != nil);

    NSUInteger styleMask = [nativeWindow styleMask];
    if (allowResizing) {
        styleMask |= NSResizableWindowMask;
        POMDOG_ASSERT((styleMask & NSResizableWindowMask) == NSResizableWindowMask);
    }
    else {
        styleMask |= NSResizableWindowMask;
        styleMask ^= NSResizableWindowMask;
        POMDOG_ASSERT((styleMask & NSResizableWindowMask) != NSResizableWindowMask);
    }

    dispatch_async(dispatch_get_main_queue(), [=] {
        [nativeWindow setStyleMask:styleMask];
    });
}
//-----------------------------------------------------------------------
std::string GameWindowCocoa::Title() const
{
    std::string title = [[nativeWindow title] UTF8String];
    return std::move(title);
}
//-----------------------------------------------------------------------
void GameWindowCocoa::Title(std::string const& title)
{
    dispatch_async(dispatch_get_main_queue(), [=] {
        [nativeWindow setTitle:[NSString stringWithUTF8String:title.c_str()]];
    });
}
//-----------------------------------------------------------------------
Rectangle GameWindowCocoa::ClientBounds() const
{
    POMDOG_ASSERT([nativeWindow contentView] != nil);
    NSRect bounds = [[nativeWindow contentView] bounds];
    NSPoint origin = [nativeWindow frame].origin;

    if (gameView != nil) {
        bounds = [gameView bounds];
        origin = [gameView frame].origin;
    }

    NSSize windowSize = [nativeWindow frame].size;
    NSSize screenSize = [[nativeWindow screen] visibleFrame].size;

    return Rectangle(
        origin.x,
        screenSize.height - windowSize.height - origin.y,
        bounds.size.width,
        bounds.size.height);
}
//-----------------------------------------------------------------------
void GameWindowCocoa::ClientBounds(Rectangle const& clientBounds)
{
    auto bounds = NSMakeSize(clientBounds.Width, clientBounds.Height);
    [nativeWindow setContentSize:bounds];

    NSSize windowSize = [nativeWindow frame].size;
    NSSize screenSize = [[nativeWindow screen] visibleFrame].size;

    auto origin = NSMakePoint(clientBounds.X,
        screenSize.height - (clientBounds.Y + windowSize.height));

    dispatch_async(dispatch_get_main_queue(), [=] {
        [nativeWindow setFrameOrigin:origin];
    });

    //NSRect bounds;
    //bounds.origin.x = clientBounds.X;
    //bounds.origin.y = clientBounds.Y;
    //bounds.size.width = clientBounds.Width;
    //bounds.size.height = clientBounds.Height;
    //[nativeWindow setFrame:bounds display:YES animate:YES];
}
//-----------------------------------------------------------------------
bool GameWindowCocoa::IsMouseCursorVisible() const
{
    return isMouseCursorVisible;
}
//-----------------------------------------------------------------------
void GameWindowCocoa::IsMouseCursorVisible(bool visibleIn)
{
    isMouseCursorVisible = visibleIn;

    if (isMouseCursorVisible) {
        [NSCursor unhide];
    }
    else {
        [NSCursor hide];
    }
}
//-----------------------------------------------------------------------
void GameWindowCocoa::SetMouseCursor(MouseCursor cursor)
{
    auto nativeCursor = ([cursor]()-> NSCursor* {
        switch (cursor) {
        case MouseCursor::Arrow:
            return [NSCursor arrowCursor];
        case MouseCursor::IBeam:
            return [NSCursor IBeamCursor];
        case MouseCursor::PointingHand:
            return [NSCursor pointingHandCursor];
        case MouseCursor::ResizeHorizontal:
            return [NSCursor resizeLeftRightCursor];
        case MouseCursor::ResizeVertical:
            return [NSCursor resizeUpDownCursor];
        }
    })();

    [nativeCursor set];
}
//-----------------------------------------------------------------------
#pragma mark - Low-Level API for GameHostCocoa
//-----------------------------------------------------------------------
bool GameWindowCocoa::IsMinimized() const
{
    return [nativeWindow isMiniaturized] == YES;
}
//-----------------------------------------------------------------------
void GameWindowCocoa::SetView(NSView* gameViewIn)
{
    gameView = gameViewIn;
}
//-----------------------------------------------------------------------
} // namespace Cocoa
} // namespace Detail
} // namespace Pomdog
