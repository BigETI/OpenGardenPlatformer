#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class MarmotEntityScript : public EntityScript {
	public:

		OGP_API MarmotEntityScript(Klein::SceneManagement::Node* node);

	protected:

		OGP_API virtual void OnGameTick(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;
	};
}
