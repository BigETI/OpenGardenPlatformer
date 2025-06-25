#pragma once

#include <Klein/SceneManagement/ISceneLoader.hpp>
#include <Klein/SceneManagement/Node.hpp>

namespace OGPGame::SceneManagement {
	class MainMenuSceneLoader : public Klein::SceneManagement::ISceneLoader {
	public:

		void Load(Klein::SceneManagement::Node& rootNode) override;
	};
}
