#pragma once

#include <cstddef>
#include <memory>
#include <chrono>

#include "../../Environment/GardenEntityData.hpp"
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
		OGP_API void Kill();
		OGP_API void Win();

		OGP_API virtual void Spawn(const OGP::Environment::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;
		
	protected:

		OGP_API virtual void OnGameTick(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		bool isAlive;
		bool hasNotWonYet;
		OGP::Math::Vector2<std::size_t> targetPosition;
		float movementProgress;

		// TODO: Separate to an input handling script
		bool isWalkingLeft;
		bool isWalkingRight;
		bool isWalkingUp;
		bool isWalkingDown;
		bool isDiggingLeft;
		bool isDiggingRight;
	};
}
