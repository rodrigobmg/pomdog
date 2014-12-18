﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_DEBUGNAVIGATOR_9FDE3D88_77EB_4B69_8A1D_99504294B826_HPP
#define POMDOG_DEBUGNAVIGATOR_9FDE3D88_77EB_4B69_8A1D_99504294B826_HPP

#if (_MSC_VER > 1000)
#pragma once
#endif

#include "Control.hpp"
#include <Pomdog/Utility/Optional.hpp>
#include <Pomdog/Pomdog.hpp>
#include <deque>

namespace Pomdog {
namespace UI {

class DebugNavigator: public Control {
public:
	DebugNavigator(std::shared_ptr<GameClock> const& clock);
	
	void Draw(DrawingContext & drawingContext) override;
	
private:
	std::shared_ptr<GameClock> clock;
	std::deque<float> frameRates;
	DurationSeconds duration;
	std::string frameRateString;
};

}// namespace UI
}// namespace Pomdog

#endif // !defined(POMDOG_DEBUGNAVIGATOR_9FDE3D88_77EB_4B69_8A1D_99504294B826_HPP)
