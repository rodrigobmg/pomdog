//
//  Copyright (C) 2013 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_SRC_APPLICATION_SYSTEMEVENTDISPATCHER_31E13282_5D14_11E3_BA5F_A8206655A22B_HPP
#define POMDOG_SRC_APPLICATION_SYSTEMEVENTDISPATCHER_31E13282_5D14_11E3_BA5F_A8206655A22B_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include <Pomdog/Event/EventQueue.hpp>

namespace Pomdog {
namespace Details {

using SystemEventDispatcher = EventQueue;

///@code
///  //
///  auto WindowWillCloseEvent = EventCodeHelper::CreateCode("WindowWillClose");
///
///  //
///  auto WindowShouldCloseEvent = EventCodeHelper::CreateCode("WindowShouldClose");
///@endcode

}// namespace Details
}// namespace Pomdog

#endif // !defined(POMDOG_SRC_APPLICATION_SYSTEMEVENTDISPATCHER_31E13282_5D14_11E3_BA5F_A8206655A22B_HPP)