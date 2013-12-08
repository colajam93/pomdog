//
//  Copyright (C) 2013 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_FUNDAMENTALTYPES_2E64F0B3_6011_11E3_B322_A8206655A22B_HPP
#define POMDOG_FUNDAMENTALTYPES_2E64F0B3_6011_11E3_B322_A8206655A22B_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include "Platform.hpp"

// stdint
#if (defined(POMDOG_COMPILER_MSVC) && (_MSC_VER > 1500)) \
	|| defined(POMDOG_COMPILER_CLANG) \
	|| (defined(POMDOG_COMPILER_GNUC) && defined(__GXX_EXPERIMENTAL_CXX0X__))
#	// C++11 types support
#	include <cstdint>
#else
#	error "C++11/C++0x is not supported."
#	//include <stdint.h>
#endif

#include <cstddef> //size_t
#include <cfloat>  //float, double

namespace Pomdog {
namespace Details {

//using std::int8_t;
//using std::int16_t;
//using std::int32_t;
//using std::uint8_t;
//using std::uint16_t;
//using std::uint32_t;

#if defined(_SIZE_T_DEFINED) || defined(__SIZE_TYPE__)
	using std::size_t;
#else
	typedef unsigned int size_t;
#endif

}// namespace Details
}// namespace Pomdog

#endif // !defined(POMDOG_FUNDAMENTALTYPES_2E64F0B3_6011_11E3_B322_A8206655A22B_HPP)
