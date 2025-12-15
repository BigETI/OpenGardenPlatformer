#pragma once

#include <filesystem>
#include <memory>
#ifndef IS_OGP_CXX_STD_17
#	include <span>
#endif
#include <string>

#ifdef IS_OGP_CXX_STD_17
#	include <tcb/span.hpp>
#endif

#include <Klein/Raylib/RaylibEngine.hpp>

#include "SceneManagement/GameSceneLoader.hpp"

namespace OGPGame {
	class Game {
	public:

		Game() = delete;
		Game(const Game&) = delete;
		Game(Game&&) noexcept = delete;
#ifdef IS_OGP_CXX_STD_17
		static int Start(const tcb::span<const std::string>& commandLineArguments);
#else
		static int Start(const std::span<const std::string>& commandLineArguments);
#endif
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
