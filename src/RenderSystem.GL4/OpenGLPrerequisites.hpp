﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_SRC_GL4_OPENGL_PREREQUISITES_70C95DBA_5F82_41A6_A732_5FB0470F0A3D_HPP
#define POMDOG_SRC_GL4_OPENGL_PREREQUISITES_70C95DBA_5F82_41A6_A732_5FB0470F0A3D_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include <Pomdog/Config/Platform.hpp>

#if defined(POMDOG_PLATFORM_WIN32)
#	include <GL/glew.h>
#	include <GL/wglew.h>
#elif defined(POMDOG_PLATFORM_MACOSX)
#	include <OpenGL/gl3.h>
#   include <OpenGL/gl3ext.h>
#elif defined(POMDOG_PLATFORM_LINUX)
#	include <GL/glew.h>
#endif

#endif // !defined(POMDOG_SRC_GL4_OPENGL_PREREQUISITES_70C95DBA_5F82_41A6_A732_5FB0470F0A3D_HPP)
