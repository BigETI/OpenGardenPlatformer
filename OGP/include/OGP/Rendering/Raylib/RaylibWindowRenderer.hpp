#pragma once

#include <chrono>
#include <string>

#include <raylib.h>

#include "../../Exportables/Exportable.hxx"
#include "../../ResourceManagement/Raylib/RaylibTexture2DResourceManager.hpp"
#include "../IRenderer.hpp"
#include "../RenderingContext.hpp"

namespace OGP::Rendering::Raylib {
	class RaylibWindowRenderer : public OGP::Rendering::IRenderer {
	public:

		OGP_API RaylibWindowRenderer(const std::string& title, unsigned int width, unsigned int height);

		RaylibWindowRenderer(const RaylibWindowRenderer&) = delete;
		RaylibWindowRenderer(RaylibWindowRenderer&&) = delete;

		OGP_API ~RaylibWindowRenderer();

		OGP_API const std::string& GetTitle() const noexcept;
		OGP_API std::string& GetTitle(std::string& result) const;
		OGP_API const unsigned int GetWidth() const noexcept;
		OGP_API const unsigned int GetHeight() const noexcept;
		OGP_API void Render(const RenderingContext& renderingContext, std::chrono::high_resolution_clock::duration deltaTime) override;

		RaylibWindowRenderer& operator =(const RaylibWindowRenderer&) = delete;
		RaylibWindowRenderer& operator =(RaylibWindowRenderer&&) = delete;

	private:

		const std::string& title;
		unsigned int width;
		unsigned int height;
		Camera2D camera;
		OGP::ResourceManagement::Raylib::RaylibTexture2DResourceManager raylibTexture2DResourceManager;
	};
}
