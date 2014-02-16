//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_DETAIL_TAGGED_1B3449F5_9833_496C_A481_71FB7536CBF5_HPP
#define POMDOG_DETAIL_TAGGED_1B3449F5_9833_496C_A481_71FB7536CBF5_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include <type_traits>
#include <functional>
#include <utility>
#include "../../Config/Export.hpp"
#include "../../Config/FundamentalTypes.hpp"

namespace Pomdog {
namespace Details {

template <typename T, class Tag>
class POMDOG_EXPORT Tagged final
{
public:
	static_assert(std::is_pod<T>::value, "You can only use plain-old-data types");

	typedef T value_type;
	typedef Tag tag_type;

	T value;

public:
	Tagged() = default;
	Tagged(Tagged const&) = default;
	Tagged(Tagged &&) = default;
	
	~Tagged() = default;
	
	Tagged& operator=(Tagged const&) = default;
	Tagged& operator=(Tagged &&) = default;

	explicit Tagged(value_type const& v)
		: value(v) {}
		
	explicit Tagged(value_type && v)
		: value(std::move(v)) {}

	constexpr T const* data() const noexcept
	{
		return &value;
	}

	T* data() noexcept
	{
		return &value;
	}

	constexpr bool operator==(Tagged const& v) const
	{
		return value == v.value;
	}

	constexpr bool operator!=(Tagged const& v) const
	{
		return value != v.value;
	}
};

}// namespace Details
}// namespace Pomdog

namespace std {

template <typename T, class U>
struct hash<Pomdog::Details::Tagged<T, U>>
{
	std::size_t operator()(Pomdog::Details::Tagged<T, U> const& key)
	{
		return std::hash<T>()(key.value);
	}
};

}// namespace std

#endif // !defined(POMDOG_DETAIL_TAGGED_1B3449F5_9833_496C_A481_71FB7536CBF5_HPP)
