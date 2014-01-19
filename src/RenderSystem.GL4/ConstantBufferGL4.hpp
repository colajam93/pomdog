//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_SRC_GL4_CONSTANTBUFFERGL4_98395F08_1611_4948_9C35_81FE756DE978_HPP
#define POMDOG_SRC_GL4_CONSTANTBUFFERGL4_98395F08_1611_4948_9C35_81FE756DE978_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include "OpenGLPrerequisites.hpp"
#include <Pomdog/Config/FundamentalTypes.hpp>
#include <Pomdog/Utility/detail/Tagged.hpp>
#include "../RenderSystem/NativeConstantBuffer.hpp"
#include "../Utility/Optional.hpp"

namespace Pomdog {
namespace Details {
namespace RenderSystem {
namespace GL4 {

namespace Tags {

struct ConstantBufferObjectTag {};

}// namespace Tags

using ConstantBufferObjectGL4 = Tagged<GLuint, Tags::ConstantBufferObjectTag>;

class ConstantBufferGL4 final: public NativeConstantBuffer
{
public:
	ConstantBufferGL4() = delete;
	
	explicit ConstantBufferGL4(std::uint32_t byteWidth);
	
	~ConstantBufferGL4();
	
	///@copydoc NativeConstantBuffer
	void GetData(std::uint32_t byteWidth, std::uint8_t* result) const override;

	///@copydoc NativeConstantBuffer
	void SetData(std::uint8_t const* data, std::uint32_t byteWidth) override;
	
	///@~Japanese
	/// @brief シェーダプログラムに定数バッファを適用します。
	void Apply(std::uint32_t slotIndex);
	
private:
	Optional<ConstantBufferObjectGL4> bufferObject;
};

}// namespace GL4
}// namespace RenderSystem
}// namespace Details
}// namespace Pomdog

#endif // !defined(POMDOG_SRC_GL4_CONSTANTBUFFERGL4_98395F08_1611_4948_9C35_81FE756DE978_HPP)