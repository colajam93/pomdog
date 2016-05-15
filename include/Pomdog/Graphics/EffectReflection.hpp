// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Graphics/detail/ForwardDeclarations.hpp"
#include "Pomdog/Graphics/EffectConstantDescription.hpp"
#include "Pomdog/Basic/Export.hpp"
#include <memory>
#include <vector>

namespace Pomdog {

class POMDOG_EXPORT EffectReflection {
public:
    EffectReflection() = delete;
    EffectReflection(const EffectReflection&) = delete;
    EffectReflection(EffectReflection &&) = default;

    EffectReflection(
        GraphicsDevice & graphicsDevice,
        const PipelineStateDescription& pipelineStateDescription,
        const std::shared_ptr<PipelineState>& pipelineState);

    EffectReflection(
        const std::shared_ptr<GraphicsDevice>& graphicsDevice,
        const PipelineStateDescription& pipelineStateDescription,
        const std::shared_ptr<PipelineState>& pipelineState);

    ~EffectReflection();

    std::vector<EffectConstantDescription> GetConstantBuffers() const;

    Detail::NativeEffectReflection* GetNativeEffectReflection();

private:
    std::unique_ptr<Detail::NativeEffectReflection> nativeEffectReflection;
};

} // namespace Pomdog
