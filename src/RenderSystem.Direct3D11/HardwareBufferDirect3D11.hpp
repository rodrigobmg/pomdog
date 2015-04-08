// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#ifndef POMDOG_HARDWAREBUFFERDIRECT3D11_58714643_HPP
#define POMDOG_HARDWAREBUFFERDIRECT3D11_58714643_HPP

#include "PrerequisitesDirect3D11.hpp"
#include "../RenderSystem/NativeBuffer.hpp"
#include "Pomdog/Graphics/detail/ForwardDeclarations.hpp"

namespace Pomdog {
namespace Detail {
namespace RenderSystem {
namespace Direct3D11 {

class HardwareBufferDirect3D11 final : public NativeBuffer {
public:
    HardwareBufferDirect3D11(ID3D11Device* nativeDevice,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> const& deviceContext,
        std::size_t sizeInBytes,
        BufferUsage bufferUsage, D3D11_BIND_FLAG bindFlag);

    HardwareBufferDirect3D11(ID3D11Device* nativeDevice,
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> const& deviceContext,
        void const* vertices, std::size_t sizeInBytes,
        BufferUsage bufferUsage, D3D11_BIND_FLAG bindFlag);

    void GetData(std::size_t sizeInBytes, void* result) const override;

    void SetData(std::size_t offsetInBytes, void const* source,
        std::size_t sizeInBytes) override;

    ID3D11Buffer* GetBuffer() const;

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> nativeBuffer;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
};

} // namespace Direct3D11
} // namespace RenderSystem
} // namespace Detail
} // namespace Pomdog

#endif // POMDOG_HARDWAREBUFFERDIRECT3D11_58714643_HPP
