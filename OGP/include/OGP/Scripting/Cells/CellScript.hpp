#pragma once

#include <memory>

#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Cells/EGardenCellType.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../Environment/GardenScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenScript;
}

namespace OGP::Scripting::Cells {
	class CellScript : public Klein::Scripting::Script {
	public:

		OGP_API CellScript(Klein::SceneManagement::Node* node);

		OGP_API std::weak_ptr<OGP::Scripting::Environment::GardenScript> GetGarden() const noexcept;
		OGP_API OGP::Cells::EGardenCellType GetGardenCellType() const noexcept;
		OGP_API void UpdateProperties(OGP::Cells::EGardenCellType gardenCellType, std::weak_ptr<OGP::Scripting::Environment::GardenScript> garden);
		OGP_API const std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript>& GetBackgroundSpriteRenderer() const noexcept;
		OGP_API const std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript>& GetForegroundSpriteRenderer() const noexcept;
		OGP_API virtual bool IsSolid() const noexcept;
		OGP_API virtual bool IsTopWalkable() const noexcept;
		OGP_API virtual bool IsClimbable() const noexcept;
		OGP_API virtual bool IsClimbingUpAllowed() const noexcept;
		OGP_API virtual bool IsDeadly() const noexcept;
		OGP_API virtual bool IsTopDeadly() const noexcept;
		OGP_API virtual bool Dig() noexcept;
		OGP_API virtual bool Interact() noexcept;

	private:

		std::weak_ptr<OGP::Scripting::Environment::GardenScript> garden;
		OGP::Cells::EGardenCellType gardenCellType;
		std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript> backgroundSpriteRenderer;
		std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript> foregroundSpriteRenderer;
	};
}
