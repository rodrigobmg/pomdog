﻿//
//  Copyright (C) 2013-2015 mogemimi.
//  Distributed under the MIT License. See LICENSE.md or
//  http://enginetrouble.net/pomdog/license for details.
//

#ifndef POMDOG_SHADERLANGUAGE_D009E979_B7EC_447B_8628_15130E02F6EB_HPP
#define POMDOG_SHADERLANGUAGE_D009E979_B7EC_447B_8628_15130E02F6EB_HPP

#if (_MSC_VER > 1000)
#pragma once
#endif

#include <cstdint>

namespace Pomdog {

enum class ShaderLanguage: std::uint8_t {
	///@brief The OpenGL shading language (GLSL)
	GLSL,

	///@brief The DirectX high-level shading language (HLSL)
	HLSL,

	///@brief The Metal shading language
	Metal,
};

}// namespace Pomdog

#endif // !defined(POMDOG_SHADERLANGUAGE_D009E979_B7EC_447B_8628_15130E02F6EB_HPP)
