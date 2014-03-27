//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_TEXTURE2D_F041ED3F_9089_4056_BC4F_1C145612778F_HPP
#define POMDOG_TEXTURE2D_F041ED3F_9089_4056_BC4F_1C145612778F_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include <cstdint>
#include <memory>
#include "../Config/Export.hpp"
#include "detail/ForwardDeclarations.hpp"
#include "Texture.hpp"
#include "SurfaceFormat.hpp"

namespace Pomdog {
namespace Details {
namespace RenderSystem {

class NativeTexture2D;

}// namespace RenderSystem
}// namespace Details

/// @addtogroup Framework
/// @{
/// @addtogroup Graphics
/// @{

///@~Japanese
/// @brief 2 次元テクスチャです。
class POMDOG_EXPORT Texture2D : public Texture {
public:
	Texture2D() = delete;
	Texture2D(Texture2D const&) = delete;
	Texture2D(Texture2D &&) = default;

	Texture2D(std::shared_ptr<GraphicsDevice> const& graphicsDevice,
		std::uint32_t width, std::uint32_t height);

	Texture2D(std::shared_ptr<GraphicsDevice> const& graphicsDevice,
		std::uint32_t width, std::uint32_t height, std::uint32_t levelCount, SurfaceFormat format);

	~Texture2D();
	
	Texture2D & operator=(Texture2D const&) = delete;
	Texture2D & operator=(Texture2D &&) = default;

	///@~Japanese
	/// @brief テクスチャの水平方向の幅（ピクセル単位）を取得します。
	std::uint32_t Width() const;

	///@~Japanese
	/// @brief テクスチャの垂直方向の幅（ピクセル単位）を取得します。
	std::uint32_t Height() const;

	///@~Japanese
	/// @brief ミップマップレベルを取得します。
	/// @remarks ミップマップレベルは 1 以上を有効とします。
	std::uint32_t LevelCount() const;

	///@~Japanese
	/// @brief テクスチャのフォーマットを取得します。
	SurfaceFormat Format() const;
	
	///@~Japanese
	/// @brief ピクセルデータを格納します。
	void SetData(void const* pixelData);

public:
	Details::RenderSystem::NativeTexture2D* NativeTexture2D();
	
private:
	std::unique_ptr<Details::RenderSystem::NativeTexture2D> nativeTexture2D;
	std::uint32_t pixelWidth;
	std::uint32_t pixelHeight;
	std::uint32_t levelCount;
	SurfaceFormat format;
};

/// @}
/// @}

}// namespace Pomdog

#endif // !defined(POMDOG_TEXTURE2D_F041ED3F_9089_4056_BC4F_1C145612778F_HPP)
