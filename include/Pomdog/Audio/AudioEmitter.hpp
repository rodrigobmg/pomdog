﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_AUDIOEMITTER_AC108A5B_5BBD_49BD_8A71_B2DB961C9E49_HPP
#define POMDOG_AUDIOEMITTER_AC108A5B_5BBD_49BD_8A71_B2DB961C9E49_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include "Pomdog/Math/Vector3.hpp"

namespace Pomdog {

class AudioEmitter {
public:
	Vector3 Forward;
	Vector3 Position;
	Vector3 Up;
	Vector3 Velocity;
	float DopplerScale;
};

}// namespace Pomdog

#endif // !defined(POMDOG_AUDIOEMITTER_AC108A5B_5BBD_49BD_8A71_B2DB961C9E49_HPP)