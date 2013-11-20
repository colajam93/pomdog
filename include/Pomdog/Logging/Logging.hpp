//
//  Copyright (C) 2013 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_LOGGING_HPP
#define POMDOG_LOGGING_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include "LogEntry.hpp"
#include "LogStream.hpp"
#include "../Event/EventConnection.hpp"
#include <functional>

namespace Pomdog {

/// @addtogroup Framework
/// @{
/// @addtogroup Logging
/// @{

struct Log
{
	///@~Japanese
	/// @brief 既定のチャンネルにログを出力します。
	static void LogMessage(std::string const& message,
		LoggingLevel verbosity = LoggingLevel::Verbose);
	
	///@~Japanese
	/// @brief 指定されたチャンネルにログを出力します。
	static void LogMessage(std::string const& message, std::string const& channelName,
		LoggingLevel verbosity = LoggingLevel::Verbose);
	
	///@~Japanese
	/// @brief 既定のチャンネルのストリームを取得します。
	static LogStream Stream(LoggingLevel verbosity = LoggingLevel::Verbose);
	
	///@~Japanese
	/// @brief 指定されたチャンネルのストリームを取得します。
	static LogStream Stream(std::string const& channelName, LoggingLevel verbosity = LoggingLevel::Verbose);
	
	///@~Japanese
	/// @brief 新しくチャンネルを追加します。
	/// @remarks すでに同名のチャンネルが存在する場合、この呼び出しは無効となります。
	static void AddChannel(std::string const& channelName, LoggingLevel verbosity = LoggingLevel::Verbose);
	
	///@~Japanese
	/// @brief 指定されたチャンネルを削除します。
	static void RemoveChannel(std::string const& channelName);
	
	///@~Japanese
	/// @brief 指定されたチャンネルが存在しているかどうかを取得します。
	static bool ExistChannel(std::string const& channelName);
	
	///@~Japanese
	/// @brief 既定のチャンネルの冗長レベルを取得します。
	static LoggingLevel GetVerbosity();
	
	///@~Japanese
	/// @brief 既定のチャンネルの冗長レベルを設定します。
	static void SetVerbosity(LoggingLevel verbosity);
	
	///@~Japanese
	/// @brief 指定されたチャンネルの冗長レベルを取得します。
	static LoggingLevel GetVerbosity(std::string const& channelName);
	
	///@~Japanese
	/// @brief 指定されたチャンネルの冗長レベルを設定します。
	static void SetVerbosity(std::string const& channelName, LoggingLevel verbosity);
	
	///@~Japanese
	/// @brief 既定のチャンネルに接続します。
	static EventConnection Connect(std::function<void(LogEntry const&)> callback);
	
	///@~Japanese
	/// @brief 指定されたチャンネルに接続します。
	/// @remarks 指定されたチャンネルが存在しない場合、新しくチャンネルを作成します。
	static EventConnection Connect(std::function<void(LogEntry const&)> callback, std::string const& channelName);
};

/// @}
/// @}

}// namespace Pomdog

#endif // !defined(POMDOG_LOGGING_HPP)