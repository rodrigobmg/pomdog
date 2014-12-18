﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_SRC_UTILITY_OPTIONAL_D43800AB_794D_41B0_8820_203F5CA7D6CB_HPP
#define POMDOG_SRC_UTILITY_OPTIONAL_D43800AB_794D_41B0_8820_203F5CA7D6CB_HPP

#if (_MSC_VER > 1000)
#pragma once
#endif

#include "Pomdog/Utility/Assert.hpp"
#include <type_traits>
#include <utility>

namespace Pomdog {

enum class OptionalType {
	NullOptional,
};

template <typename T>
class Optional final {
private:
	T data;
	bool valid;

public:
	constexpr Optional()
		: valid(false)
	{}
	
	constexpr Optional(OptionalType const&)
		: valid(false)
	{}

	Optional(Optional const&) = default;
	Optional(Optional &&) = default;
	
	constexpr Optional(T const& v)
		: data(v)
		, valid(true)
	{}
	
	constexpr Optional(T && v)
		: data(std::move(v))
		, valid(true)
	{}
	
	~Optional() = default;
	
	Optional & operator=(OptionalType const&)
	{
		valid = false;
		data.~T();
		return *this;
	}
	
	Optional & operator=(Optional const&) = default;
	Optional & operator=(Optional &&) = default;

	Optional & operator=(T const& v)
	{
		this->valid = true;
		this->data = v;
		return *this;
	}
	
	Optional & operator=(T && v)
	{
		this->valid = true;
		this->data = std::move(v);
		return *this;
	}
	
	constexpr T const* operator->() const noexcept
	{
		return POMDOG_CONSTEXPR_ASSERT(valid), &data;
	}
	
	T* operator->() noexcept
	{
		POMDOG_ASSERT(valid);
		return &data;
	}
	
	constexpr T const& operator*() const
	{
		return POMDOG_CONSTEXPR_ASSERT(valid), data;
	}
	
	T & operator*()
	{
		POMDOG_ASSERT(valid);
		return data;
	}
	
	constexpr explicit operator bool() const noexcept
	{
		return valid;
	}
	
	constexpr T const& value() const
	{
		return POMDOG_CONSTEXPR_ASSERT(valid), data;
	}
	
	T & value()
	{
		POMDOG_ASSERT(valid);
		return data;
	}
};

template <typename T>
inline constexpr Optional<T> MakeOptional(T v)
{
	return Optional<T>(v);
}

}// namespace Pomdog

#endif // !defined(POMDOG_SRC_UTILITY_OPTIONAL_D43800AB_794D_41B0_8820_203F5CA7D6CB_HPP)
