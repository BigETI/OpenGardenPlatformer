#include <filesystem>
#include <memory>

#include <Klein/ResourceManagement/FileSystem.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Scripting/Environment/GardenCollectionScript.hpp>

#include <OGPGame/Game.hpp>
#include <OGPGame/SceneManagement/GameSceneLoader.hpp>

using namespace std;
using namespace std::filesystem;

using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;

using namespace OGP::Scripting::Environment;

using namespace OGPGame;
using namespace OGPGame::SceneManagement;

GameSceneLoader::GameSceneLoader(const path& gardenCollectionPath) : gardenCollectionPath(gardenCollectionPath) {
	// ...
}

const path& GameSceneLoader::GetGardenCollectionPath() const noexcept {
	return gardenCollectionPath;
}

void GameSceneLoader::SetGardenCollectionPath(const path& gardenCollectionPath) {
	this->gardenCollectionPath = gardenCollectionPath;
}

void GameSceneLoader::Load(Node& rootNode) {
	shared_ptr<GardenCollectionScript> garden_collection(rootNode.CreateNewChild("GardenCollection")->AddScript<GardenCollectionScript>());
	garden_collection->OnClosureRequested += []() {
		Game::ShowMainMenuScene();
	};
	garden_collection->LoadGardenCollectionFile(gardenCollectionPath);
}
