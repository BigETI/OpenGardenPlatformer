#pragma once

#include <Klein/SceneManagement/ISceneLoader.hpp>

namespace OGP::SceneManagement {
	class BaseSceneLoader : public Klein::SceneManagement::ISceneLoader {

		void Load(Klein::SceneManagement::Node& rootNode) override;
	};
}
