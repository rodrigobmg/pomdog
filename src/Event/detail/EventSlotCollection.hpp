//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_SRC_EVENT_EVENTSLOTCOLLECTION_4CF7B636_FCAF_47FF_8CDF_B8814C49357C_HPP
#define POMDOG_SRC_EVENT_EVENTSLOTCOLLECTION_4CF7B636_FCAF_47FF_8CDF_B8814C49357C_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include <Pomdog/Utility/Noncopyable.hpp>
#include <Pomdog/Event/EventConnection.hpp>
#include <functional>
#include <memory>
#include "EventSlot.hpp"

namespace Pomdog {

class Event;

namespace Details {
namespace EventInternal {

class EventSlotCollection: Noncopyable, public std::enable_shared_from_this<EventSlotCollection>
{
private:
	typedef Event event_type;
	typedef std::function<void(Event const&)> function_type;

public:
	EventSlotCollection();
	~EventSlotCollection();

	EventConnection Connect(function_type const& slot);
	EventConnection Connect(function_type && slot);
	
	void Disconnect(EventSlot<void(Event const&)> const* observer);

	void Invoke(event_type const& event);

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

}// namespace EventInternal
}// namespace Details
}// namespace Pomdog

#endif // !defined(POMDOG_SRC_EVENT_EVENTSLOTCOLLECTION_4CF7B636_FCAF_47FF_8CDF_B8814C49357C_HPP)
