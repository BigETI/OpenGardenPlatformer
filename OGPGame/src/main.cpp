#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <OGP/Game.hpp>
#include <OGP/InputSystem/Raylib/RaylibInputHandler.hpp>
#include <OGP/Math/Ratio.hpp>
#include <OGP/Rendering/Raylib/RaylibWindowRenderer.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Environment/GardenCollectionScript.hpp>

using namespace OGP;
using namespace OGP::InputSystem::Raylib;
using namespace OGP::Math;
using namespace OGP::Rendering::Raylib;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Environment;
using namespace std;
using namespace std::filesystem;

const path gameConfigurationFilePath("./game_config.json");

int main(int argc, char* argv[]) {
	vector<string> command_line_arguments;
	for (int index(0); index < argc; index++) {
		command_line_arguments.push_back(string(argv[index]));
	}
	Game game(command_line_arguments, gameConfigurationFilePath);

	// TODO: Add fullscreen support in the future
	// TODO: Use values from configuration file
	game.AddRenderer(make_shared<RaylibWindowRenderer>(string("Open Garden Platformer"), 1280U, 960U));
	game.AddInputHandler(make_shared<RaylibInputHandler>());

	// TODO: Add introduction scene

	// TODO: Add main menu scene

	// Environment
	shared_ptr<Node> game_scene(game.CreateNewEmptyScene());
	shared_ptr<Node> environment_node(game_scene->CreateNewChild("Environment"));
	shared_ptr<GardenCollectionScript> garden_collection(environment_node->CreateNewChild("GardenCollection")->AddScript<GardenCollectionScript>());
	
	// TODO: Test
	garden_collection->LoadGardenCollectionFile("C:\\dosbox\\C\\DAISYG2\\DAISYG.DGF");

	return game.Start();
}
