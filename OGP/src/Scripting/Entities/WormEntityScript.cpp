#include <chrono>

#include <OGP/Game.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/WormEntityScript.hpp>

using namespace OGP;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Entities;
using namespace std::chrono;

WormEntityScript::WormEntityScript(Node* node) : EntityScript(node) {
	// ...
}

void WormEntityScript::OnGameTick(Game& game, high_resolution_clock::duration deltaTime) {
	// TODO: Implement worm logic
}
