﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_SUBSYSTEMSCHEDULER_15398B34_AE1E_4AAA_BE2A_EC942196DC84_HPP
#define POMDOG_SUBSYSTEMSCHEDULER_15398B34_AE1E_4AAA_BE2A_EC942196DC84_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include "../Utility/Noncopyable.hpp"
#include "Pomdog/Event/Signal.hpp"

namespace Pomdog {
namespace Details {

class SubsystemScheduler: Noncopyable {
public:
	Signal<void()> OnUpdate;
};

}// namespace Details
}// namespace Pomdog

#endif // !defined(POMDOG_SUBSYSTEMSCHEDULER_15398B34_AE1E_4AAA_BE2A_EC942196DC84_HPP)
