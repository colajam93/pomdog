// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "Pomdog/Graphics/ShaderCompilers/MetalCompiler.hpp"
#include "../../RenderSystem/NativeGraphicsDevice.hpp"
#include "../../RenderSystem/ShaderBytecode.hpp"
#include "../../RenderSystem/ShaderCompileOptions.hpp"
#include "Pomdog/Graphics/GraphicsDevice.hpp"
#include "Pomdog/Graphics/Shader.hpp"
#include "Pomdog/Graphics/ShaderLanguage.hpp"
#include "Pomdog/Utility/Assert.hpp"

using Pomdog::Detail::ShaderBytecode;
using Pomdog::Detail::ShaderCompileOptions;

namespace Pomdog {
namespace ShaderCompilers {
//-----------------------------------------------------------------------
std::unique_ptr<Shader> MetalCompiler::CreateShaderFromSource(
    GraphicsDevice & graphicsDevice,
    void const* shaderSource,
    std::size_t byteLength,
    std::string const& entryPoint,
    ShaderPipelineStage pipelineStage)
{
    POMDOG_ASSERT(shaderSource != nullptr);
    POMDOG_ASSERT(byteLength > 0);
    POMDOG_ASSERT(graphicsDevice.GetSupportedLanguage() == ShaderLanguage::Metal);

    auto nativeGraphicsDevice = graphicsDevice.GetNativeGraphicsDevice();

    ShaderBytecode shaderBytecode;
    shaderBytecode.Code = shaderSource;
    shaderBytecode.ByteLength = byteLength;

    ShaderCompileOptions compileOptions;
    compileOptions.EntryPoint = entryPoint;
    compileOptions.Profile.PipelineStage = pipelineStage;
    compileOptions.Precompiled = false;

    return nativeGraphicsDevice->CreateShader(shaderBytecode, compileOptions);
}
//-----------------------------------------------------------------------
} // namespace ShaderCompilers
} // namespace Pomdog
