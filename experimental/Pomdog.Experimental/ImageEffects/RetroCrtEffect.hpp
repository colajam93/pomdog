// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "ImageEffectBase.hpp"
#include "Pomdog/Content/AssetManager.hpp"
#include "Pomdog/Graphics/detail/ForwardDeclarations.hpp"
#include <memory>

namespace Pomdog {

class RetroCrtEffect : public ImageEffectBase {
public:
    RetroCrtEffect(
        std::shared_ptr<GraphicsDevice> const& graphicsDevice,
        AssetManager & assets);

    void Apply(
        GraphicsCommandList & commandList,
        std::shared_ptr<RenderTarget2D> const& source,
        std::shared_ptr<ConstantBuffer> const& constantBuffer) override;

private:
    std::shared_ptr<SamplerState> samplerState;
    std::shared_ptr<PipelineState> pipelineState;
};

} // namespace Pomdog
