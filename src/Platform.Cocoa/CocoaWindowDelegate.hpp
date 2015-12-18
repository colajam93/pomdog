// Copyright (c) 2013-2015 mogemimi. Distributed under the MIT license.

#ifndef POMDOG_COCOAWINDOWDELEGATE_56FEB6D8_HPP
#define POMDOG_COCOAWINDOWDELEGATE_56FEB6D8_HPP

#include "Pomdog/Signals/EventQueue.hpp"
#import <Cocoa/Cocoa.h>
#include <memory>

@interface CocoaWindowDelegate : NSObject <NSWindowDelegate>

- (instancetype)initWithEventQueue:(std::shared_ptr<Pomdog::EventQueue>)eventQueue;

@end

#endif // POMDOG_COCOAWINDOWDELEGATE_56FEB6D8_HPP
