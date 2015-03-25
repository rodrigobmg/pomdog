// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#ifndef POMDOG_GAMEPAD_5B4AD0B0_EC50_42DA_AF23_1C9E2553DCA0_HPP
#define POMDOG_GAMEPAD_5B4AD0B0_EC50_42DA_AF23_1C9E2553DCA0_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Pomdog/Basic/Export.hpp"

namespace Pomdog {

class GamepadState;
class GamepadCapabilities;

class POMDOG_EXPORT Gamepad {
public:
    virtual ~Gamepad() = default;

    virtual GamepadCapabilities GetCapabilities() const = 0;

    virtual GamepadState GetState() const = 0;
};

}// namespace Pomdog

#endif // !defined(POMDOG_GAMEPAD_5B4AD0B0_EC50_42DA_AF23_1C9E2553DCA0_HPP)
