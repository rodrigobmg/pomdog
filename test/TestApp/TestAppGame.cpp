//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#include "TestAppGame.hpp"
#include <utility>
#include "PrimitiveAxes.hpp"
#include "PrimitiveGrid.hpp"
#include "SpriteBatch.hpp"
#include "SpriteRenderer.hpp"
#include "FXAA.hpp"
#include "TextureAtlasLoader.hpp"

namespace TestApp {
namespace {

//static Matrix3x3 CreateViewMatrix2D(Transform2D const& transform, Camera2D const& camera)
//{
//	return Matrix3x3::CreateTranslation({-transform.Position.X, -transform.Position.Y})*
//		Matrix3x3::CreateRotationZ(-transform.Rotation) *
//		Matrix3x3::CreateScale({camera.Zoom(), camera.Zoom(), 1});
//}
//-----------------------------------------------------------------------
static Matrix4x4 CreateViewMatrix3D(Transform2D const& transform, Camera2D const& camera)
{
	return Matrix4x4::CreateTranslation({-transform.Position.X, -transform.Position.Y, 1.0f})*
		Matrix4x4::CreateRotationZ(-transform.Rotation) *
		Matrix4x4::CreateScale({camera.Zoom(), camera.Zoom(), 1});
}

}// unnamed namespace
//-----------------------------------------------------------------------
TestAppGame::TestAppGame(std::shared_ptr<GameHost> const& gameHostIn)
	: gameHost(gameHostIn)
{
	POMDOG_ASSERT(gameHostIn);
	graphicsContext = gameHost->GraphicsContext();
}
//-----------------------------------------------------------------------
TestAppGame::~TestAppGame() = default;
//-----------------------------------------------------------------------
void TestAppGame::Initialize()
{
	auto window = gameHost->Window();
	window->Title("TestApp - Enjoy Game Dev, Have Fun.");
	window->AllowPlayerResizing(false);
	
	auto graphicsDevice = gameHost->GraphicsDevice();
	auto assets = gameHost->AssetManager();

	{
		samplerPoint = SamplerState::CreatePointWrap(graphicsDevice);
		
		auto blendState = BlendState::CreateNonPremultiplied(graphicsDevice);
		graphicsContext->SetBlendState(blendState);
		texture = assets->Load<Texture2D>("pomdog.png");
	}
	{
		renderTarget = std::make_shared<RenderTarget2D>(graphicsDevice,
			window->ClientBounds().Width, window->ClientBounds().Height,
			false, SurfaceFormat::R8G8B8A8_UNorm, DepthFormat::None);
	}
	
	primitiveAxes = std::unique_ptr<PrimitiveAxes>(new PrimitiveAxes(gameHost));
	primitiveGrid = std::unique_ptr<PrimitiveGrid>(new PrimitiveGrid(gameHost));
	spriteRenderer = std::unique_ptr<SpriteRenderer>(new SpriteRenderer(gameHost));
	fxaa = std::unique_ptr<FXAA>(new FXAA(gameHost));
	
	{
		// NOTE: Create main camera:
		auto gameObject = gameWorld.CreateObject();;
		mainCameraID = gameObject->ID();
		
		auto node = gameObject->AddComponent<Node2D>(gameObject);
		gameObject->AddComponent<CanvasItem>();
		gameObject->AddComponent<Camera2D>();
		//auto sprite = gameObject->AddComponent<Sprite>();
		//sprite->Origin = Vector2{0.5f, 0.5f};
		//sprite->Subrect = Rectangle(0, 0, texture->Width(), texture->Height());//Rectangle(0, 0, 16, 28);
		node->Transform().Scale = Vector2{2.5f, 2.5f};
	}
	
	{
		auto gameObject = gameWorld.CreateObject();
		rootObjectID = gameObject->ID();
		
		auto node = gameObject->AddComponent<Node2D>(gameObject);
		auto sprite = gameObject->AddComponent<Sprite>();
		sprite->Origin = Vector2{0.5f, 0.5f};
		sprite->Subrect = Rectangle(0, 0, texture->Width(), texture->Height());
	
		auto & transform = node->Transform();
		transform.Position = {0, 0};
		transform.Scale = {2, 2};
	}
	
	auto rootNode = gameWorld.Component<Node2D>(rootObjectID);
	
	for (int i = 0; i < 10; ++i)
	{
		auto gameObject = gameWorld.CreateObject();
		gameObject->AddComponent<CanvasItem>();
		auto node = gameObject->AddComponent<Node2D>(gameObject);
		auto sprite = gameObject->AddComponent<Sprite>();
		auto & transform = node->Transform();
		
		transform.Position.X = i * 64 * 2.0f;
		transform.Position.Y = 0;
		transform.Scale.X = transform.Scale.Y = 2.0f;
		transform.Rotation = (0.5f * i) * MathConstants<float>::PiOver4();
		sprite->Origin = Vector2{0.5f, 0.5f};
		sprite->Subrect = Rectangle(0, 0, texture->Width(), texture->Height());//Rectangle(0, 0, 16, 28);
		
		rootNode->AddChild(gameObject);
	}
	
	{
		auto textureAtlas = assets->Load<Details::TexturePacker::TextureAtlas>("MaidChan/parts.atlas");
		
		Log::Info(StringFormat("TextureAtlas.Pages = %ld", textureAtlas.pages.size()));
		Log::Info(StringFormat("TextureAtlas.Regions = %ld", textureAtlas.regions.size()));
		
		for (auto page: textureAtlas.pages)
		{
			Log::Info(StringFormat("Page.Name = %s", page.Name.c_str()));
		}
		
		for (auto region: textureAtlas.regions)
		{
			Log::Info(StringFormat("Region.Name = %s", region.Name.c_str()));
			Log::Info(StringFormat("  Region.Page = %d", region.TexturePage));
			Log::Info(StringFormat("  Region.Rotate = %d", region.Rotate?1:0));
			Log::Info(StringFormat("  Region.Flip = %d", region.Flip?1:0));
			Log::Info(StringFormat("  Region.XY = %d, %d", (int)region.Subrect.X, (int)region.Subrect.Y));
			Log::Info(StringFormat("  Region.Size = %d, %d", (int)region.Subrect.Width, (int)region.Subrect.Height));
			Log::Info(StringFormat("  Region.Offset = %d %d", (int)region.XOffset, (int)region.YOffset));
			Log::Info(StringFormat("  Region.Advance = %d %d", (int)region.OriginalWidth, (int)region.OriginalHeight));
		}
	}
}
//-----------------------------------------------------------------------
void TestAppGame::Update()
{
	auto clock = gameHost->Clock();
	auto mouse = gameHost->Mouse();
	{
		auto node = gameWorld.Component<Node2D>(mainCameraID);
		auto camera = gameWorld.Component<Camera2D>(mainCameraID);
		
		if (node && camera)
		{
			cameraView.Input(mouse->State(), *clock, graphicsContext->Viewport().Bounds, node->Transform(), *camera);
		}
	}
	
	if (auto rootNode = gameWorld.Component<Node2D>(rootObjectID))
	{
		auto & transform = rootNode->Transform();
		if (mouse->State().RightButton == ButtonState::Pressed) {
			transform.Position.Y += 0.5f;
		}
	}
	
	{
		static auto duration = DurationSeconds(0);
		
		if (clock->TotalGameTime() - duration > DurationSeconds(0.2)) {
			gameHost->Window()->Title(StringFormat("%f fps", clock->FrameRate()));
			duration = clock->TotalGameTime();
		}
	}
}
//-----------------------------------------------------------------------
void TestAppGame::DrawSprites()
{
	auto camera = gameWorld.Component<Camera2D>(mainCameraID);
	auto nodeCamera = gameWorld.Component<Node2D>(mainCameraID);
	
	auto vierMatrix3D = CreateViewMatrix3D(nodeCamera->Transform(), *camera);;
	auto projectionMatrix3D = Matrix4x4::CreateOrthographicLH(800.0f, 480.0f, 0.1f, 1000.0f);
	
	POMDOG_ASSERT(primitiveGrid);
	primitiveGrid->Draw(*graphicsContext, vierMatrix3D * projectionMatrix3D);
	
	POMDOG_ASSERT(primitiveAxes);
	primitiveAxes->Draw(*graphicsContext, vierMatrix3D * projectionMatrix3D);
	
	//auto viewMatrix2D = CreateViewMatrix2D(nodeCamera->Transform(), *camera);
	
	POMDOG_ASSERT(spriteRenderer);
	spriteRenderer->Begin(vierMatrix3D);
		
	auto rootNode = gameWorld.Component<Node2D>(rootObjectID);
	auto worldMatrix = Matrix4x4::CreateTranslation(Vector3(rootNode->Transform().Position, 0.0f));
	{
		for (auto child: rootNode->Children())
		{
			auto canvasItem = child->Component<CanvasItem>();
			if (!child || !canvasItem->Visibile) {
				continue;
			}
		
			auto node = child->Component<Node2D>();

			auto sprite = child->Component<Sprite>();
			auto & transform = node->Transform();
			
			constexpr float layerDepth = 0.0f;

			spriteRenderer->Draw(texture, worldMatrix, transform.Position, sprite->Subrect,
				Color::White, transform.Rotation, sprite->Origin, transform.Scale, layerDepth);
		}
	}
	
	spriteRenderer->End();
}
//-----------------------------------------------------------------------
void TestAppGame::Draw()
{
	constexpr bool enableFxaa = true;

	if (enableFxaa) {
		graphicsContext->SetRenderTarget(renderTarget);
	}
	
	graphicsContext->Clear(Color::CornflowerBlue);
	
	graphicsContext->SetSamplerState(0, samplerPoint);
	DrawSprites();
	
	if (enableFxaa) {
		fxaa->Draw(*graphicsContext, renderTarget);
	}
	
	graphicsContext->Present();
}
//-----------------------------------------------------------------------
}// namespace TestApp
