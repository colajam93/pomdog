// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "PipelineStateMetal.hpp"
#include "MetalFormatHelper.hpp"
#include "Pomdog/Graphics/DepthFormat.hpp"
#include "Pomdog/Graphics/PipelineStateDescription.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/Exception.hpp"
#include "Pomdog/Utility/StringHelper.hpp"
#include "ShaderMetal.hpp"
#import <Metal/MTLVertexDescriptor.h>

namespace Pomdog {
namespace Detail {
namespace Metal {
namespace {

MTLVertexStepFunction ToVertexStepFunction(InputClassification classification) noexcept
{
    switch (classification) {
    case InputClassification::InputPerVertex:
        return MTLVertexStepFunctionPerVertex;
    case InputClassification::InputPerInstance:
        return MTLVertexStepFunctionPerInstance;
    }
}
//-----------------------------------------------------------------------
MTLVertexFormat ToVertexFormat(InputElementFormat format) noexcept
{
    switch (format) {
    case InputElementFormat::Float: return MTLVertexFormatFloat;
    case InputElementFormat::Float2: return MTLVertexFormatFloat2;
    case InputElementFormat::Float3: return MTLVertexFormatFloat3;
    case InputElementFormat::Float4: return MTLVertexFormatFloat4;
    case InputElementFormat::Int4: return MTLVertexFormatInt4;
    case InputElementFormat::Byte4: return MTLVertexFormatUChar4;
    case InputElementFormat::HalfFloat2: return MTLVertexFormatHalf2;
    case InputElementFormat::HalfFloat4: return MTLVertexFormatHalf4;
    }
}
//-----------------------------------------------------------------------
MTLVertexDescriptor* ToVertexDescriptor(const InputLayoutDescription& inputLayout)
{
    MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor new];

    int bufferIndex = 0;
    for (auto & element : inputLayout.InputElements) {
        auto bufferLayout = vertexDescriptor.layouts[bufferIndex];
        bufferLayout.stride = element.ByteOffset;
        bufferLayout.stepRate = element.InstanceStepRate;
        bufferLayout.stepFunction = ToVertexStepFunction(element.InputSlotClass);

        auto attribute = vertexDescriptor.attributes[bufferIndex];
        attribute.format = ToVertexFormat(element.Format);
        attribute.offset = element.ByteOffset;
        attribute.bufferIndex = bufferIndex;
        ++bufferIndex;
    }

