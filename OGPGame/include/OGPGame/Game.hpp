#pragma once

#include <filesystem>
#include <memory>
#include <span>
#include <string>

#include <Klein/Raylib/RaylibEngine.hpp>

#include "SceneManagement/GameSceneLoader.hpp"

namespace OGPGame {
	class Game {
	public:

		Game() = delete;
		Game(const Game&) = delete;
		Game(Game&&) noexcept = delete;

		static int Start(const std::span<const std::string>& commandLineArguments);
		static void ShowIntroductionScene();
		static void ShowMainMenuScene();
		static void PlayGardenCollection(const std::filesystem::path& gardenCOllectionPath);

		Game& operator =(const Game&) = delete;
		Game& operator =(Game&&) noexcept = delete;

	private:

		static std::unique_ptr<Klein::Raylib::RaylibEngine> engine;
		static std::weak_ptr<OGPGame::SceneManagement::GameSceneLoader> gameSceneLoader;
		static std::weak_ptr<Klein::SceneManagement::Node> currentSceneNode;

		static void UnloadCurrentSceneNode();
	};
}
