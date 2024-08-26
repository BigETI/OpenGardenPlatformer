#pragma once

#include <chrono>

#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../SceneManagement/Node.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class LiftEntityScript : public EntityScript {
	public:

		OGP_API LiftEntityScript(OGP::SceneManagement::Node* node);

	protected:

		OGP_API virtual void OnGameTick(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;
	};
}
