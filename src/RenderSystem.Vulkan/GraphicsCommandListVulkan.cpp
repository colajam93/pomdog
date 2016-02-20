// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "GraphicsCommandListVulkan.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/Exception.hpp"

namespace Pomdog {
namespace Detail {
namespace Vulkan {

void GraphicsCommandListVulkan::Close()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::Reset()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

std::size_t GraphicsCommandListVulkan::GetCount() const noexcept
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::Clear(ClearOptions options, Color const& color, float depth, std::uint8_t stencil)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::Draw(std::size_t vertexCount)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::DrawIndexed(std::size_t indexCount)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::DrawInstanced(
    std::size_t vertexCount,
    std::size_t instanceCount)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::DrawIndexedInstanced(
    std::size_t indexCount,
    std::size_t instanceCount)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetViewport(Viewport const& viewport)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetScissorRectangle(Rectangle const& rectangle)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetBlendFactor(Color const& blendFactor)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetVertexBuffers(std::vector<VertexBufferBinding> const& vertexBuffers)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetVertexBuffers(std::vector<VertexBufferBinding> && vertexBuffers)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetIndexBuffer(std::shared_ptr<IndexBuffer> const& indexBuffer)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetPipelineState(std::shared_ptr<NativePipelineState> const& pipelineState)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetConstantBuffer(int index, std::shared_ptr<NativeBuffer> const& constantBuffer)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetSampler(int index, std::shared_ptr<NativeSamplerState> && sampler)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetTexture(int index)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetTexture(int index, std::shared_ptr<Texture2D> const& texture)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetTexture(int index, std::shared_ptr<RenderTarget2D> const& texture)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetRenderTarget()
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetRenderTargets(std::vector<std::shared_ptr<RenderTarget2D>> const& renderTargets)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

void GraphicsCommandListVulkan::SetRenderTargets(std::vector<std::shared_ptr<RenderTarget2D>> && renderTargets)
{
    POMDOG_THROW_EXCEPTION(std::runtime_error, "Not implemented");
}

} // namespace Vulkan
} // namespace Detail
} // namespace Pomdog
