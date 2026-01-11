#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <Klein/Engine.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Cells/EGardenCellType.hpp"
#include "../../Environment/ESideFlags.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../Entities/EntityScript.hpp"
#include "../Environment/GardenScript.hpp"

namespace OGP::Scripting::Entities {
	class EntityScript;
}

namespace OGP::Scripting::Environment {
	class GardenScript;
}

namespace OGP::Scripting::Cells {
	class CellScript : public Klein::Scripting::Script {
	public:

		OGP_API CellScript(Klein::SceneManagement::Node* node);

		OGP_API std::weak_ptr<OGP::Scripting::Environment::GardenScript> GetGarden() const noexcept;
		OGP_API OGP::Cells::EGardenCellType GetGardenCellType() const noexcept;
		OGP_API void UpdateProperties(OGP::Cells::EGardenCellType gardenCellType, const Klein::Math::Vector2<std::size_t>& position, std::weak_ptr<OGP::Scripting::Environment::GardenScript> garden);
		OGP_API const std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript>& GetBackgroundSpriteRenderer() const noexcept;
		OGP_API const std::vector<std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript>>& GetForegroundSpriteRenderers() const noexcept;
		OGP_API void UpdateVisuals();
		OGP_API virtual bool IsSolid() const noexcept;
		OGP_API virtual bool IsTopWalkable() const noexcept;
		OGP_API virtual bool IsClimbable() const noexcept;
		OGP_API virtual bool IsClimbingUpAllowed() const noexcept;
		OGP_API virtual bool IsDeadly() const noexcept;
		OGP_API virtual bool IsTopDeadly() const noexcept;
		OGP_API virtual OGP::Environment::ESideFlags GetConnectableSideFlags() const noexcept;
		OGP_API virtual bool Dig() noexcept;
		OGP_API virtual bool Interact(OGP::Scripting::Entities::EntityScript& sourceEntity) noexcept;

	protected:

		OGP_API virtual void OnFrameRender(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		OGP::Cells::EGardenCellType gardenCellType;
		Klein::Math::Vector2<std::size_t> position;
		std::weak_ptr<OGP::Scripting::Environment::GardenScript> garden;
		std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript> backgroundSpriteRenderer;
		std::vector<std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript>> foregroundSpriteRenderers;
		OGP::Environment::ESideFlags oldConnectableSideFlags;

		void UpdateVisuals(const std::shared_ptr<OGP::Scripting::Environment::GardenScript>& garden);
		std::shared_ptr<Klein::Scripting::Rendering::SpriteRendererScript> AppendForegroundSpriteRenderer();
	};
}
