//
//  Copyright (C) 2013 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#include <Pomdog/Graphics/GraphicsDevice.hpp>
#include <utility>
#include <Pomdog/Utility/Assert.hpp>
#include "../RenderSystem/NativeGraphicsDevice.hpp"

namespace Pomdog {
//-----------------------------------------------------------------------
GraphicsDevice::GraphicsDevice(std::unique_ptr<Details::RenderSystem::NativeGraphicsDevice> device)
	: nativeGraphicsDevice(std::move(device))
{
	POMDOG_ASSERT(nativeGraphicsDevice);
}
//-----------------------------------------------------------------------
GraphicsDevice::~GraphicsDevice()
{
}
//-----------------------------------------------------------------------
Details::RenderSystem::NativeGraphicsDevice* GraphicsDevice::GetNativeGraphicsDevice()
{
	POMDOG_ASSERT(nativeGraphicsDevice);
	return nativeGraphicsDevice.get();
}
//-----------------------------------------------------------------------
}// namespace Pomdog
