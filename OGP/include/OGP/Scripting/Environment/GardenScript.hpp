#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "../../Collections/ResizableGrid.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Environment/GardenData.hpp"
#include "../../EventSystem/Event.hpp"
#include "../Cells/CellScript.hpp"
#include "../Entities/EntityScript.hpp"
#include "../Script.hpp"

namespace OGP::Scripting::Cells {
	class CellScript;
}

namespace OGP::Scripting::Entities {
	class EntityScript;
}

namespace OGP::Scripting::Environment {
	class GardenScript : public OGP::Scripting::Script {
	public:

		OGP::EventSystem::Event<> OnCompleted;
		OGP::EventSystem::Event<> OnFailed;

		OGP_API GardenScript(OGP::SceneManagement::Node* node);

		OGP_API const std::string& GetGardenName() const noexcept;
		OGP_API std::string& GetGardenName(std::string& result) const;
		OGP_API std::size_t GetTimeInGameSeconds() const noexcept;
		OGP_API std::size_t GetScore() const noexcept;
		OGP_API void SetScore(std::size_t score) noexcept;
		OGP_API void AddScore(std::size_t score) noexcept;
		OGP_API const OGP::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>>& GetGardenCells() const noexcept;
		OGP_API OGP::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>>& GetGardenCells() noexcept;
		OGP_API const std::vector<std::weak_ptr<OGP::Scripting::Entities::EntityScript>>& GetEntities() const noexcept;
		OGP_API bool RemoveEntity(std::shared_ptr<OGP::Scripting::Entities::EntityScript> entity) noexcept;
		OGP_API std::size_t GetHarvestableCount() const noexcept;
		OGP_API void IncrementHarvestableCount() noexcept;
		OGP_API void DecrementHarvestableCount() noexcept;
		std::size_t GetRedKeyCount() const noexcept;
		std::size_t GetYellowKeyCount() const noexcept;
		std::size_t GetGreenKeyCount() const noexcept;
		OGP_API void AddRedKey() noexcept;
		OGP_API bool UseRedKey() noexcept;
		OGP_API void AddYellowKey() noexcept;
		OGP_API bool UseYellowKey() noexcept;
		OGP_API void AddGreenKey() noexcept;
		OGP_API bool UseGreenKey() noexcept;
		bool IsCompletionEnabled() const noexcept;
		OGP_API void LoadGardenFromGardenData(const OGP::Environment::GardenData& gardenData);
		OGP_API std::shared_ptr<OGP::Scripting::Cells::CellScript> GetCellAt(const OGP::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API std::vector<std::shared_ptr<OGP::Scripting::Entities::EntityScript>>& GetEntitiesAt(const OGP::Math::Vector2<std::size_t>& position, std::vector<std::shared_ptr<OGP::Scripting::Entities::EntityScript>>& result) const noexcept;
		OGP_API bool IsSolidAt(const OGP::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsWalkableAt(const OGP::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsClimbableAt(const OGP::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsClimbingUpAllowedAt(const OGP::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsDeadlyAt(const OGP::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsTopDeadlyAt(const OGP::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsWinnableAt(const OGP::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool DigAt(const OGP::Math::Vector2<std::size_t>& position) noexcept;
		OGP_API bool InteractAt(const OGP::Math::Vector2<std::size_t>& position, OGP::Scripting::Entities::EntityScript* sourceEntity) noexcept;

	private:

		std::string gardenName;
		std::size_t timeInGameSeconds;
		std::size_t score;
		OGP::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>> gardenCells;
		std::vector<std::weak_ptr<OGP::Scripting::Entities::EntityScript>> entities;
		std::size_t harvestableCount;
		std::size_t redKeyCount;
		std::size_t yellowKeyCount;
		std::size_t greenKeyCount;
	};
}
