//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#include "SpriteRenderer.hpp"

namespace TestApp {
//-----------------------------------------------------------------------
#if defined(POMDOG_COMPILER_CLANG)
#pragma mark - SpriteRenderer::Impl
#endif
//-----------------------------------------------------------------------
class SpriteRenderer::Impl {
private:
	static constexpr std::size_t MaxBatchSize = 2048;
	static constexpr std::size_t MinBatchSize = 128;
	
	static_assert(MaxBatchSize >= MinBatchSize, "");

	struct alignas(16) SpriteInfo
	{
		// {x___} = worldMatrix.M00
		// {_y__} = worldMatrix.M01
		// {__z_} = worldMatrix.M10
		// {___w} = worldMatrix.M11
		Vector4 TransformMatrix1;
		
		// {x___} = worldMatrix.M20
		// {_y__} = worldMatrix.M21
		// {__z_} = layerDepth
		// {___w} = rotation
		Vector4 TransformMatrix2;
		
		// {xy__} = xy
		// {__zw} = {width, height}
		Vector4 SourceRect;
		
		// {xy__} = originPivot.xy
		// {__zw} = scale.xy
		Vector4 OriginScale;
		
		// {rgb_} = color.rgb
		// {___a} = color.a
		Vector4 Color;
	};

private:
	std::shared_ptr<GraphicsContext> graphicsContext;
	
	std::vector<std::vector<SpriteInfo>> spriteQueues;
	std::vector<std::shared_ptr<Texture2D>> textures;

	std::shared_ptr<VertexBuffer> planeVertices;
	std::shared_ptr<IndexBuffer> planeIndices;
	std::shared_ptr<VertexBuffer> instanceVertices;
	
	std::shared_ptr<EffectPass> effectPass;
	std::shared_ptr<InputLayout> inputLayout;
	
	Matrix4x4 projectionMatrix;
	SpriteSortMode sortMode;

public:
	Impl(std::shared_ptr<GraphicsContext> const& graphicsContext,
		std::shared_ptr<GraphicsDevice> const& graphicsDevice, AssetManager & assets);
	
	void ResetProjectionMatrix(Matrix4x4 const& projectionMatrix);
	
	void Begin(SpriteSortMode sortMode);
	
	void Begin(SpriteSortMode sortMode, Matrix4x4 const& transformMatrix);
	
	void Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
		Vector2 const& position, Color const& color,
		Radian<float> const& rotation, Vector2 const& originPivot, Vector2 const& scale, float layerDepth);
	
	void Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
		Vector2 const& position, Rectangle const& sourceRect, Color const& color,
		Radian<float> const& rotation, Vector2 const& originPivot, Vector2 const& scale, float layerDepth);
	
	void End();
	
