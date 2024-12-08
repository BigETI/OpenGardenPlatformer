#include <filesystem>
#include <string>
#include <vector>

#include <Klein/Engine.hpp>
#include <Klein/InputSystem/Raylib/RaylibInputHandler.hpp>
#include <Klein/Rendering/Raylib/RaylibWindowRenderer.hpp>

using namespace Klein;
using namespace Klein::InputSystem::Raylib;
using namespace Klein::Rendering::Raylib;
using namespace std;
using namespace std::filesystem;

const path editorConfigurationFilePath("./game_config.json");

int main(int argc, char* argv[]) {
	vector<string> command_line_arguments;
	for (int index(0); index < argc; index++) {
		command_line_arguments.push_back(string(argv[index]));
	}
	Engine engine(command_line_arguments, editorConfigurationFilePath);

	// TODO: Add fullscreen support in the future
	// TODO: Use values from configuration file
	engine.AddRenderer(make_shared<RaylibWindowRenderer>(string("Open Garden Platformer Editor"), 1280U, 960U));
	engine.AddInputHandler(make_shared<RaylibInputHandler>());

	// TODO: Add editor UI

	// TODO: Add editor environment
	
	return engine.Start();
}
