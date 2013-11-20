//
//  Copyright (C) 2013 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_LOGGINGLEVEL_HPP
#define POMDOG_LOGGINGLEVEL_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include "../Config/FundamentalTypes.hpp"

namespace Pomdog {

/// @addtogroup Framework
/// @{
/// @addtogroup Logging
/// @{

///@~English
/// @brief verbosity level of logging.
///@~Japanese
/// @brief ログの出力における冗長レベルです。
enum class LoggingLevel: std::uint16_t
{
	///@~Japanese
	/// @brief 致命的なエラーです。回復することが不可能な状態を示します。
	Critical = 0,

	///@~Japanese
	/// @brief 特定の重要なイベントログのみを示します。
	Brief = 1,

	///@~Japanese
	/// @brief エンジンの内部処理に関するログを除くすべてのメッセージを示します。
	Verbose = 2,

	///@~Japanese
	/// @brief エンジンの内部処理に関わるログなどを含めたすべてのログを示します。
	Internal = 3
};

/// @}
/// @}

}// namespace Pomdog

#endif // !defined(POMDOG_LOGGINGLEVEL_HPP)