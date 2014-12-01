﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#include "GraphicsDeviceDirect3D11.hpp"
#include "BlendStateDirect3D11.hpp"
#include "ConstantBufferDirect3D11.hpp"
#include "DepthStencilStateDirect3D11.hpp"
#include "EffectPassDirect3D11.hpp"
#include "EffectReflectionDirect3D11.hpp"
#include "IndexBufferDirect3D11.hpp"
#include "InputLayoutDirect3D11.hpp"
#include "RasterizerStateDirect3D11.hpp"
#include "RenderTarget2DDirect3D11.hpp"
#include "SamplerStateDirect3D11.hpp"
#include "Texture2DDirect3D11.hpp"
#include "VertexBufferDirect3D11.hpp"
#include <Pomdog/Graphics/detail/ShaderBytecode.hpp>
#include <Pomdog/Logging/Log.hpp>
#include <Pomdog/Utility/StringFormat.hpp>
#include <Pomdog/Utility/Assert.hpp>
#include <Pomdog/Utility/Exception.hpp>
#include <array>

namespace Pomdog {
namespace Details {
namespace RenderSystem {
namespace Direct3D11 {
namespace {

using Microsoft::WRL::ComPtr;

class AdapterManager final {
private:
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	ComPtr<IDXGIAdapter1> activeAdapter;

public:
	void EnumAdapters();
	void Clear();

	IDXGIAdapter1* ActiveAdapter() const
	{
		POMDOG_ASSERT(activeAdapter);
		return activeAdapter.Get();
	}

	ComPtr<IDXGIFactory1> GetFactory()
	{
		ComPtr<IDXGIFactory1> dxgiFactory;

		POMDOG_ASSERT(activeAdapter);
		HRESULT hr = activeAdapter->GetParent(__uuidof(IDXGIFactory1), &dxgiFactory);
		if (FAILED(hr))
		{
			///@todo throw exception

			// FUS RO DAH!
			Log::Internal("Failed to get DXGIFactory1");
		}

		return std::move(dxgiFactory);
	}
};
//-----------------------------------------------------------------------
void AdapterManager::EnumAdapters()
{
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));

	if (FAILED(hr))
	{
		// error
		///@todo throw exception
		Log::Internal("Failed to create DXGIFactory1.");
		return;
	}

	UINT i = 0;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> newAdapter;
	while ((hr = dxgiFactory->EnumAdapters1(i, &newAdapter)) != DXGI_ERROR_NOT_FOUND)
	{
		if (FAILED(hr))
		{
			// error
			///@todo throw exception
			Log::Internal("Failed to enumerate adapters.");
			break;
		}

		adapters.push_back(newAdapter);
		newAdapter.Reset();
		++i;
	}

	if (adapters.empty())
	{
		// error
		///@todo throw exception
		return;
	}

	POMDOG_ASSERT(!adapters.empty());
	activeAdapter = adapters.front();
}
//-----------------------------------------------------------------------
void AdapterManager::Clear()
{
	activeAdapter.Reset();

	for (auto & adapter : adapters) {
		adapter.Reset();
	}
	adapters.clear();
}
//-----------------------------------------------------------------------
static ID3D11InfoQueue* BuildInfoQueue(ID3D11Device* nativeDevice)
{
	POMDOG_ASSERT(nativeDevice);

	ID3D11InfoQueue* infoQueue = nullptr;
	HRESULT hr = nativeDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));

	POMDOG_ASSERT(SUCCEEDED(hr));

	if (FAILED(hr)) {
		// error
		///@todo throw exception
	}

	infoQueue->ClearRetrievalFilter();
	infoQueue->ClearStorageFilter();

	D3D11_INFO_QUEUE_FILTER filter;
	ZeroMemory(&filter, sizeof(filter));

	std::array<D3D11_MESSAGE_SEVERITY, 4> severityList = {
		D3D11_MESSAGE_SEVERITY_CORRUPTION,
		D3D11_MESSAGE_SEVERITY_ERROR,
		D3D11_MESSAGE_SEVERITY_WARNING,
		D3D11_MESSAGE_SEVERITY_INFO,
	};

	filter.DenyList.pSeverityList = severityList.data();
	filter.DenyList.NumSeverities = severityList.size();

	infoQueue->AddStorageFilterEntries(&filter);
	infoQueue->AddRetrievalFilterEntries(&filter);

