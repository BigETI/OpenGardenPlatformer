#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../Math/Vector2.hpp"
#include "../../Rendering/Color.hpp"
#include "../../Rendering/RenderingContextElement.hpp"
#include "../../ResourceManagement/ResourceID.hpp"
#include "../../SceneManagement/Node.hpp"
#include "../Script.hpp"

namespace OGP::Scripting::Rendering {
	class SpriteRendererScript : public Script {
	public:

		OGP_API SpriteRendererScript(OGP::SceneManagement::Node* node);

		OGP_API const std::shared_ptr<OGP::Rendering::RenderingContextElement>& GetRenderingContextElement() const noexcept;
		OGP_API std::shared_ptr<OGP::Rendering::RenderingContextElement>& GetRenderingContextElement() noexcept;
		OGP_API const OGP::ResourceManagement::ResourceID& GetResourceID() const noexcept;
		OGP_API OGP::ResourceManagement::ResourceID& GetResourcePath(OGP::ResourceManagement::ResourceID& result) const;
		OGP_API void SetResourceID(const OGP::ResourceManagement::ResourceID& resourceID);
		OGP_API void SetResourceID(OGP::ResourceManagement::ResourceID&& resourceID) noexcept;
		OGP_API const OGP::Math::Rectangle<float>& GetSourceRectangle() const noexcept;
		OGP_API void SetSourceRectangle(const OGP::Math::Rectangle<float>& sourceRectangle) noexcept;
		OGP_API const OGP::Math::Vector2<float>& GetPivot() const noexcept;
		OGP_API void SetPivot(const OGP::Math::Vector2<float>& pivot) noexcept;
		OGP_API const OGP::Rendering::Color<std::uint8_t>& GetColor() const noexcept;
		OGP_API void SetColor(const OGP::Rendering::Color<std::uint8_t>& color) noexcept;
		OGP_API unsigned int GetLayerIndex() const noexcept;
		OGP_API void SetLayerIndex(unsigned int layerIndex) noexcept;

	protected:

		OGP_API virtual void OnFrameRender(Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;
	
	private:

		std::shared_ptr<OGP::Rendering::RenderingContextElement> renderingContextElement;
	};
}
