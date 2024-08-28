#pragma once

#include <chrono>
#include <memory>

#include "../../Entities/ELiftMovementState.hpp"
#include "../../Entities/GardenEntityData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../SceneManagement/Node.hpp"
#include "../Environment/GardenScript.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class LiftEntityScript : public EntityScript {
	public:

		OGP_API LiftEntityScript(OGP::SceneManagement::Node* node);

		OGP_API virtual OGP::Math::Vector2<float> GetToBeRenderedPosition() const noexcept override;
		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;
		OGP_API virtual bool Interact(EntityScript& sourceEntity) override;

	protected:

		OGP_API virtual void OnGameTick(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		OGP::Entities::ELiftMovementState liftMovementState;
		float movementProgress;
		float parkingProgress;

		bool IsAtTopBound(const OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool IsAtBottomBound(const OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool IsAtLeftBound(const OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool IsAtRightBound(const OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool SwitchToMovingUp(const OGP::Scripting::Environment::GardenScript& garden) noexcept;
		bool SwitchToMovingDown(const OGP::Scripting::Environment::GardenScript& garden) noexcept;
		bool SwitchToMovingLeft(const OGP::Scripting::Environment::GardenScript& garden) noexcept;
		bool SwitchToMovingRight(const OGP::Scripting::Environment::GardenScript& garden) noexcept;
		void SwitchToMoving() noexcept;
		void ParkFromMovingUp() noexcept;
		void ParkFromMovingDown() noexcept;
		void ParkFromMovingLeft() noexcept;
		void ParkFromMovingRight() noexcept;
		void Park() noexcept;
	};
}
