// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "../Utility/Noncopyable.hpp"

namespace Pomdog {
namespace Detail {
namespace GL4 {

class OpenGLContext : Noncopyable {
public:
    virtual ~OpenGLContext() = default;

    virtual void MakeCurrent() = 0;

    virtual void ClearCurrent() = 0;

    virtual void SwapBuffers() = 0;
};

} // namespace GL4
} // namespace Detail
} // namespace Pomdog
