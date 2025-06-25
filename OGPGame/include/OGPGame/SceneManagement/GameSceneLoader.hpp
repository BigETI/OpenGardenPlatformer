#pragma once

#include <filesystem>
#include <span>
#include <string>

#include <Klein/SceneManagement/ISceneLoader.hpp>
#include <Klein/SceneManagement/Node.hpp>

namespace OGPGame::SceneManagement {
	class GameSceneLoader : public Klein::SceneManagement::ISceneLoader {
	public:

		GameSceneLoader() = delete;

		GameSceneLoader(const std::filesystem::path& gardenCollectionPath);

		const std::filesystem::path& GetGardenCollectionPath() const noexcept;
		void SetGardenCollectionPath(const std::filesystem::path& gardenCollectionPath);
		void Load(Klein::SceneManagement::Node& rootNode) override;

	private:

		std::filesystem::path gardenCollectionPath;
	};
}