private:
	void Flush();
	void DrawInstance(std::shared_ptr<Texture2D> const& texture, std::vector<SpriteInfo> const& sprites);
};
//-----------------------------------------------------------------------
SpriteRenderer::Impl::Impl(std::shared_ptr<GraphicsContext> const& graphicsContextIn,
	std::shared_ptr<GraphicsDevice> const& graphicsDevice, AssetManager & assets)
	: graphicsContext(graphicsContextIn)
	, sortMode(SpriteSortMode::BackToFront)
{
	using PositionTextureCoord = CustomVertex<Vector4>;

	{
		auto viewport = graphicsContext->Viewport();
		POMDOG_ASSERT(viewport.Width() > 0);
		POMDOG_ASSERT(viewport.Height() > 0);
		projectionMatrix = Matrix4x4::CreateOrthographicLH(viewport.Width(), viewport.Height(), 0.1f, 100.0f);
	}
	{
		std::array<PositionTextureCoord, 4> const verticesCombo = {
			Vector4(0.0f, 0.0f, 0.0f, 1.0f),
			Vector4(0.0f, 1.0f, 0.0f, 0.0f),
			Vector4(1.0f, 1.0f, 1.0f, 0.0f),
			Vector4(1.0f, 0.0f, 1.0f, 1.0f),
		};
		planeVertices = std::make_shared<VertexBuffer>(graphicsDevice,
			verticesCombo.data(), verticesCombo.size(),
			PositionTextureCoord::Declaration().StrideBytes(), BufferUsage::Immutable);
	}
	{
		std::array<std::uint16_t, 6> const indices = {
			0, 1, 2,
			2, 3, 0
		};

		// Create index buffer
		planeIndices = std::make_shared<IndexBuffer>(graphicsDevice,
			IndexElementSize::SixteenBits, indices.data(), indices.size(), BufferUsage::Immutable);
	}
	{
		std::array<SpriteInfo, MaxBatchSize> verticesCombo;
		std::fill(std::begin(verticesCombo), std::end(verticesCombo),
			SpriteInfo {
				Vector4(1.0f, 0.0f, 0.0f, 1.0f),
				Vector4(0.0f, 0.0f, 0.0f, 0.0f),
				Vector4(0.0f, 0.0f, 1.0f, 1.0f),
				Vector4(0.5f, 0.5f, 1.0f, 1.0f),
				Vector4(1.0f, 1.0f, 1.0f, 1.0f),
			});
		
		instanceVertices = std::make_shared<VertexBuffer>(graphicsDevice,
			verticesCombo.data(), verticesCombo.size(), sizeof(SpriteInfo), BufferUsage::Dynamic);
	}
	{
		effectPass = assets.Load<EffectPass>("Effects/SpriteRendererEffect");

		auto declartation = PositionTextureCoord::Declaration();
		using SpriteInfoVertex = CustomVertex<Vector4, Vector4, Vector4, Vector4, Vector4>;
		
		inputLayout = std::make_shared<InputLayout>(graphicsDevice, effectPass,
			std::initializer_list<VertexBufferBinding>{
				{declartation, 0, 0},
				{SpriteInfoVertex::Declaration(), declartation.StrideBytes(), 1}
			});
	}
#ifdef DEBUG
	{
		for (auto & parameter: effectPass->Parameters()) {
			Log::Stream() << "EffectParameter: " << parameter.first;
		}
		
		auto effectReflection = std::make_shared<EffectReflection>(graphicsDevice, effectPass);
	
		auto stream = Log::Stream();
		for (auto & description: effectReflection->GetConstantBuffers()) {
			stream << "-----------------------" << "\n";
			stream << "     Name: " << description.Name << "\n";
			stream << " ByteSize: " << description.ByteSize << "\n";
			stream << "Variables: " << description.Variables.size() << "\n";
		}
	}
#endif
}
//-----------------------------------------------------------------------
void SpriteRenderer::Impl::ResetProjectionMatrix(Matrix4x4 const& projectionMatrixIn)
{
	this->projectionMatrix = projectionMatrixIn;
}
//-----------------------------------------------------------------------
void SpriteRenderer::Impl::Begin(SpriteSortMode sortModeIn)
{
	this->sortMode = sortModeIn;

	alignas(16) Matrix4x4 projection = Matrix4x4::Transpose(projectionMatrix);

	auto parameter = effectPass->Parameters("Matrices");
	parameter->SetValue(projection);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Impl::Begin(SpriteSortMode sortModeIn, Matrix4x4 const& transformMatrix)
{
	this->sortMode = sortModeIn;

	alignas(16) Matrix4x4 projection = Matrix4x4::Transpose(
		transformMatrix * projectionMatrix);

	auto parameter = effectPass->Parameters("Matrices");
	parameter->SetValue(projection);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Impl::End()
{
	Flush();
}
//-----------------------------------------------------------------------
void SpriteRenderer::Impl::Flush()
{
	if (textures.empty()) {
		return;
	}
	
	for (std::size_t queuePos = 0; queuePos < textures.size(); ++queuePos)
	{
		POMDOG_ASSERT(!spriteQueues.empty());
		POMDOG_ASSERT(queuePos < spriteQueues.size());
		POMDOG_ASSERT(!spriteQueues[queuePos].empty());
		
		auto & sprites = spriteQueues[queuePos];

		switch (sortMode) {
		case SpriteSortMode::BackToFront:
			std::sort(std::begin(sprites), std::end(sprites), [](SpriteInfo const& a, SpriteInfo const& b) {
				return a.TransformMatrix2.Z > b.TransformMatrix2.Z;
			});
			break;
		case SpriteSortMode::FrontToBack:
			std::sort(std::begin(sprites), std::end(sprites), [](SpriteInfo const& a, SpriteInfo const& b) {
				return a.TransformMatrix2.Z < b.TransformMatrix2.Z;
			});
			break;
		case SpriteSortMode::Deferred:
			break;
		}
		
		DrawInstance(textures[queuePos], sprites);
		sprites.clear();
	}
	
	textures.clear();
	spriteQueues.clear();
}
//-----------------------------------------------------------------------
void SpriteRenderer::Impl::DrawInstance(std::shared_ptr<Texture2D> const& texture, std::vector<SpriteInfo> const& sprites)
{
	POMDOG_ASSERT(texture);
	POMDOG_ASSERT(sprites.size() <= MaxBatchSize);
	
	{
		POMDOG_ASSERT(texture->Width() > 0);
		POMDOG_ASSERT(texture->Height() > 0);
		
		alignas(16) Vector2 inverseTexturePixelWidth {
			(texture->Width() > 0) ? (1.0f / static_cast<float>(texture->Width())): 0.0f,
			(texture->Height() > 0) ? (1.0f / static_cast<float>(texture->Height())): 0.0f,
		};

		auto parameter = effectPass->Parameters("TextureConstants");
		parameter->SetValue(inverseTexturePixelWidth);
	}

	POMDOG_ASSERT(sprites.size() <= MaxBatchSize);
	instanceVertices->SetData(sprites.data(), static_cast<std::uint32_t>(sprites.size()));

	graphicsContext->SetTexture(0, texture);
	graphicsContext->SetInputLayout(inputLayout);
	graphicsContext->SetVertexBuffers({planeVertices, instanceVertices});
	effectPass->Apply();
	graphicsContext->DrawIndexedInstanced(PrimitiveTopology::TriangleList,
		planeIndices, planeIndices->IndexCount(), static_cast<std::uint32_t>(sprites.size()));
}
//-----------------------------------------------------------------------
void SpriteRenderer::Impl::Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
	Vector2 const& position, Color const& color,
	Radian<float> const& rotation, Vector2 const& originPivot, Vector2 const& scale, float layerDepth)
{
	if (scale.X == 0.0f || scale.Y == 0.0f) {
		return;
	}
	
	POMDOG_ASSERT(texture);

	if (spriteQueues.empty() || (textures.back() != texture)) {
		textures.push_back(texture);
		spriteQueues.push_back({});
		spriteQueues.back().reserve(MinBatchSize);
	}
	
	POMDOG_ASSERT(layerDepth >= 0.0f);
	POMDOG_ASSERT(layerDepth <= 1.0f);
	
	POMDOG_ASSERT(texture->Width() > 0);
	POMDOG_ASSERT(texture->Height() > 0);
	
	auto transform = Matrix3x2::CreateTranslation(position) * worldMatrix;
	
	SpriteInfo info;
	info.TransformMatrix1 = {transform(0, 0), transform(0, 1), transform(1, 0), transform(1, 1)};
	info.TransformMatrix2 = {transform(2, 0), transform(2, 1), layerDepth, rotation.value};
	info.SourceRect = Vector4(0, 0, texture->Width(), texture->Height());
	info.OriginScale = Vector4(originPivot.X, originPivot.Y, scale.X, scale.Y);
	info.Color = color.ToVector4();
	
	spriteQueues.back().push_back(std::move(info));
	POMDOG_ASSERT(spriteQueues.size() <= MaxBatchSize);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Impl::Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
	Vector2 const& position, Rectangle const& sourceRect, Color const& color,
	Radian<float> const& rotation, Vector2 const& originPivot, Vector2 const& scale, float layerDepth)
{
	if (scale.X == 0.0f || scale.Y == 0.0f) {
		return;
	}
	
	POMDOG_ASSERT(texture);

	if (spriteQueues.empty()
		|| (textures.back() != texture)
		|| (spriteQueues.back().size() >= MaxBatchSize))
	{
		textures.push_back(texture);
		spriteQueues.push_back({});
		spriteQueues.back().reserve(MinBatchSize);
	}
	
	POMDOG_ASSERT(layerDepth >= 0.0f);
	POMDOG_ASSERT(layerDepth <= 1.0f);
	
	POMDOG_ASSERT(sourceRect.Width > 0);
	POMDOG_ASSERT(sourceRect.Height > 0);
	
	auto transform = Matrix3x2::CreateTranslation(position) * worldMatrix;
	
	SpriteInfo info;
	info.TransformMatrix1 = {transform(0, 0), transform(0, 1), transform(1, 0), transform(1, 1)};
	info.TransformMatrix2 = {transform(2, 0), transform(2, 1), layerDepth, rotation.value};
	info.SourceRect = Vector4(sourceRect.X, sourceRect.Y, sourceRect.Width, sourceRect.Height);
	info.OriginScale = Vector4(originPivot.X, originPivot.Y, scale.X, scale.Y);
	info.Color = color.ToVector4();
	
	spriteQueues.back().push_back(std::move(info));
	POMDOG_ASSERT(spriteQueues.size() <= MaxBatchSize);
}
//-----------------------------------------------------------------------
#if defined(POMDOG_COMPILER_CLANG)
#pragma mark - SpriteRenderer
#endif
//-----------------------------------------------------------------------
SpriteRenderer::SpriteRenderer(std::shared_ptr<GraphicsContext> const& graphicsContext,
	std::shared_ptr<GraphicsDevice> const& graphicsDevice, AssetManager & assets)
	: impl(std::unique_ptr<Impl>(new Impl(graphicsContext, graphicsDevice, assets)))
{}
//-----------------------------------------------------------------------
SpriteRenderer::~SpriteRenderer() = default;
//-----------------------------------------------------------------------
void ResetProjectionMatrix(Matrix4x4 const& projectionMatrix)
{
}
//-----------------------------------------------------------------------
void SpriteRenderer::SetProjectionMatrix(Matrix4x4 const& projectionMatrix)
{
	POMDOG_ASSERT(impl);
	impl->ResetProjectionMatrix(projectionMatrix);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Begin(SpriteSortMode sortMode)
{
	POMDOG_ASSERT(impl);
	impl->Begin(sortMode);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Begin(SpriteSortMode sortMode, Matrix4x4 const& transformMatrixIn)
{
	POMDOG_ASSERT(impl);
	impl->Begin(sortMode, transformMatrixIn);
}
//-----------------------------------------------------------------------
void SpriteRenderer::End()
{
	POMDOG_ASSERT(impl);
	impl->End();
}
//-----------------------------------------------------------------------
void SpriteRenderer::Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
	Rectangle const& sourceRect, Color const& color)
{
	POMDOG_ASSERT(impl);
	impl->Draw(texture, worldMatrix, {0, 0}, sourceRect, color, 0, {0.5f, 0.5f}, {1.0f, 1.0f}, 0.0f);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
	Vector2 const& position, Color const& color)
{
	POMDOG_ASSERT(impl);
	impl->Draw(texture, worldMatrix, {0, 0}, color, 0, {0.5f, 0.5f}, {1.0f, 1.0f}, 0.0f);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
	Vector2 const& position, Rectangle const& sourceRect, Color const& color)
{
	POMDOG_ASSERT(impl);
	impl->Draw(texture, worldMatrix, position, sourceRect, color, 0, {0.5f, 0.5f}, {1.0f, 1.0f}, 0.0f);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
	Vector2 const& position, Rectangle const& sourceRect, Color const& color,
	Radian<float> const& rotation, Vector2 const& originPivot, float scale, float layerDepth)
{
	POMDOG_ASSERT(impl);
	impl->Draw(texture, worldMatrix, position, sourceRect, color, rotation, originPivot, {scale, scale}, layerDepth);
}
//-----------------------------------------------------------------------
void SpriteRenderer::Draw(std::shared_ptr<Texture2D> const& texture, Matrix3x2 const& worldMatrix,
	Vector2 const& position, Rectangle const& sourceRect, Color const& color,
	Radian<float> const& rotation, Vector2 const& originPivot, Vector2 const& scale, float layerDepth)
{
	POMDOG_ASSERT(impl);
	impl->Draw(texture, worldMatrix, position, sourceRect, color, rotation, originPivot, scale, layerDepth);
}
//-----------------------------------------------------------------------
}// namespace TestApp