	return std::move(infoQueue);
}
//-----------------------------------------------------------------------
static void CheckError(ID3D11InfoQueue* infoQueue)
{
	POMDOG_ASSERT(infoQueue);

	const auto storedMessageCount = infoQueue->GetNumStoredMessages();

	std::string message;
	for (UINT64 i = 0; i < storedMessageCount; ++i)
	{
		SIZE_T messageByteLength = 0;
		infoQueue->GetMessage(i, nullptr, &messageByteLength);

		if (messageByteLength <= 0) {
			continue;
		}

		std::vector<D3D11_MESSAGE> messageBuffer(messageByteLength);
		infoQueue->GetMessage(i, messageBuffer.data(), &messageByteLength);
		message += messageBuffer.data()->pDescription;
		message += "\n";
	}

	if (message.empty())
	{
		Log::Internal(message);
	}
}

}// unnamed namespace
//-----------------------------------------------------------------------
class GraphicsDeviceDirect3D11::Impl {
public:
	AdapterManager adapters;
	ComPtr<ID3D11Device> nativeDevice;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<ID3D11InfoQueue> infoQueue;
	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

public:
	Impl();

private:
	void BuildDevice();
};
//-----------------------------------------------------------------------
GraphicsDeviceDirect3D11::Impl::Impl()
	: driverType(D3D_DRIVER_TYPE_NULL)
	, featureLevel(D3D_FEATURE_LEVEL_11_0)
{
	adapters.EnumAdapters();
	BuildDevice();

	infoQueue = BuildInfoQueue(nativeDevice.Get());
}
//-----------------------------------------------------------------------
void GraphicsDeviceDirect3D11::Impl::BuildDevice()
{
	POMDOG_ASSERT(!nativeDevice);
	POMDOG_ASSERT(!deviceContext);

	UINT createDeviceFlags = 0;
#if defined(DEBUG) && !defined(NDEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// NOTE: supported Direct2D1
	//createDeviceFlags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	constexpr std::array<D3D_DRIVER_TYPE, 3> driverTypes = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	constexpr std::array<D3D_FEATURE_LEVEL, 6> featureLevels = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT	hr = S_OK;

	auto adapter = adapters.ActiveAdapter();
	for (auto & type: driverTypes)
	{
		driverType = type;
		hr = D3D11CreateDevice(adapter
			, (nullptr != adapter ? D3D_DRIVER_TYPE_UNKNOWN : driverType)
			, nullptr
			, createDeviceFlags
			, featureLevels.data()
			, static_cast<UINT>(featureLevels.size())
			, D3D11_SDK_VERSION
			, &nativeDevice
			, &featureLevel
			, &deviceContext);

		if (SUCCEEDED(hr)) {
			break;
		}
	}

	if (FAILED(hr))
	{
		///@todo throw exception

		// FUS RO DAH!
		Log::Internal("Failed to create ID3D11Device");
		return;
	}

#if defined(DEBUG) && !defined(NDEBUG)
	{
		switch (driverType) {
		case D3D_DRIVER_TYPE_HARDWARE:
			Log::Internal("Direct3D11 DriverType: Hardware Device.");
			break;
		case D3D_DRIVER_TYPE_WARP:
			Log::Internal("Direct3D11 DriverType: Warp Device.");
			break;
		case D3D_DRIVER_TYPE_REFERENCE:
			Log::Internal("Direct3D11 DriverType: Reference Device.");
			break;
		};
	}
#endif
}
//-----------------------------------------------------------------------
GraphicsDeviceDirect3D11::GraphicsDeviceDirect3D11()
	: impl(std::make_unique<Impl>())
{}
//-----------------------------------------------------------------------
GraphicsDeviceDirect3D11::~GraphicsDeviceDirect3D11() = default;
//-----------------------------------------------------------------------
std::unique_ptr<NativeIndexBuffer>
GraphicsDeviceDirect3D11::CreateIndexBuffer(std::uint32_t sizeInBytes, BufferUsage bufferUsage)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);
	POMDOG_ASSERT(impl->deviceContext);

	return std::make_unique<IndexBufferDirect3D11>(
		impl->nativeDevice.Get(), impl->deviceContext, sizeInBytes, bufferUsage);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeIndexBuffer>
GraphicsDeviceDirect3D11::CreateIndexBuffer(void const* indices,
	std::uint32_t sizeInBytes, BufferUsage bufferUsage)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);
	POMDOG_ASSERT(impl->deviceContext);

	return std::make_unique<IndexBufferDirect3D11>(
		impl->nativeDevice.Get(), impl->deviceContext, indices, sizeInBytes, bufferUsage);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeVertexBuffer>
GraphicsDeviceDirect3D11::CreateVertexBuffer(std::uint32_t sizeInBytes, BufferUsage bufferUsage)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);
	POMDOG_ASSERT(impl->deviceContext);

	try {
		return std::make_unique<VertexBufferDirect3D11>(
			impl->nativeDevice.Get(), impl->deviceContext, sizeInBytes, bufferUsage);
	}
	catch (std::exception const& e) {
	#if defined(DEBUG) && !defined(NDEBUG)
		CheckError(impl->infoQueue.Get());
	#endif
		throw e;
	}
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeVertexBuffer>
GraphicsDeviceDirect3D11::CreateVertexBuffer(void const* vertices,
	std::uint32_t sizeInBytes, BufferUsage bufferUsage)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);
	POMDOG_ASSERT(impl->deviceContext);

	return std::make_unique<VertexBufferDirect3D11>(
		impl->nativeDevice.Get(), impl->deviceContext, vertices, sizeInBytes, bufferUsage);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeBlendState>
