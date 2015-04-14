// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#include "ConstantBufferGL4.hpp"
#include "ErrorChecker.hpp"
#include "TypesafeHelperGL4.hpp"
#include "../Utility/ScopeGuard.hpp"
#include "Pomdog/Graphics/BufferUsage.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include <utility>

namespace Pomdog {
namespace Detail {
namespace RenderSystem {
namespace GL4 {
namespace {

static GLenum ToBufferUsage(BufferUsage bufferUsage) noexcept
{
    switch (bufferUsage) {
    case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
    //case BufferUsage::Dynamic: return GL_STREAM_DRAW;
    case BufferUsage::Immutable: return GL_STATIC_DRAW;
    }
#ifdef _MSC_VER
    return GL_STATIC_DRAW;
#endif
}

} // unnamed namespace
//-----------------------------------------------------------------------
template<>
struct TypesafeHelperGL4::OpenGLGetTraits<ConstantBufferObjectGL4> {
    constexpr static GLenum bufferObjectBinding = GL_UNIFORM_BUFFER_BINDING;
    constexpr static GLenum bufferObjectTarget = GL_UNIFORM_BUFFER;
};
//-----------------------------------------------------------------------
ConstantBufferGL4::ConstantBufferGL4(std::size_t sizeInBytes, BufferUsage bufferUsage)
    : ConstantBufferGL4(nullptr, sizeInBytes, bufferUsage)
{
    POMDOG_ASSERT(bufferUsage != BufferUsage::Immutable);
}
//-----------------------------------------------------------------------
ConstantBufferGL4::ConstantBufferGL4(void const* sourceData, std::size_t sizeInBytes,
    BufferUsage bufferUsage)
{
    POMDOG_ASSERT(bufferUsage == BufferUsage::Immutable
        ? sourceData != nullptr : true);

    // Generate constant buffer
    bufferObject = ([] {
        ConstantBufferObjectGL4 constantBuffer;
        glGenBuffers(1, constantBuffer.Data());
        return std::move(constantBuffer);
    })();

    auto const oldBuffer = TypesafeHelperGL4::Get<ConstantBufferObjectGL4>();
    ScopeGuard scope([&] { TypesafeHelperGL4::BindBuffer(oldBuffer); });

    POMDOG_ASSERT(bufferObject);
    TypesafeHelperGL4::BindBuffer(*bufferObject);
    POMDOG_CHECK_ERROR_GL4("glBindBuffer");

    POMDOG_ASSERT(sizeInBytes > 0);
    glBufferData(GL_UNIFORM_BUFFER, sizeInBytes, sourceData,
        ToBufferUsage(bufferUsage));
    POMDOG_CHECK_ERROR_GL4("glBufferData");
}
//-----------------------------------------------------------------------
ConstantBufferGL4::~ConstantBufferGL4()
{
    if (bufferObject) {
        glDeleteBuffers(1, bufferObject->Data());
        POMDOG_CHECK_ERROR_GL4("glDeleteBuffers");
    }
}
//-----------------------------------------------------------------------
void ConstantBufferGL4::GetData(std::size_t offsetInBytes,
    void* destination, std::size_t sizeInBytes) const
{
    POMDOG_ASSERT(destination != nullptr);
    POMDOG_ASSERT(sizeInBytes > 0);

    auto const oldBuffer = TypesafeHelperGL4::Get<ConstantBufferObjectGL4>();
    ScopeGuard scope([&] { TypesafeHelperGL4::BindBuffer(oldBuffer); });

    POMDOG_ASSERT(bufferObject);
    TypesafeHelperGL4::BindBuffer(*bufferObject);
    POMDOG_CHECK_ERROR_GL4("glBindBuffer");

#if defined(DEBUG) && !defined(NDEBUG)
    {
        GLint bufferSize = 0;
        glGetBufferParameteriv(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &bufferSize);
        POMDOG_ASSERT((offsetInBytes + sizeInBytes)
            <= static_cast<std::size_t>(bufferSize));
    }
#endif

    glGetBufferSubData(GL_UNIFORM_BUFFER,
        offsetInBytes, sizeInBytes, destination);
    POMDOG_CHECK_ERROR_GL4("glGetBufferSubData");
}
//-----------------------------------------------------------------------
void ConstantBufferGL4::SetData(std::size_t offsetInBytes,
    void const* source, std::size_t sizeInBytes)
{
    POMDOG_ASSERT(source != nullptr);

    auto const oldBuffer = TypesafeHelperGL4::Get<ConstantBufferObjectGL4>();
    ScopeGuard scope([&] { TypesafeHelperGL4::BindBuffer(oldBuffer); });

    POMDOG_ASSERT(bufferObject);
    TypesafeHelperGL4::BindBuffer(*bufferObject);
    POMDOG_CHECK_ERROR_GL4("glBindBuffer");

#if defined(DEBUG) && !defined(NDEBUG)
    {
        GLint bufferSize = 0;
        glGetBufferParameteriv(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &bufferSize);
        POMDOG_ASSERT(sizeInBytes <= static_cast<std::size_t>(bufferSize));
    }
#endif

    POMDOG_ASSERT(sizeInBytes > 0);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetInBytes, sizeInBytes, source);
    POMDOG_CHECK_ERROR_GL4("glBufferSubData");
}
//-----------------------------------------------------------------------
void ConstantBufferGL4::Apply(int slotIndex)
{
    POMDOG_ASSERT(bufferObject);
    POMDOG_ASSERT(slotIndex >= 0);

#if defined(DEBUG) && !defined(NDEBUG)
    {
        static auto const maxUniformBufferBindings = ([] {
            GLint value = 0;
            glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
            return value;
        })();

        POMDOG_ASSERT(slotIndex < maxUniformBufferBindings);
    }
#endif

    glBindBufferBase(GL_UNIFORM_BUFFER, slotIndex, bufferObject->value);
    POMDOG_CHECK_ERROR_GL4("glBindBufferBase");
}
//-----------------------------------------------------------------------
} // namespace GL4
} // namespace RenderSystem
} // namespace Detail
} // namespace Pomdog
