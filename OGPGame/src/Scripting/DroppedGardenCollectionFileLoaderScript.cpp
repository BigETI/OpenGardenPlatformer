#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <string>

#include <Klein/Engine.hpp>
#include <Klein/Hashing/StringHash.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Script.hpp>

#include <OGPGame/Game.hpp>
#include <OGPGame/Scripting/DroppedGardenCollectionFileLoaderScript.hpp>

using namespace std;
using namespace std::chrono;
using namespace std::filesystem;

using namespace Klein;
using namespace Klein::Hashing;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting;

using namespace OGPGame;
using namespace OGPGame::Scripting;

static const StringHash fileSystemDroppedFilePathInputEventStringHash("FileSystem.DroppedFilePath");

DroppedGardenCollectionFileLoaderScript::DroppedGardenCollectionFileLoaderScript(Node* node) : Script(node) {
	// ...
}

void DroppedGardenCollectionFileLoaderScript::OnGameTick(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	for (const auto& input_event : engine.GetCurrentInputEvents()) {
		if (input_event.GetNameHash() == fileSystemDroppedFilePathInputEventStringHash) {
			path path(input_event.GetPath());
			if (path.empty() || !path.has_extension()) {
				continue;
			}
			string extension(path.extension().string());
			transform(
				extension.begin(),
				extension.end(),
				extension.begin(),
				[](string::value_type character) {
					return tolower(character);
				}
			);
			if (extension != ".dgf") {
				continue;
			}
			Game::PlayGardenCollection(path);
			break;
		}
	}
}
