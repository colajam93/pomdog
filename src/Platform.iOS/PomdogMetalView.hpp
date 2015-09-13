// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Basic/Export.hpp"
#include "Pomdog/Signals/detail/ForwardDeclarations.hpp"
#include <memory>
#include <functional>
#import <UIKit/UIKit.h>
#import <QuartzCore/CAMetalLayer.h>

namespace Pomdog {
namespace Detail {
namespace iOS {

} // namespace iOS
} // namespace Detail
} // namespace Pomdog

POMDOG_EXPORT @interface PomdogMetalView : UIView

//- (void)setEventQueue:(std::shared_ptr<Pomdog::EventQueue>)eventQueue;
//
//- (void)setRenderCallback:(std::function<void()>)callback;
//
//- (void)setResizingCallback:(std::function<void(bool)>)callback;

@end
