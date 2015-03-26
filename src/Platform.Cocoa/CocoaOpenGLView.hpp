// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#ifndef POMDOG_COCOAOPENGLVIEW_A756AF9C_HPP
#define POMDOG_COCOAOPENGLVIEW_A756AF9C_HPP

#import <Cocoa/Cocoa.h>
#include <functional>

@class NSOpenGLContext;
@class CocoaGameViewDelegate;

@interface CocoaOpenGLView : NSView

@property (nonatomic, retain) NSOpenGLContext* openGLContext;
@property (nonatomic, retain) CocoaGameViewDelegate* delegate;

- (id)initWithFrame:(NSRect)frameRect;

- (void)clearGLContext;
- (void)reshape;

- (void)setRenderCallback:(std::function<void()>)callback;

@end

#endif // POMDOG_COCOAOPENGLVIEW_A756AF9C_HPP
