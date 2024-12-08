#pragma once

#include <cstddef>
#include <memory>
#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/EventSystem/Event.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include "../../Entities/GardenEntityData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../Environment/GardenScript.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenScript;
}

namespace OGP::Scripting::Entities {
	class PlayerEntityScript : public EntityScript {
	public:

		Klein::EventSystem::Event<> OnDied;
		Klein::EventSystem::Event<> OnWon;

		OGP_API PlayerEntityScript(Klein::SceneManagement::Node* node);

		OGP_API bool IsAlive() const noexcept;
		OGP_API bool Kill();
		OGP_API bool Win();

		OGP_API virtual Klein::Math::Vector2<float> GetToBeRenderedPosition() const noexcept override;
		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;
		
	protected:

		OGP_API virtual void OnGameTick(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		bool isAlive;
		bool hasNotWonYet;
		Klein::Math::Vector2<std::size_t> targetPosition;
		float movementProgress;
		Klein::Math::Vector2<float> toBeRenderedAtOffset;

		// TODO: Separate to an input handling script
		bool isWalkingLeft;
		bool isWalkingRight;
		bool isWalkingUp;
		bool isWalkingDown;
		bool isDiggingLeft;
		bool isDiggingRight;
	};
}
