﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#ifndef POMDOG_CONSTANTBUFFERDIRECT3D11_FCEBAAAC_FF4D_4266_B8C4_BF5A4A512E72_HPP
#define POMDOG_CONSTANTBUFFERDIRECT3D11_FCEBAAAC_FF4D_4266_B8C4_BF5A4A512E72_HPP

#if (_MSC_VER > 1000)
#	pragma once
#endif

#include "PrerequisitesDirect3D11.hpp"
#include "../RenderSystem/NativeConstantBuffer.hpp"
#include <Pomdog/Graphics/detail/ForwardDeclarations.hpp>

namespace Pomdog {
namespace Details {
namespace RenderSystem {
namespace Direct3D11 {

class ConstantBufferDirect3D11 final: public NativeConstantBuffer {
public:
	ConstantBufferDirect3D11() = delete;

	ConstantBufferDirect3D11(ID3D11Device* nativeDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> const& deviceContext,
		std::uint32_t sizeInBytes);

	///@copydoc NativeConstantBuffer
	void GetData(std::uint32_t byteWidth, void* result) const override;

	///@copydoc NativeConstantBuffer
	void SetData(std::uint32_t offsetInBytes,
		void const* source, std::uint32_t sizeInBytes) override;

	ID3D11Buffer* NativeBuffer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> nativeConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
};

}// namespace Direct3D11
}// namespace RenderSystem
}// namespace Details
}// namespace Pomdog

#endif // !defined(POMDOG_CONSTANTBUFFERDIRECT3D11_FCEBAAAC_FF4D_4266_B8C4_BF5A4A512E72_HPP)