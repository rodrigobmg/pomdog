﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_SRC_APPLICATION_SYSTEMEVENTDISPATCHER_7D649F10_0048_43D1_97DA_92FD44BE74E4_HPP
#define POMDOG_SRC_APPLICATION_SYSTEMEVENTDISPATCHER_7D649F10_0048_43D1_97DA_92FD44BE74E4_HPP

#if (_MSC_VER > 1000)
#pragma once
#endif

#include <Pomdog/Event/EventQueue.hpp>
#include <Pomdog/Input/Keys.hpp>

namespace Pomdog {
namespace Details {

using SystemEventDispatcher = EventQueue;

struct WindowWillCloseEvent {};
struct WindowShouldCloseEvent {};

struct ViewNeedsUpdateSurfaceEvent {};
struct ViewWillStartLiveResizeEvent {};
struct ViewDidEndLiveResizeEvent {};

//struct InputCharacterEvent {
//	std::uint16_t Character;
//};

struct InputKeyDownEvent {
	Keys Key;
};

struct InputKeyUpEvent {
	Keys Key;
};

}// namespace Details
}// namespace Pomdog

#endif // !defined(POMDOG_SRC_APPLICATION_SYSTEMEVENTDISPATCHER_7D649F10_0048_43D1_97DA_92FD44BE74E4_HPP)
