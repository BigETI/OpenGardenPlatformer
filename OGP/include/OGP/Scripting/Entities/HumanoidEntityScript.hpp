#pragma once

#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/EventSystem/Event.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Entities/EHumanoidMovementState.hpp"
#include "../../Entities/GardenEntityData.hpp"
#include "../../Entities/HumanoidInput.hpp"
#include "../../Environment/EKillerType.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../Environment/GardenScript.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenScript;
}

namespace OGP::Scripting::Entities {
	class HumanoidEntityScript : public EntityScript {
	public:

		Klein::EventSystem::Event<> OnStandingStarted;
		Klein::EventSystem::Event<> OnStandingFinished;
		Klein::EventSystem::Event<> OnFallingStarted;
		Klein::EventSystem::Event<> OnFallingFinished;
		Klein::EventSystem::Event<> OnWalkingStarted;
		Klein::EventSystem::Event<> OnWalkingFinished;
		Klein::EventSystem::Event<> OnClimbingStarted;
		Klein::EventSystem::Event<> OnClimbingFinished;
		Klein::EventSystem::Event<> OnMounted;
		Klein::EventSystem::Event<> OnDismounted;

		OGP_API HumanoidEntityScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsAlive() const noexcept;
		OGP_API virtual float GetMaximalMovementSpeed() const noexcept;
		OGP_API virtual bool Kill(OGP::Environment::EKillerType killerType);
		OGP_API virtual bool Win();
		OGP_API virtual Klein::Math::Vector2<float> GetToBeRenderedPosition() const noexcept override;
		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;

	protected:

		OGP_API virtual OGP::Entities::HumanoidInput GetInput(const Klein::Engine& engine) const noexcept;
		OGP_API virtual void OnGameTick(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		OGP::Entities::HumanoidInput input;
		Klein::Math::Vector2<std::size_t> targetPosition;
		float movementProgress;
		Klein::Math::Vector2<float> toBeRenderedAtOffset;
		OGP::Entities::EHumanoidMovementState movementState;

		void UpdateMovementState(OGP::Entities::EHumanoidMovementState movementState);
	};
}
