// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Content/AssetBuilders/Builder.hpp"
#include "Pomdog/Graphics/detail/ForwardDeclarations.hpp"
#include "Pomdog/Basic/Export.hpp"
#include <string>
#include <memory>
#include <vector>

namespace Pomdog {
namespace Detail {
class AssetLoaderContext;
} // namespace Detail

namespace AssetBuilders {

template <>
class POMDOG_EXPORT Builder<PipelineState> {
public:
    explicit Builder(Detail::AssetLoaderContext const& loaderContext);

    Builder(Builder &&);

    Builder & operator=(Builder &&);

    ~Builder();

    Builder & SetVertexShader(std::shared_ptr<Shader> const& vertexShader);

    Builder & SetVertexShader(std::shared_ptr<Shader> && vertexShader);

    Builder & SetPixelShader(std::shared_ptr<Shader> const& pixelShader);

    Builder & SetPixelShader(std::shared_ptr<Shader> && pixelShader);

    Builder & SetInputLayout(InputLayoutDescription const& inputLayout);

    Builder & SetInputLayout(InputLayoutDescription && inputLayout);

    Builder & SetBlendState(BlendDescription const& blendState);

    Builder & SetRasterizerState(RasterizerDescription const& rasterizerState);

    Builder & SetDepthStencilState(DepthStencilDescription const& depthStencilState);

    Builder & SetConstantBufferBindSlot(std::string const& name, int slotIndex);

    Builder & SetRenderTargetViewFormats(std::vector<SurfaceFormat> const& renderTargetViewFormats);

    Builder & SetRenderTargetViewFormats(std::vector<SurfaceFormat> && renderTargetViewFormats);

    Builder & SetDepthStencilViewFormat(DepthFormat depthStencilViewFormat);

    std::shared_ptr<PipelineState> Build();

    std::shared_ptr<EffectReflection> CreateEffectReflection(
        std::shared_ptr<PipelineState> const& pipelineState);

    PipelineStateDescription const& GetDescription() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace AssetBuilders
} // namespace Pomdogs
