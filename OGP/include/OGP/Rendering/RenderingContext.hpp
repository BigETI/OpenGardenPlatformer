#pragma once

#include <memory>
#include <queue>

#include "../Exportables/Exportable.hxx"
#include "../Math/Vector2.hpp"
#include "RenderingContextElement.hpp"

namespace OGP::Rendering {
	class RenderingContext {
	public:

		using ConstantIterator = std::vector<std::shared_ptr<RenderingContextElement>>::const_iterator;
		using Iterator = std::vector<std::shared_ptr<RenderingContextElement>>::iterator;
		using const_iterator = ConstantIterator;
		using iterator = Iterator;

		OGP_API RenderingContext();
		OGP_API RenderingContext(const RenderingContext& renderingContext);
		OGP_API RenderingContext(RenderingContext&& renderingContext) noexcept;

		OGP_API const OGP::Math::Vector2<float>& GetCameraPosition() const noexcept;
		OGP_API void SetCameraPosition(const OGP::Math::Vector2<float>& cameraPosition) noexcept;
		OGP_API float GetCameraRotation() const noexcept;
		OGP_API void SetCameraRotation(float cameraRotation) noexcept;
		OGP_API float GetCameraZoom() const noexcept;
		OGP_API void SetCameraZoom(float cameraZoom) noexcept;
		OGP_API const std::priority_queue<std::shared_ptr<RenderingContextElement>>& GetStagedElements() const noexcept;
		OGP_API std::priority_queue<std::shared_ptr<RenderingContextElement>>& GetStagedElements(
			std::priority_queue<std::shared_ptr<RenderingContextElement>>& result
		) const;
		OGP_API const std::vector<std::shared_ptr<RenderingContextElement>>& GetCommitedElements() const noexcept;
		OGP_API std::vector<std::shared_ptr<RenderingContextElement>>& GetCommitedElements(
			std::vector<std::shared_ptr<RenderingContextElement>>& result
		) const;
		OGP_API void StageElement(const std::shared_ptr<RenderingContextElement>& renderingContextElement);
		OGP_API void StageElement(std::shared_ptr<RenderingContextElement>&& renderingContextElement);
		OGP_API bool TryUnstagingElement(std::shared_ptr<RenderingContextElement>& result) noexcept;
		OGP_API void CommitElements();
		OGP_API void Clear();

		OGP_API const_iterator begin() const;
		OGP_API iterator begin();
		OGP_API const_iterator end() const;
		OGP_API iterator end();

		OGP_API RenderingContext& operator =(const RenderingContext& renderingContext);
		OGP_API RenderingContext& operator =(RenderingContext&& renderingContext) noexcept;

	private:

		OGP::Math::Vector2<float> cameraPosition;
		float cameraRotation;
		float cameraZoom;
		std::priority_queue<std::shared_ptr<RenderingContextElement>> stagedElements;
		std::vector<std::shared_ptr<RenderingContextElement>> commitedElements;
	};
}
