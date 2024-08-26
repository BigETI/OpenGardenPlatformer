#include <filesystem>
#include <string>
#include <vector>

#include <OGP/Game.hpp>
#include <OGP/InputSystem/Raylib/RaylibInputHandler.hpp>
#include <OGP/Rendering/Raylib/RaylibWindowRenderer.hpp>

using namespace OGP;
using namespace OGP::InputSystem::Raylib;
using namespace OGP::Rendering::Raylib;
using namespace std;
using namespace std::filesystem;

const path editorConfigurationFilePath("./game_config.json");

int main(int argc, char* argv[]) {
	vector<string> command_line_arguments;
	for (int index(0); index < argc; index++) {
		command_line_arguments.push_back(string(argv[index]));
	}
	Game game(command_line_arguments, editorConfigurationFilePath);

	// TODO: Add fullscreen support in the future
	// TODO: Use values from configuration file
	game.AddRenderer(make_shared<RaylibWindowRenderer>(string("Open Garden Platformer Editor"), 1280U, 960U));
	game.AddInputHandler(make_shared<RaylibInputHandler>());

	// TODO: Add editor UI

	// TODO: Add editor environment
	
	return game.Start();
}
