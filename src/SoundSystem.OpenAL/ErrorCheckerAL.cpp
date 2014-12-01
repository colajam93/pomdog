﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#include "ErrorCheckerAL.hpp"
#include "PrerequisitesOpenAL.hpp"
#include <Pomdog/Utility/detail/Tagged.hpp>
#include <Pomdog/Logging/Log.hpp>
#include <Pomdog/Logging/LogStream.hpp>
#include <cstddef>
#include <sstream>

namespace Pomdog {
namespace Details {
namespace SoundSystem {
namespace OpenAL {
namespace {

struct ErrorCodeTag;
using ErrorCodeAL = Tagged<ALenum, ErrorCodeTag>;

static std::string ToString(ErrorCodeAL const& errorCode)
{
	switch (errorCode.value) {
	case AL_NO_ERROR: return "AL_NO_ERROR";
	case AL_INVALID_ENUM: return "AL_INVALID_ENUM";
	case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
	case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
	case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
	}
	std::stringstream ss;
	ss << "ErrorCode '" << reinterpret_cast<void const*>(errorCode.value) << "'";
	return ss.str();
}

}// unnamed namespace
//-----------------------------------------------------------------------
void ErrorCheckerAL::CheckError(char const* command, char const* filename, int line)
{
	ErrorCodeAL const errorCode{alGetError()};

	if (AL_NO_ERROR == errorCode.value) {
		return;
	}

	constexpr std::size_t maxLine = 30;

	static std::size_t lines = 0;
	if (lines < maxLine)
	{
		Log::Stream("Pomdog.SoundSystem", LogLevel::Warning)
			<< ">>> File " << filename << ", line " << line << ", in " << command << "\n"
			<< "OpenAL Error: " << ToString(errorCode);

		if (lines == (maxLine - 1U))
		{
			Log::Warning("Pomdog.SoundSystem", "OpenAL Error: More...");
		}
		++lines;
	}
}
//-----------------------------------------------------------------------
}// namespace OpenAL
}// namespace SoundSystem
}// namespace Details
}// namespace Pomdog