// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#ifndef POMDOG_PREREQUISITESDIRECTINPUT_98459263_302E_4170_99E5_E4E0F212DAFC_HPP
#define POMDOG_PREREQUISITESDIRECTINPUT_98459263_302E_4170_99E5_E4E0F212DAFC_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Pomdog/Platform/Win32/PrerequisitesWin32.hpp"

#ifndef DIRECTINPUT_VERSION
#    ifdef DIRECTINPUT_HEADER_VERSION
#        define DIRECTINPUT_VERSION DIRECTINPUT_HEADER_VERSION
#    else
#        define DIRECTINPUT_VERSION 0x0800
#    endif
#endif
#include <dinput.h>

#endif // !defined(POMDOG_PREREQUISITESDIRECTINPUT_98459263_302E_4170_99E5_E4E0F212DAFC_HPP)
