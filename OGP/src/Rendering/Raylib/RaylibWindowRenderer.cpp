#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

#include <raylib.h>

#include <OGP/Math/Rectangle.hpp>
#include <OGP/Math/Vector2.hpp>
#include <OGP/Rendering/Color.hpp>
#include <OGP/Rendering/Raylib/RaylibWindowRenderer.hpp>
#include <OGP/Rendering/RenderingContext.hpp>

using namespace OGP::Math;
using namespace OGP::Rendering::Raylib;
using namespace std;
using namespace std::chrono;

RaylibWindowRenderer::RaylibWindowRenderer(const string& title, unsigned int width, unsigned int height) : title(title), width(width), height(height) {
	if (IsWindowReady()) {
		throw exception("raylib window has been already initialized.");
	}
	InitWindow(static_cast<int>(width), static_cast<int>(height), title.c_str());
	raylibTexture2DResourceManager.Initialize();
}

RaylibWindowRenderer::~RaylibWindowRenderer() {
	raylibTexture2DResourceManager.Deinitialize();
	CloseWindow();
}

const string& RaylibWindowRenderer::GetTitle() const noexcept {
	return title;
}

string& RaylibWindowRenderer::GetTitle(string& result) const {
	return result = title;
}

const unsigned int RaylibWindowRenderer::GetWidth() const noexcept {
	return width;
}

const unsigned int RaylibWindowRenderer::GetHeight() const noexcept {
	return height;
}

void RaylibWindowRenderer::Render(const RenderingContext& renderingContext, high_resolution_clock::duration deltaTime) {
	if (IsWindowReady()) {
		OGP::Math::Vector2<float> camera_position(renderingContext.GetCameraPosition());
		camera.target = { camera_position.x, -camera_position.y };
		camera.offset = { GetRenderWidth() * 0.5f, GetRenderHeight() * 0.5f };
		camera.rotation = renderingContext.GetCameraRotation();
		camera.zoom = renderingContext.GetCameraZoom();
		BeginDrawing();
		ClearBackground({ 0x00, 0x00, 0x00, 0x00 });
		BeginMode2D(camera);
		Texture2D texture_2d({ 0 });
		for (const auto& rendering_context_element : renderingContext) {
			if (
				raylibTexture2DResourceManager.TryGettingResource(
					rendering_context_element->GetResourceID(),
					texture_2d
				)
			) {
				OGP::Math::Rectangle<float> sourceRectangle(rendering_context_element->GetSourceRectangle());
				OGP::Math::Vector2<float> position(rendering_context_element->GetPosition());
				OGP::Math::Vector2<float> size(rendering_context_element->GetSize());
				OGP::Math::Vector2<float> origin(rendering_context_element->GetPivot() * size);
				OGP::Rendering::Color<uint8_t> color(rendering_context_element->GetColor());
				DrawTexturePro(
					texture_2d,
					{
						sourceRectangle.position.x * static_cast<float>(texture_2d.width),
						sourceRectangle.position.y * static_cast<float>(texture_2d.height),
						sourceRectangle.size.x * static_cast<float>(texture_2d.width),
						sourceRectangle.size.y * static_cast<float>(texture_2d.height)
					},
					{ position.x, -position.y, size.x, size.y },
					{ origin.x, origin.y },
					-rendering_context_element->GetRotation(),
					{
						static_cast<unsigned char>(color.red),
						static_cast<unsigned char>(color.green),
						static_cast<unsigned char>(color.blue),
						static_cast<unsigned char>(color.alpha)
					}
				);
			}
		}
		EndMode2D();
		EndDrawing();
	}

	// TODO: Cleanup resource manager of unused resources
}
