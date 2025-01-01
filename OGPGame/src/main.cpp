#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <Klein/Math/Ratio.hpp>
#include <Klein/Raylib/RaylibEngine.hpp>
#include <Klein/Rendering/Raylib/RaylibWindowRenderer.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Scripting/Environment/GardenCollectionScript.hpp>

using namespace std;
using namespace std::filesystem;

using namespace Klein::Math;
using namespace Klein::Raylib;
using namespace Klein::SceneManagement;

using namespace OGP::Scripting::Environment;

const path gameConfigurationFilePath("./game_config.json");

int main(int argc, char* argv[]) {
	vector<string> command_line_arguments;
	for (int index(0); index < argc; index++) {
		command_line_arguments.push_back(string(argv[index]));
	}

	// TODO: Add fullscreen support in the future
	// TODO: Use values from configuration file
	RaylibEngine engine(string("Open Garden Platformer"), static_cast<size_t>(1280), static_cast<size_t>(960), command_line_arguments, gameConfigurationFilePath);

	// TODO: Add introduction scene

	// TODO: Add main menu scene

	// Environment
	shared_ptr<Node> game_scene(engine.CreateNewEmptyScene());
	shared_ptr<Node> environment_node(game_scene->CreateNewChild("Environment"));
	shared_ptr<GardenCollectionScript> garden_collection(environment_node->CreateNewChild("GardenCollection")->AddScript<GardenCollectionScript>());
	
	// TODO: Test
	garden_collection->LoadGardenCollectionFile("C:\\dosbox\\C\\DAISYG2\\DAISYG.DGF");

	return engine.Start();
}
