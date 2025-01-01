#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

#include <Klein/Raylib/RaylibEngine.hpp>

using namespace std;
using namespace std::filesystem;

using namespace Klein::Raylib;

const path editorConfigurationFilePath("./game_config.json");

int main(int argc, char* argv[]) {
	vector<string> command_line_arguments;
	for (int index(0); index < argc; index++) {
		command_line_arguments.push_back(string(argv[index]));
	}

	// TODO: Add fullscreen support in the future
	// TODO: Use values from configuration file
	RaylibEngine engine(string("Open Garden Platformer"), static_cast<size_t>(1280), static_cast<size_t>(960), command_line_arguments, editorConfigurationFilePath);

	// TODO: Add editor UI

	// TODO: Add editor environment
	
	return engine.Start();
}
