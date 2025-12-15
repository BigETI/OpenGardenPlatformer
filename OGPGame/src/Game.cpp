#include <cstddef>
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
#include <Klein/ResourceManagement/FileSystem.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/SceneManagement/BaseSceneLoader.hpp>
#include <OGP/Scripting/Audio/MusicPlayerScript.hpp>
#include <OGP/Scripting/Audio/SoundEffectsScript.hpp>
#include <OGP/Scripting/Environment/GardenCollectionScript.hpp>

#include <OGPGame/Game.hpp>
#include <OGPGame/SceneManagement/IntroductionSceneLoader.hpp>
#include <OGPGame/SceneManagement/MainMenuSceneLoader.hpp>
#include <OGPGame/SceneManagement/GameSceneLoader.hpp>
#include <OGPGame/Scripting/DroppedGardenCollectionFileLoaderScript.hpp>

using namespace std;
using namespace std::filesystem;
#ifdef IS_OGP_CXX_STD_17
using namespace tcb;
#endif
using namespace Klein::Raylib;
using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;

using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Audio;
using namespace OGP::Scripting::Environment;

using namespace OGPGame;
using namespace OGPGame::SceneManagement;
using namespace OGPGame::Scripting;

static const ResourceID baseSceneLoaderResourceID("Base");
static const ResourceID introductionSceneLoaderResourceID("Introduction");
static const ResourceID mainMenuSceneLoaderResourceID("MainMenu");
static const ResourceID gameSceneLoaderResourceID("Game");

unique_ptr<RaylibEngine> Game::engine;
weak_ptr<GameSceneLoader> Game::gameSceneLoader;
weak_ptr<Node> Game::currentSceneNode;

int Game::Start(const span<const string>& commandLineArguments) {

	// TODO: Add fullscreen support in the future

	// TODO: Use values from configuration file

	engine = make_unique<RaylibEngine>(string("Open Garden Platformer"), static_cast<size_t>(1920 / 2), static_cast<size_t>(1080 / 2), commandLineArguments, path("./game_config.json"));
	engine->RegisterSceneLoader<BaseSceneLoader>(baseSceneLoaderResourceID);
	engine->RegisterSceneLoader<IntroductionSceneLoader>(introductionSceneLoaderResourceID);
	engine->RegisterSceneLoader<MainMenuSceneLoader>(mainMenuSceneLoaderResourceID);
	bool is_garden_collection_path_specified(commandLineArguments.size() > static_cast<size_t>(1));
	path garden_collection_path(is_garden_collection_path_specified ? path(commandLineArguments[static_cast<size_t>(1)]) : FileSystem::GetFilePathFromResourceID(ResourceID("GardenCollections/DAISYG.DGF")));
	gameSceneLoader = engine->RegisterSceneLoader<GameSceneLoader>(gameSceneLoaderResourceID, garden_collection_path);

	// TODO: Add introduction scene

	// TODO: Add main menu scene

	engine->CreateNewSceneNode(baseSceneLoaderResourceID)->CreateNewChild()->AddScript<DroppedGardenCollectionFileLoaderScript>();
	if (is_garden_collection_path_specified) {
		PlayGardenCollection(garden_collection_path);
	}
	else {
		ShowIntroductionScene();
	}
	return engine->Start();
}

void Game::ShowIntroductionScene() {
	if (!engine) {
		return;
	}
	UnloadCurrentSceneNode();
	currentSceneNode = engine->CreateNewSceneNode(introductionSceneLoaderResourceID);
}

void Game::ShowMainMenuScene() {
	if (!engine) {
		return;
	}
	UnloadCurrentSceneNode();
	currentSceneNode = engine->CreateNewSceneNode(mainMenuSceneLoaderResourceID);
}

void Game::PlayGardenCollection(const path& gardenCollectionPath) {
	if (!engine) {
		return;
	}
	if (shared_ptr<GameSceneLoader> game_scene_loader = gameSceneLoader.lock()) {
		UnloadCurrentSceneNode();
		game_scene_loader->SetGardenCollectionPath(gardenCollectionPath);
		currentSceneNode = engine->CreateNewSceneNode(gameSceneLoaderResourceID);
	}
}

void Game::UnloadCurrentSceneNode() {
	if (shared_ptr<Node> current_scene_node = currentSceneNode.lock()) {
		engine->RemoveSceneNode(current_scene_node);
		currentSceneNode.reset();
	}
}
