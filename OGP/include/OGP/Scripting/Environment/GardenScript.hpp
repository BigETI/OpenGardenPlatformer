#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include <Klein/Collections/ResizableGrid.hpp>
#include <Klein/EventSystem/Event.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Exportables/Exportable.hxx"
#include "../../Environment/EGardenState.hpp"
#include "../../Environment/GardenData.hpp"
#include "../Cells/CellScript.hpp"
#include "../Entities/EntityScript.hpp"

namespace OGP::Scripting::Cells {
	class CellScript;
}

namespace OGP::Scripting::Entities {
	class EntityScript;
}

namespace OGP::Scripting::Environment {
	class GardenScript : public Klein::Scripting::Script {
	public:

		Klein::EventSystem::Event<> OnCompletionDisabled;
		Klein::EventSystem::Event<> OnCompletionEnabled;
		Klein::EventSystem::Event<> OnCompleted;
		Klein::EventSystem::Event<> OnFailed;

		OGP_API GardenScript(Klein::SceneManagement::Node* node);

		OGP_API const std::string& GetGardenName() const noexcept;
		OGP_API std::string& GetGardenName(std::string& result) const;
		OGP_API std::size_t GetTimeInGameSeconds() const noexcept;
		OGP_API const Klein::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>>& GetGardenCells() const noexcept;
		OGP_API Klein::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>>& GetGardenCells() noexcept;
		OGP_API const std::vector<std::weak_ptr<OGP::Scripting::Entities::EntityScript>>& GetEntities() const noexcept;
		OGP_API OGP::Environment::EGardenState GetGardenState() const noexcept;
		OGP_API void SetGardenState(OGP::Environment::EGardenState gardenState) noexcept;
		
		template <typename TEntityScript = OGP::Scripting::Entities::EntityScript>
		constexpr inline void EnumerateEntities(const std::function<void(const TEntityScript& entity)>& onEntityEnumerated) const noexcept {
			for (const auto& entity_ptr : entities) {
				if (std::shared_ptr<TEntityScript> entity = std::dynamic_pointer_cast<TEntityScript>(entity_ptr.lock())) {
					onEntityEnumerated(*(entity.get()));
				}
			}
		}
		
		OGP_API bool TryGettingEntity(const OGP::Scripting::Entities::EntityScript& entity, std::shared_ptr<OGP::Scripting::Entities::EntityScript>& result) const noexcept;
		OGP_API bool RemoveEntity(std::shared_ptr<OGP::Scripting::Entities::EntityScript> entity) noexcept;
		OGP_API std::size_t GetHarvestableCount() const noexcept;
		OGP_API void IncrementHarvestableCount() noexcept;
		OGP_API void DecrementHarvestableCount() noexcept;
		OGP_API bool IsCompletionEnabled() const noexcept;
		OGP_API void LoadGardenFromGardenData(const OGP::Environment::GardenData& gardenData);
		OGP_API std::shared_ptr<OGP::Scripting::Cells::CellScript> GetCellAt(const Klein::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API std::vector<std::shared_ptr<OGP::Scripting::Entities::EntityScript>>& GetEntitiesAt(const Klein::Math::Vector2<std::size_t>& position, std::vector<std::shared_ptr<OGP::Scripting::Entities::EntityScript>>& result) const noexcept;
		OGP_API bool IsSolidAt(const Klein::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsWalkableAt(const Klein::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsClimbableAt(const Klein::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsClimbingUpAllowedAt(const Klein::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsDeadlyAt(const Klein::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsTopDeadlyAt(const Klein::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool IsWinnableAt(const Klein::Math::Vector2<std::size_t>& position) const noexcept;
		OGP_API bool DigAt(const Klein::Math::Vector2<std::size_t>& position) noexcept;
		OGP_API bool InteractAt(const Klein::Math::Vector2<std::size_t>& position, OGP::Scripting::Entities::EntityScript& sourceEntity) noexcept;
		OGP_API void Unload() noexcept;

	private:

		std::string gardenName;
		std::size_t timeInGameSeconds;
		Klein::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>> gardenCells;
		std::vector<std::weak_ptr<OGP::Scripting::Entities::EntityScript>> entities;
		std::size_t harvestableCount;
		OGP::Environment::EGardenState gardenState;
	};
}
