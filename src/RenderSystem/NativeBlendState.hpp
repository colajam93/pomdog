// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#ifndef POMDOG_NATIVEBLENDSTATE_95C63681_HPP
#define POMDOG_NATIVEBLENDSTATE_95C63681_HPP

namespace Pomdog {
namespace Detail {
namespace RenderSystem {

class NativeGraphicsContext;

class NativeBlendState {
public:
    NativeBlendState() = default;
    NativeBlendState(NativeBlendState const&) = delete;
    NativeBlendState & operator=(NativeBlendState const&) = delete;

    virtual ~NativeBlendState() = default;

    virtual void Apply(NativeGraphicsContext & graphicsContext) = 0;
};

}// namespace RenderSystem
}// namespace Detail
}// namespace Pomdog

#endif // POMDOG_NATIVEBLENDSTATE_95C63681_HPP
