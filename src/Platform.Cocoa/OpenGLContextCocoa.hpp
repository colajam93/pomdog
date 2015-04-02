// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#ifndef POMDOG_OPENGLCONTEXTCOCOA_306ECA78_HPP
#define POMDOG_OPENGLCONTEXTCOCOA_306ECA78_HPP

#include "../RenderSystem.GL4/OpenGLContext.hpp"
#import <Cocoa/Cocoa.h>

@class NSOpenGLContext, NSOpenGLPixelFormat;

namespace Pomdog {

class PresentationParameters;

namespace Detail {
namespace Cocoa {

struct CocoaOpenGLHelper {
    static NSOpenGLPixelFormat* CreatePixelFormat(
        PresentationParameters const& presentationParameters);
};

class OpenGLContextCocoa final: public RenderSystem::GL4::OpenGLContext {
public:
    OpenGLContextCocoa() = delete;

    explicit OpenGLContextCocoa(NSOpenGLPixelFormat* pixelFormat);

    ~OpenGLContextCocoa();

    void MakeCurrent() override;

    void ClearCurrent() override;

    void SwapBuffers() override;

    void Lock();

    void Unlock();

    void SetView(NSView* view);

    void SetView();

    NSOpenGLContext* NativeOpenGLContext();

private:
    __strong NSOpenGLContext* openGLContext;
};

} // namespace Cocoa
} // namespace Detail
} // namespace Pomdog

#endif // POMDOG_OPENGLCONTEXTCOCOA_306ECA78_HPP
