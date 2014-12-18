﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_GAMEPADBUTTONS_7EF60FA0_1450_4C86_8013_7C72030AEBD8_HPP
#define POMDOG_GAMEPADBUTTONS_7EF60FA0_1450_4C86_8013_7C72030AEBD8_HPP

#if (_MSC_VER > 1000)
#pragma once
#endif

#include "ButtonState.hpp"
#include <cstdint>

namespace Pomdog {

///@~Japanese
/// @brief ゲームパッドの任意のボタンの状態を示します。
class GamepadButtons {
public:
	ButtonState A = ButtonState::Released;
	ButtonState B = ButtonState::Released;
	ButtonState X = ButtonState::Released;
	ButtonState Y = ButtonState::Released;
	ButtonState LeftShoulder = ButtonState::Released;
	ButtonState RightShoulder = ButtonState::Released;
	ButtonState Start = ButtonState::Released;
	ButtonState LeftStick = ButtonState::Released;
	ButtonState RightStick = ButtonState::Released;
};

}// namespace Pomdog

#endif // !defined(POMDOG_GAMEPADBUTTONS_7EF60FA0_1450_4C86_8013_7C72030AEBD8_HPP)
