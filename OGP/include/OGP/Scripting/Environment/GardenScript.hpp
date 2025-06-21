#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include <Klein/Collections/ResizableGrid.hpp>
#include <Klein/Engine.hpp>
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

		Klein::EventSystem::Event<std::size_t> OnGameTimeUpdated;
		Klein::EventSystem::Event<> OnCompletionDisabled;
		Klein::EventSystem::Event<> OnCompletionEnabled;
		Klein::EventSystem::Event<> OnCompleted;
		Klein::EventSystem::Event<> OnFailed;

		OGP_API GardenScript(Klein::SceneManagement::Node* node);

		OGP_API const std::string& GetGardenName() const noexcept;
		OGP_API std::string& GetGardenName(std::string& result) const;
		OGP_API std::size_t GetMaximalTimeInGameSeconds() const noexcept;
		OGP_API std::size_t GetElapsedTimeInGameSeconds() const noexcept;
		OGP_API const Klein::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>>& GetGardenCells() const noexcept;
		OGP_API Klein::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>>& GetGardenCells() noexcept;
		OGP_API const std::vector<std::weak_ptr<OGP::Scripting::Entities::EntityScript>>& GetEntities() const noexcept;
		OGP_API const std::chrono::high_resolution_clock::duration& GetMaximalGameTickCount() const noexcept;
		OGP_API const std::chrono::high_resolution_clock::duration& GetElapsedGameTickCount() const noexcept;
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

		template <typename TEntityScript = OGP::Scripting::Entities::EntityScript>
		constexpr inline bool TryGettingEntity(const TEntityScript& entity, std::shared_ptr<TEntityScript>& result) const noexcept {
			bool ret(false);
			for (const auto& target_entity : entities) {
				if (std::shared_ptr<TEntityScript> current_target_entity = std::dynamic_pointer_cast<TEntityScript>(target_entity.lock())) {
					if (current_target_entity.get() == &entity) {
						result = current_target_entity;
						ret = true;
						break;
					}
				}
			}
			return ret;
		}

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

		OGP_API virtual void OnGameTick(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		std::string gardenName;
		std::size_t maximalTimeInGameSeconds;
		std::size_t elapsedTimeInGameSeconds;
		Klein::Collections::ResizableGrid<std::weak_ptr<OGP::Scripting::Cells::CellScript>> gardenCells;
		std::vector<std::weak_ptr<OGP::Scripting::Entities::EntityScript>> entities;
		std::size_t harvestableCount;
		std::chrono::high_resolution_clock::duration maximalGameTickCount;
		std::chrono::high_resolution_clock::duration elapsedGameTickCount;
		OGP::Environment::EGardenState gardenState;
	};
}
