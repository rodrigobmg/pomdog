// Copyright (c) 2013-2015 mogemimi.
// Distributed under the MIT license. See LICENSE.md file for details.

#include "GraphicsDeviceGL4.hpp"
#include "ConstantBufferGL4.hpp"
#include "EffectPassGL4.hpp"
#include "EffectReflectionGL4.hpp"
#include "IndexBufferGL4.hpp"
#include "SamplerStateGL4.hpp"
#include "ShaderGL4.hpp"
#include "Texture2DGL4.hpp"
#include "RenderTarget2DGL4.hpp"
#include "VertexBufferGL4.hpp"
#include "../RenderSystem/ShaderCompileOptions.hpp"
#include "Pomdog/Graphics/ShaderLanguage.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Utility/Exception.hpp"

namespace Pomdog {
namespace Detail {
namespace RenderSystem {
namespace GL4 {
//-----------------------------------------------------------------------
ShaderLanguage GraphicsDeviceGL4::GetSupportedLanguage() const
{
    return ShaderLanguage::GLSL;
}
//-----------------------------------------------------------------------
std::unique_ptr<Shader>
GraphicsDeviceGL4::CreateShader(ShaderBytecode const& shaderBytecode,
    ShaderCompileOptions const& compileOptions)
{
    switch (compileOptions.Profile.PipelineStage) {
    case ShaderPipelineStage::VertexShader: {
        return std::make_unique<VertexShaderGL4>(shaderBytecode);
    }
    case ShaderPipelineStage::PixelShader: {
        return std::make_unique<PixelShaderGL4>(shaderBytecode);
    }
    }

    POMDOG_THROW_EXCEPTION(std::domain_error, "Failed to create shader");
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeBuffer>
GraphicsDeviceGL4::CreateConstantBuffer(std::size_t byteConstants)
{
    return std::make_unique<ConstantBufferGL4>(byteConstants);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeBuffer>
GraphicsDeviceGL4::CreateIndexBuffer(std::size_t sizeInBytes, BufferUsage bufferUsage)
{
    return std::make_unique<IndexBufferGL4>(sizeInBytes, bufferUsage);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeBuffer>
GraphicsDeviceGL4::CreateIndexBuffer(void const* indices,
    std::size_t sizeInBytes, BufferUsage bufferUsage)
{
    return std::make_unique<IndexBufferGL4>(indices, sizeInBytes, bufferUsage);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeBuffer>
GraphicsDeviceGL4::CreateVertexBuffer(std::size_t sizeInBytes, BufferUsage bufferUsage)
{
    return std::make_unique<VertexBufferGL4>(sizeInBytes, bufferUsage);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeBuffer>
GraphicsDeviceGL4::CreateVertexBuffer(void const* vertices,
    std::size_t sizeInBytes, BufferUsage bufferUsage)
{
    return std::make_unique<VertexBufferGL4>(vertices, sizeInBytes, bufferUsage);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeSamplerState>
GraphicsDeviceGL4::CreateSamplerState(SamplerDescription const& description)
{
    return std::make_unique<SamplerStateGL4>(description);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeEffectPass>
GraphicsDeviceGL4::CreateEffectPass(EffectPassDescription const& description)
{
    return std::make_unique<EffectPassGL4>(description);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeEffectReflection>
GraphicsDeviceGL4::CreateEffectReflection(NativeEffectPass & nativeEffectPass)
{
    auto const effectPassGL4 = dynamic_cast<EffectPassGL4*>(&nativeEffectPass);
    POMDOG_ASSERT(effectPassGL4 != nullptr);

    return std::make_unique<EffectReflectionGL4>(effectPassGL4->GetShaderProgram());
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeTexture2D>
GraphicsDeviceGL4::CreateTexture2D(std::int32_t width, std::int32_t height, std::uint32_t mipmapLevels,
    SurfaceFormat format)
{
    return std::make_unique<Texture2DGL4>(width, height, mipmapLevels, format);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeRenderTarget2D>
GraphicsDeviceGL4::CreateRenderTarget2D(std::int32_t width, std::int32_t height,
    std::uint32_t mipmapLevels, SurfaceFormat format, DepthFormat depthStencilFormat)
{
    return std::make_unique<RenderTarget2DGL4>(width, height, mipmapLevels, format, depthStencilFormat);
}
//-----------------------------------------------------------------------
} // namespace GL4
} // namespace RenderSystem
} // namespace Detail
} // namespace Pomdog
