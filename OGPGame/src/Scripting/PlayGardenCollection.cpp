#include <Klein/ResourceManagement/FileSystem.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGPGame/Game.hpp>
#include <OGPGame/Scripting/PlayGardenCollectionScript.hpp>

using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;

using namespace OGPGame;
using namespace OGPGame::Scripting;

PlayGardenCollectionScript::PlayGardenCollectionScript(Node* node) : Script(node) {
	// ...
}

void PlayGardenCollectionScript::PlayGardenCollection(const ResourceID& gardenCollectionResourceID) const {
	Game::PlayGardenCollection(FileSystem::GetFilePathFromResourceID(gardenCollectionResourceID));
}
