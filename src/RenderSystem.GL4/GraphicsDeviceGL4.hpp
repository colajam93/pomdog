﻿//
//  Copyright (C) 2013 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_SRC_GL4_GRAPHICSDEVICEGL4_B530C330_5F5F_11E3_BCD5_A8206655A22B_HPP
#define POMDOG_SRC_GL4_GRAPHICSDEVICEGL4_B530C330_5F5F_11E3_BCD5_A8206655A22B_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include "../RenderSystem/NativeGraphicsDevice.hpp"
#include <memory>

namespace Pomdog {
namespace Details {
namespace RenderSystem {
namespace GL4 {

class GraphicsDeviceGL4 final: public NativeGraphicsDevice
{
public:
	GraphicsDeviceGL4() = default;

	~GraphicsDeviceGL4() = default;
	
	///@copydoc NativeGraphicsDevice
	std::unique_ptr<NativeBlendState>
	CreateBlendState(BlendDescription const& description) override;

	///@copydoc NativeGraphicsDevice
	std::unique_ptr<NativeDepthStencilState>
	CreateDepthStencilState(DepthStencilDescription const& description) override;

	///@copydoc NativeGraphicsDevice
	std::unique_ptr<NativeSamplerState>
	CreateSamplerState(SamplerDescription const& description) override;

	///@copydoc NativeGraphicsDevice
	std::unique_ptr<NativeRasterizerState>
	CreateRasterizerState(RasterizerDescription const& description) override;
};

}// namespace GL4
}// namespace RenderSystem
}// namespace Details
}// namespace Pomdog

#endif // !defined(POMDOG_SRC_GL4_GRAPHICSDEVICEGL4_B530C330_5F5F_11E3_BCD5_A8206655A22B_HPP)