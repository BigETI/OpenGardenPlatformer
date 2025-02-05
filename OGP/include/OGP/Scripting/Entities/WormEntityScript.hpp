#pragma once

#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Entities/EWormMovementState.hpp"
#include "../../Entities/GardenEntityData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../Environment/GardenScript.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class WormEntityScript : public EntityScript {
	public:

		OGP_API WormEntityScript(Klein::SceneManagement::Node* node);

		OGP_API virtual Klein::Math::Vector2<float> GetToBeRenderedPosition() const noexcept override;
		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;
		OGP_API virtual bool Interact(EntityScript& sourceEntity) override;

	protected:

		OGP_API virtual void OnGameTick(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		OGP::Entities::EWormMovementState wormMovementState;
		float movementProgress;
		bool hasStartedToMove;
		bool isFinishingToMove;

		bool IsLeftMovable(OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool IsRightMovable(OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool IsAtLeftBound(const OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool IsAtLeftBound(const OGP::Scripting::Environment::GardenScript& garden, std::size_t targetXPosition) const noexcept;
		bool IsAtRightBound(const OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool IsAtRightBound(const OGP::Scripting::Environment::GardenScript& garden, std::size_t targetXPosition) const noexcept;
		bool IsMovingLeftReachesEnd(const OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		bool IsMovingRightReachesEnd(const OGP::Scripting::Environment::GardenScript& garden) const noexcept;
		float GetAnimatedMovementProgress() const noexcept;
	};
}
