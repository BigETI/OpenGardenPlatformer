#include <string>
#include <vector>

#include <OGPGame/Game.hpp>

using namespace std;

using namespace OGPGame;

int main(int argc, char* argv[]) {
	vector<string> command_line_arguments;
	for (int index(0); index < argc; index++) {
		command_line_arguments.emplace_back(argv[index]);
	}
	return Game::Start(command_line_arguments);
}
