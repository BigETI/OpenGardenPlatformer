#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "../Exportables/Exportable.hxx"
#include "../Math/Rectangle.hpp"
#include "../Math/Vector2.hpp"
#include "../Rendering/Color.hpp"
#include "../ResourceManagement/ResourceID.hpp"

namespace OGP::Rendering {
	class RenderingContextElement {
	public:

		OGP_API RenderingContextElement();
		OGP_API RenderingContextElement(
			const OGP::ResourceManagement::ResourceID& resourceID,
			const OGP::Math::Rectangle<float>& sourceRectangle,
			const OGP::Math::Vector2<float>& position,
			float rotation,
			const OGP::Math::Vector2<float>& size,
			const OGP::Math::Vector2<float>& pivot,
			const OGP::Rendering::Color<std::uint8_t>& color,
			unsigned int layerIndex
		);
		OGP_API RenderingContextElement(
			OGP::ResourceManagement::ResourceID&& resourceID,
			OGP::Math::Rectangle<float>&& sourceRectangle,
			OGP::Math::Vector2<float>&& position,
			float rotation,
			OGP::Math::Vector2<float>&& size,
			OGP::Math::Vector2<float>&& pivot,
			OGP::Rendering::Color<std::uint8_t>&& color,
			unsigned int layerIndex
		) noexcept;
		OGP_API RenderingContextElement(const RenderingContextElement& renderingContextElement);
		OGP_API RenderingContextElement(RenderingContextElement&& renderingContextElement) noexcept;

		OGP_API const OGP::ResourceManagement::ResourceID& GetResourceID() const noexcept;
		OGP_API OGP::ResourceManagement::ResourceID& GetResourceID(OGP::ResourceManagement::ResourceID& result) const;
		OGP_API void SetResourceID(const OGP::ResourceManagement::ResourceID& resourceID);
		OGP_API void SetResourceID(OGP::ResourceManagement::ResourceID&& resourceID) noexcept;
		OGP_API const OGP::Math::Rectangle<float>& GetSourceRectangle() const noexcept;
		OGP_API void SetSourceRectangle(const OGP::Math::Rectangle<float>& sourceRectangle) noexcept;
		OGP_API const OGP::Math::Vector2<float>& GetPosition() const noexcept;
		OGP_API void SetPosition(const OGP::Math::Vector2<float>& position) noexcept;
		OGP_API float GetRotation() const noexcept;
		OGP_API void SetRotation(float rotation) noexcept;
		OGP_API const OGP::Math::Vector2<float>& GetSize() const noexcept;
		OGP_API void SetSize(const OGP::Math::Vector2<float>& size) noexcept;
		OGP_API const OGP::Math::Vector2<float>& GetPivot() const noexcept;
		OGP_API void SetPivot(const OGP::Math::Vector2<float>& pivot) noexcept;
		OGP_API const OGP::Rendering::Color<std::uint8_t>& GetColor() const noexcept;
		OGP_API void SetColor(const OGP::Rendering::Color<std::uint8_t>& color) noexcept;
		OGP_API unsigned int GetLayerIndex() const noexcept;
		OGP_API void SetLayerIndex(unsigned int layerIndex) noexcept;

		OGP_API RenderingContextElement& operator =(const RenderingContextElement& renderingContextElement);
		OGP_API RenderingContextElement& operator =(RenderingContextElement&& renderingContextElement) noexcept;
		OGP_API bool operator ==(const RenderingContextElement& renderingContextElement) const noexcept;
		OGP_API bool operator <(const RenderingContextElement& renderingContextElement) const noexcept;
		OGP_API bool operator <=(const RenderingContextElement& renderingContextElement) const noexcept;
		OGP_API bool operator >(const RenderingContextElement& renderingContextElement) const noexcept;
		OGP_API bool operator >=(const RenderingContextElement& renderingContextElement) const noexcept;

	private:

		OGP::ResourceManagement::ResourceID resourceID;
		OGP::Math::Rectangle<float> sourceRectangle;
		OGP::Math::Vector2<float> position;
		float rotation;
		OGP::Math::Vector2<float> size;
		OGP::Math::Vector2<float> pivot;
		OGP::Rendering::Color<std::uint8_t> color;
		unsigned int layerIndex;
	};
}

template <>
struct std::less<std::shared_ptr<OGP::Rendering::RenderingContextElement>> {
	constexpr bool operator ()(
		const std::shared_ptr<OGP::Rendering::RenderingContextElement>& lhs,
		const std::shared_ptr<OGP::Rendering::RenderingContextElement>& rhs) const noexcept {
		return rhs && (!lhs || (*lhs < *rhs));
	}
};
