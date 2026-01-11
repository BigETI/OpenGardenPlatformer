#pragma once

#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/EventSystem/Event.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Physics/AABBColliderScript.hpp>

#include "../../Entities/GardenEntityData.hpp"
#include "../../Entities/HumanoidInput.hpp"
#include "../../Environment/EKillerType.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../Environment/GardenScript.hpp"
#include "HumanoidEntityScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenScript;
}

namespace OGP::Scripting::Entities {
	class PlayerEntityScript : public HumanoidEntityScript {
	public:

		Klein::EventSystem::Event<> OnDied;
		Klein::EventSystem::Event<> OnWon;
		Klein::EventSystem::Event<std::size_t> OnScoreChanged;
		Klein::EventSystem::Event<> OnRedKeyCollected;
		Klein::EventSystem::Event<> OnRedKeyUsed;
		Klein::EventSystem::Event<> OnYellowKeyCollected;
		Klein::EventSystem::Event<> OnYellowKeyUsed;
		Klein::EventSystem::Event<> OnGreenKeyCollected;
		Klein::EventSystem::Event<> OnGreenKeyUsed;
		Klein::EventSystem::Event<> OnGarlicEffectActivated;
		Klein::EventSystem::Event<> OnGarlicEffectDeactivated;
		Klein::EventSystem::Event<> OnMushroomEffectActivated;
		Klein::EventSystem::Event<> OnMushroomEffectDeactivated;

		OGP_API PlayerEntityScript(Klein::SceneManagement::Node* node);

		OGP_API bool IsAlive() const noexcept override;
		OGP_API bool Kill(OGP::Environment::EKillerType killerType) override;
		OGP_API bool Win() override;
		OGP_API std::size_t GetScore() const noexcept;
		OGP_API void SetScore(std::size_t score) noexcept;
		OGP_API void AddScore(std::size_t score) noexcept;
		OGP_API std::size_t GetRedKeyCount() const noexcept;
		OGP_API std::size_t GetYellowKeyCount() const noexcept;
		OGP_API std::size_t GetGreenKeyCount() const noexcept;
		OGP_API void AddRedKey() noexcept;
		OGP_API bool UseRedKey() noexcept;
		OGP_API void AddYellowKey() noexcept;
		OGP_API bool UseYellowKey() noexcept;
		OGP_API void AddGreenKey() noexcept;
		OGP_API bool UseGreenKey() noexcept;
		OGP_API const std::chrono::high_resolution_clock::duration& GetRemainingGarlicEffectTime() const noexcept;
		OGP_API const std::chrono::high_resolution_clock::duration& GetRemainingMushroomEffectTime() const noexcept;
		OGP_API bool IsGarlicEffectActive() const noexcept;
		OGP_API void ActivateGarlicEffect() noexcept;
		OGP_API bool IsMushroomEffectActive() const noexcept;
		OGP_API void ActivateMushroomEffect() noexcept;
		OGP_API void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;
		
	protected:

		OGP_API OGP::Entities::HumanoidInput GetInput(const Klein::Engine& engine) const noexcept override;
		OGP_API void OnGameTick(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		bool isAlive;
		bool hasNotWonYet;
		std::size_t score;
		std::size_t redKeyCount;
		std::size_t yellowKeyCount;
		std::size_t greenKeyCount;
		std::chrono::high_resolution_clock::duration remainingGarlicEffectTime;
		std::chrono::high_resolution_clock::duration remainingMushroomEffectTime;
		std::weak_ptr<Klein::Scripting::Physics::AABBColliderScript> collider;
		OGP::Entities::HumanoidInput keyboardInput;
		OGP::Entities::HumanoidInput analogGamepadInput;
		OGP::Entities::HumanoidInput digitalGamepadInput;
		bool isInQuestionMark;
		Klein::Math::Vector2<std::size_t> lastQuestionMarkPosition;
	};
}