GraphicsDeviceDirect3D11::CreateBlendState(BlendDescription const& description)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);

	return std::make_unique<BlendStateDirect3D11>(impl->nativeDevice.Get(), description);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeDepthStencilState>
GraphicsDeviceDirect3D11::CreateDepthStencilState(DepthStencilDescription const& description)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);

	return std::make_unique<DepthStencilStateDirect3D11>(impl->nativeDevice.Get(), description);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeSamplerState>
GraphicsDeviceDirect3D11::CreateSamplerState(SamplerDescription const& description)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);

	return std::make_unique<SamplerStateDirect3D11>(impl->nativeDevice.Get(), description);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeRasterizerState>
GraphicsDeviceDirect3D11::CreateRasterizerState(RasterizerDescription const& description)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);

	return std::make_unique<RasterizerStateDirect3D11>(impl->nativeDevice.Get(), description);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeEffectPass>
GraphicsDeviceDirect3D11::CreateEffectPass(ShaderBytecode const& vertexShaderBytecode, ShaderBytecode const& pixelShaderBytecode)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);

	return std::make_unique<EffectPassDirect3D11>(impl->nativeDevice.Get(),
		vertexShaderBytecode, pixelShaderBytecode);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeConstantBuffer>
GraphicsDeviceDirect3D11::CreateConstantBuffer(std::uint32_t byteConstants)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);
	POMDOG_ASSERT(impl->deviceContext);

	return std::make_unique<ConstantBufferDirect3D11>(
		impl->nativeDevice.Get(), impl->deviceContext.Get(), byteConstants);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeEffectReflection>
GraphicsDeviceDirect3D11::CreateEffectReflection(NativeEffectPass & nativeEffectPass)
{
	auto const effectPass = dynamic_cast<EffectPassDirect3D11*>(&nativeEffectPass);
	POMDOG_ASSERT(effectPass != nullptr);

	if (!effectPass) {
		// FUS RO DAH!
		///@todo throw exception
		return {};
	}

	return std::make_unique<EffectReflectionDirect3D11>(
		effectPass->GetVertexShaderBlob(), effectPass->GetPixelShaderBlob());
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeInputLayout>
GraphicsDeviceDirect3D11::CreateInputLayout(NativeEffectPass & nativeEffectPass,
	std::vector<VertexBufferBinding> const& vertexBufferBindings)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);

	auto const effectPass = dynamic_cast<EffectPassDirect3D11*>(&nativeEffectPass);
	POMDOG_ASSERT(effectPass != nullptr);

	if (!effectPass) {
		// FUS RO DAH!
		///@todo throw exception
		return {};
	}

	return std::make_unique<InputLayoutDirect3D11>(impl->nativeDevice.Get(),
		effectPass->GetVertexShaderBlob(), vertexBufferBindings);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeTexture2D>
GraphicsDeviceDirect3D11::CreateTexture2D(std::int32_t width, std::int32_t height, std::uint32_t mipmapLevels,
	SurfaceFormat format)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);
	POMDOG_ASSERT(impl->deviceContext);

	return std::make_unique<Texture2DDirect3D11>(impl->nativeDevice.Get(),
		impl->deviceContext, width, height, mipmapLevels, format);
}
//-----------------------------------------------------------------------
std::unique_ptr<NativeRenderTarget2D>
GraphicsDeviceDirect3D11::CreateRenderTarget2D(std::int32_t width, std::int32_t height,
	std::uint32_t mipmapLevels, SurfaceFormat format, DepthFormat depthStencilFormat)
{
	POMDOG_ASSERT(impl);
	POMDOG_ASSERT(impl->nativeDevice);
	return std::make_unique<RenderTarget2DDirect3D11>(impl->nativeDevice.Get(),
		width, height, mipmapLevels, format, depthStencilFormat);
}
//-----------------------------------------------------------------------
Microsoft::WRL::ComPtr<ID3D11DeviceContext> GraphicsDeviceDirect3D11::DeviceContext() const
{
	POMDOG_ASSERT(impl);
	return impl->deviceContext;
}
//-----------------------------------------------------------------------
Microsoft::WRL::ComPtr<ID3D11Device> GraphicsDeviceDirect3D11::NativeDevice() const
{
	POMDOG_ASSERT(impl);
	return impl->nativeDevice;
}
//-----------------------------------------------------------------------
Microsoft::WRL::ComPtr<IDXGIFactory1> GraphicsDeviceDirect3D11::DXGIFactory() const
{
	POMDOG_ASSERT(impl);
	return impl->adapters.GetFactory();
}
//-----------------------------------------------------------------------
}// namespace Direct3D11
}// namespace RenderSystem
}// namespace Details
}// namespace Pomdog