    return vertexDescriptor;
}
//-----------------------------------------------------------------------
MTLBlendOperation ToBlendOperation(BlendFunction blendOperation) noexcept
{
    switch (blendOperation) {
    case BlendFunction::Add: return MTLBlendOperationAdd;
    case BlendFunction::Subtract: return MTLBlendOperationSubtract;
    case BlendFunction::ReverseSubtract: return MTLBlendOperationReverseSubtract;
    case BlendFunction::Min: return MTLBlendOperationMin;
    case BlendFunction::Max: return MTLBlendOperationMax;
    }
}
//-----------------------------------------------------------------------
MTLBlendFactor ToBlendFactor(Blend blend)
{
    switch (blend) {
    case Blend::Zero: return MTLBlendFactorZero;
    case Blend::One: return MTLBlendFactorOne;
    case Blend::SourceColor: return MTLBlendFactorSourceColor;
    case Blend::InverseSourceColor: return MTLBlendFactorOneMinusSourceColor;
    case Blend::SourceAlpha: return MTLBlendFactorSourceAlpha;
    case Blend::InverseSourceAlpha: return MTLBlendFactorOneMinusSourceAlpha;
    case Blend::DestinationAlpha: return MTLBlendFactorDestinationAlpha;
    case Blend::InverseDestinationAlpha: return MTLBlendFactorOneMinusDestinationAlpha;
    case Blend::DestinationColor: return MTLBlendFactorDestinationColor;
    case Blend::InverseDestinationColor: return MTLBlendFactorOneMinusDestinationColor;
    case Blend::SourceAlphaSaturation: return MTLBlendFactorSourceAlphaSaturated;
    case Blend::Source1Color: return MTLBlendFactorBlendColor;
    case Blend::InverseSource1Color: return MTLBlendFactorOneMinusBlendColor;
    case Blend::Source1Alpha: return MTLBlendFactorBlendAlpha;
    case Blend::InverseSource1Alpha: return MTLBlendFactorOneMinusBlendAlpha;
    case Blend::BlendFactor:
        break;
    case Blend::InvereseBlendFactor:
        break;
    }

    POMDOG_THROW_EXCEPTION(std::runtime_error, "Invalid blend factor");
    return MTLBlendFactorOne;
}
//-----------------------------------------------------------------------
MTLStencilOperation ToStencilOperation(StencilOperation operation) noexcept
{
    switch (operation) {
    case StencilOperation::Keep: return MTLStencilOperationKeep;
    case StencilOperation::Zero: return MTLStencilOperationZero;
    case StencilOperation::Replace: return MTLStencilOperationReplace;
    case StencilOperation::IncrementSaturation: return MTLStencilOperationIncrementClamp;
    case StencilOperation::DecrementSaturation: return MTLStencilOperationDecrementClamp;
    case StencilOperation::Invert: return MTLStencilOperationInvert;
    case StencilOperation::Increment: return MTLStencilOperationIncrementWrap;
    case StencilOperation::Decrement: return MTLStencilOperationDecrementWrap;
    }
    return MTLStencilOperationKeep;
}
//-----------------------------------------------------------------------
MTLCompareFunction ToComparisonFunction(ComparisonFunction compareFunction) noexcept
{
    switch (compareFunction) {
    case ComparisonFunction::Never: return MTLCompareFunctionNever;
    case ComparisonFunction::Less:  return MTLCompareFunctionLess;
    case ComparisonFunction::Equal: return MTLCompareFunctionEqual;
    case ComparisonFunction::LessEqual: return MTLCompareFunctionLessEqual;
    case ComparisonFunction::Greater:   return MTLCompareFunctionGreater;
    case ComparisonFunction::NotEqual:  return MTLCompareFunctionNotEqual;
    case ComparisonFunction::GreaterEqual: return MTLCompareFunctionGreaterEqual;
    case ComparisonFunction::Always: return MTLCompareFunctionAlways;
    }
    return MTLCompareFunctionLessEqual;
}
//-----------------------------------------------------------------------
void ToDepthStencilOperation(MTLStencilDescriptor* desc, const DepthStencilOperation& description)
{
    desc.stencilCompareFunction = ToComparisonFunction(description.StencilFunction);
    desc.depthStencilPassOperation = ToStencilOperation(description.StencilPass);
    desc.stencilFailureOperation = ToStencilOperation(description.StencilFail);
    desc.depthFailureOperation = ToStencilOperation(description.StencilDepthBufferFail);

    // TODO: Not implemented
    //desc.readMask = 0xff;
    //desc.writeMask = 0xff;
}
//-----------------------------------------------------------------------
MTLCullMode ToCullMode(CullMode cullMode) noexcept
{
    switch (cullMode) {
    case CullMode::ClockwiseFace: return MTLCullModeFront;
    case CullMode::CounterClockwiseFace: return MTLCullModeBack;
    case CullMode::None: return MTLCullModeNone;
    }
    return MTLCullModeBack;
}
//-----------------------------------------------------------------------
MTLTriangleFillMode ToFillMode(FillMode fillMode) noexcept
{
    switch (fillMode) {
    case FillMode::WireFrame: return MTLTriangleFillModeLines;
    case FillMode::Solid: return MTLTriangleFillModeFill;
    }
    return MTLTriangleFillModeFill;
}
//-----------------------------------------------------------------------
MTLPixelFormat ToDepthPixelFormat(DepthFormat depthFormat) noexcept
{
    // Not supported:
    POMDOG_ASSERT(depthFormat != DepthFormat::Depth16);

    switch (depthFormat) {
    case DepthFormat::Depth16: return MTLPixelFormatDepth32Float;
    case DepthFormat::Depth32: return MTLPixelFormatDepth32Float;
#if defined(MAC_OS_X_VERSION_10_11) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_11)
    case DepthFormat::Depth24Stencil8: return MTLPixelFormatDepth24Unorm_Stencil8;
#else
    case DepthFormat::Depth24Stencil8: return MTLPixelFormatDepth32Float_Stencil8;
#endif
    case DepthFormat::None: return MTLPixelFormatInvalid;
    }
    return MTLPixelFormatDepth32Float;
}
//-----------------------------------------------------------------------
MTLPixelFormat ToStencilPixelFormat(DepthFormat depthFormat) noexcept
{
    switch (depthFormat) {
    case DepthFormat::Depth24Stencil8: return MTLPixelFormatStencil8;
    case DepthFormat::Depth32: return MTLPixelFormatInvalid;
    case DepthFormat::Depth16: return MTLPixelFormatInvalid;
    case DepthFormat::None: return MTLPixelFormatInvalid;
    }
    return MTLPixelFormatInvalid;
}

} // unnamed namespace
//-----------------------------------------------------------------------
PipelineStateMetal::PipelineStateMetal(
    id<MTLDevice> device,
    PipelineStateDescription const& description)
    : pipelineState(nil)
    , depthStencilState(nil)
    , reflection(nil)
{
    POMDOG_ASSERT(device != nil);

    rasterizerState.depthBias = description.RasterizerState.DepthBias;
    rasterizerState.slopeScaledDepthBias = description.RasterizerState.SlopeScaledDepthBias;
    rasterizerState.cullMode = ToCullMode(description.RasterizerState.CullMode);
    rasterizerState.fillMode = ToFillMode(description.RasterizerState.FillMode);

    auto vertexShaderMetal = std::dynamic_pointer_cast<ShaderMetal>(description.VertexShader);
    if (!vertexShaderMetal) {
        POMDOG_THROW_EXCEPTION(std::runtime_error, "Invalid vertex shader.");
    }

    auto vertexShader = vertexShaderMetal->GetShader();
    if (!vertexShader) {
        POMDOG_THROW_EXCEPTION(std::runtime_error, "The vertex shader is null");
    }

    auto pixelShaderMetal = std::dynamic_pointer_cast<ShaderMetal>(description.PixelShader);
    if (!pixelShaderMetal) {
        POMDOG_THROW_EXCEPTION(std::runtime_error, "Invalid pixel shader.");
    }

    auto pixelShader = pixelShaderMetal->GetShader();
    if (!pixelShader) {
        POMDOG_THROW_EXCEPTION(std::runtime_error, "The pixel shader is null");
    }

    ///@todo MSAA is not implemented yet
    constexpr int multiSampleCount = 1;

    MTLRenderPipelineDescriptor* descriptor = [[MTLRenderPipelineDescriptor alloc] init];
    descriptor.label = @"Pomdog.RenderPipeline";
    descriptor.vertexFunction = vertexShader;
    descriptor.fragmentFunction = pixelShader;
    descriptor.vertexDescriptor = ToVertexDescriptor(description.InputLayout);
    descriptor.sampleCount = multiSampleCount;
    descriptor.depthAttachmentPixelFormat = ToDepthPixelFormat(description.DepthStencilViewFormat);
    descriptor.stencilAttachmentPixelFormat = ToStencilPixelFormat(description.DepthStencilViewFormat);

    std::size_t index = 0;
    for (auto & renderTarget : description.BlendState.RenderTargets) {
        auto colorAttachment = descriptor.colorAttachments[index];
        colorAttachment.rgbBlendOperation = ToBlendOperation(renderTarget.ColorBlendFunction);
        colorAttachment.alphaBlendOperation = ToBlendOperation(renderTarget.AlphaBlendFunction);
        colorAttachment.sourceRGBBlendFactor = ToBlendFactor(renderTarget.ColorSourceBlend);
        colorAttachment.sourceAlphaBlendFactor = ToBlendFactor(renderTarget.AlphaSourceBlend);
        colorAttachment.destinationRGBBlendFactor = ToBlendFactor(renderTarget.ColorDestinationBlend);
        colorAttachment.destinationAlphaBlendFactor = ToBlendFactor(renderTarget.AlphaDestinationBlend);
        colorAttachment.blendingEnabled = renderTarget.BlendEnable;

        POMDOG_ASSERT(index < description.RenderTargetViewFormats.size());
        if (index < description.RenderTargetViewFormats.size()) {
            auto pixelFormat = MetalFormatHelper::ToMTLPixelFormat(description.RenderTargetViewFormats[index]);
            POMDOG_ASSERT(pixelFormat);
            colorAttachment.pixelFormat = *pixelFormat;
        }
        else {
            colorAttachment.pixelFormat = MTLPixelFormatInvalid;
        }

        // TODO: Not implemented
        colorAttachment.writeMask = MTLColorWriteMaskAll;

        ++index;
    }

    NSError* error = nil;

    MTLRenderPipelineReflection* autoReleasingReflection = nil;

    this->pipelineState = [device
        newRenderPipelineStateWithDescriptor:descriptor
        options:MTLPipelineOptionArgumentInfo
        reflection:&autoReleasingReflection
        error:&error];

    reflection = autoReleasingReflection;

    if (!this->pipelineState) {
        POMDOG_THROW_EXCEPTION(std::runtime_error,
            StringHelper::Format(
                "Failed to create pipeline state, error %s",
                [[error domain] UTF8String]));
    }

    MTLDepthStencilDescriptor* depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
    depthStateDesc.label = @"Pomdog.DepthStencilState";
    depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
    depthStateDesc.depthWriteEnabled = YES;

    ToDepthStencilOperation(
        depthStateDesc.frontFaceStencil,
        description.DepthStencilState.ClockwiseFace);

    ToDepthStencilOperation(
        depthStateDesc.backFaceStencil,
        description.DepthStencilState.CounterClockwiseFace);

    depthStencilState = [device newDepthStencilStateWithDescriptor:depthStateDesc];
    if (!this->depthStencilState) {
        POMDOG_THROW_EXCEPTION(std::runtime_error,
            "Failed to create depth stencil state for Metal");
    }
}
//-----------------------------------------------------------------------
void PipelineStateMetal::Apply(id<MTLRenderCommandEncoder> commandEncoder)
{
    POMDOG_ASSERT(commandEncoder != nil);

    [commandEncoder setRenderPipelineState:pipelineState];
    [commandEncoder setDepthStencilState:depthStencilState];
    [commandEncoder setCullMode:rasterizerState.cullMode];
    [commandEncoder setTriangleFillMode:rasterizerState.fillMode];
    [commandEncoder setDepthBias:rasterizerState.depthBias
        slopeScale:rasterizerState.slopeScaledDepthBias
        clamp:0.0f];
}
//-----------------------------------------------------------------------
} // namespace Metal
} // namespace Detail
} // namespace Pomdog
