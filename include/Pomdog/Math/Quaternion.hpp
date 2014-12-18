﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_QUATERNION_601F9EDC_AA32_4901_8824_EDD91E38D6E2_HPP
#define POMDOG_QUATERNION_601F9EDC_AA32_4901_8824_EDD91E38D6E2_HPP

#if (_MSC_VER > 1000)
#pragma once
#endif

#include "detail/FloatingPointQuaternion.hpp"

namespace Pomdog {

///@~Japanese
/// @brief 四元数（クォータニオン）を表す 4 次元ベクトルを定義します。
using Quaternion = Details::FloatingPointQuaternion<float>;

}// namespace Pomdog

#endif // !defined(POMDOG_QUATERNION_601F9EDC_AA32_4901_8824_EDD91E38D6E2_HPP)
