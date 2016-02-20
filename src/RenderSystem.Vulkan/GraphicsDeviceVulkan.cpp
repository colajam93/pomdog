// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "GraphicsDeviceVulkan.hpp"
#include "Pomdog/Graphics/ShaderLanguage.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/Exception.hpp"
#include <vulkan/vulkan.h>

namespace Pomdog {
namespace Detail {
namespace Vulkan {

class GraphicsDeviceVulkan::Impl final {
public:
    std::vector<VkPhysicalDevice> devices;

public:
    Impl();
};
//-----------------------------------------------------------------------
GraphicsDeviceVulkan::Impl::Impl()
{
}
//-----------------------------------------------------------------------
GraphicsDeviceVulkan::GraphicsDeviceVulkan()
    : impl(std::make_unique<Impl>())
{
}
//-----------------------------------------------------------------------
GraphicsDeviceVulkan::~GraphicsDeviceVulkan() = default;
//-----------------------------------------------------------------------
ShaderLanguage GraphicsDeviceVulkan::GetSupportedLanguage() const
{
    return ShaderLanguage::GLSL;
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeGraphicsCommandList>
GraphicsDeviceVulkan::CreateGraphicsCommandList()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::unique_ptr<Shader>
GraphicsDeviceVulkan::CreateShader(ShaderBytecode const& shaderBytecode,
    ShaderCompileOptions const& compileOptions)
{
    POMDOG_ASSERT(impl);
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeBuffer>
GraphicsDeviceVulkan::CreateBuffer(std::size_t sizeInBytes,
    BufferUsage bufferUsage, BufferBindMode)
{
    POMDOG_ASSERT(impl);
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeBuffer>
GraphicsDeviceVulkan::CreateBuffer(void const* sourceData,  std::size_t sizeInBytes,
    BufferUsage bufferUsage, BufferBindMode)
{
    POMDOG_ASSERT(impl);
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeSamplerState>
GraphicsDeviceVulkan::CreateSamplerState(SamplerDescription const& description)
{
    POMDOG_ASSERT(impl);
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::unique_ptr<NativePipelineState>
GraphicsDeviceVulkan::CreatePipelineState(PipelineStateDescription const& description)
{
    POMDOG_ASSERT(impl);
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeEffectReflection>
GraphicsDeviceVulkan::CreateEffectReflection(PipelineStateDescription const& description,
    NativePipelineState & pipelineState)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeTexture2D>
GraphicsDeviceVulkan::CreateTexture2D(std::int32_t width, std::int32_t height,
    std::int32_t mipmapLevels, SurfaceFormat format)
{
    POMDOG_ASSERT(impl);
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeRenderTarget2D>
GraphicsDeviceVulkan::CreateRenderTarget2D(std::int32_t width, std::int32_t height,
    std::int32_t mipmapLevels,
    SurfaceFormat format,
    DepthFormat depthStencilFormat,
    std::int32_t multiSampleCount)
{
    POMDOG_ASSERT(impl);
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}
//-----------------------------------------------------------------------
} // namespace Vulkan
} // namespace Detail
} // namespace Pomdog
