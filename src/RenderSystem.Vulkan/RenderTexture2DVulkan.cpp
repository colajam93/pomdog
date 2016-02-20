// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "RenderTexture2DVulkan.hpp"
#include "Pomdog/Graphics/DepthFormat.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/Exception.hpp"

namespace Pomdog {
namespace Detail {
namespace Vulkan {
namespace {

VkFormat ToDepthStencilFormat(DepthFormat depthFormat) noexcept
{
    POMDOG_ASSERT(depthFormat != DepthFormat::None);
    switch (depthFormat) {
    case DepthFormat::Depth16: return VK_FORMAT_D16_UNORM;
    case DepthFormat::Depth24Stencil8: return VK_FORMAT_D24_UNORM_S8_UINT;
    case DepthFormat::Depth32: return VK_FORMAT_D32_SFLOAT;
    case DepthFormat::None:
        break;
    }
    return VK_FORMAT_D24_UNORM_S8_UINT;
}

} // unnamed namespace


} // namespace Vulkan
} // namespace Detail
} // namespace Pomdog
