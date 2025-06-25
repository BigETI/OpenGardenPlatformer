#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Script.hpp>

namespace OGPGame::Scripting {
	class DroppedGardenCollectionFileLoaderScript : public Klein::Scripting::Script {
	public:

		DroppedGardenCollectionFileLoaderScript(Klein::SceneManagement::Node* node);

	protected:

		void OnGameTick(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;
	};
}
