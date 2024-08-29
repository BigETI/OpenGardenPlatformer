#pragma once

#include <cstddef>
#include <memory>
#include <chrono>

#include "../../Entities/GardenEntityData.hpp"
#include "../../EventSystem/Event.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../Math/Vector2.hpp"
#include "../../SceneManagement/Node.hpp"
#include "../Environment/GardenScript.hpp"
#include "../Rendering/SpriteRendererScript.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenScript;
}

namespace OGP::Scripting::Entities {
	class PlayerEntityScript : public EntityScript {
	public:

		OGP::EventSystem::Event<> OnDied;
		OGP::EventSystem::Event<> OnWon;

		OGP_API PlayerEntityScript(OGP::SceneManagement::Node* node);

		OGP_API bool IsAlive() const noexcept;
		OGP_API bool Kill();
		OGP_API bool Win();

		OGP_API virtual OGP::Math::Vector2<float> GetToBeRenderedPosition() const noexcept override;
		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;
		
	protected:

		OGP_API virtual void OnGameTick(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		bool isAlive;
		bool hasNotWonYet;
		OGP::Math::Vector2<std::size_t> targetPosition;
		float movementProgress;
		OGP::Math::Vector2<float> toBeRenderedAtOffset;

		// TODO: Separate to an input handling script
		bool isWalkingLeft;
		bool isWalkingRight;
		bool isWalkingUp;
		bool isWalkingDown;
		bool isDiggingLeft;
		bool isDiggingRight;
	};
}
