#include <chrono>

#include <OGP/Game.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/MarmotEntityScript.hpp>

using namespace OGP;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Entities;
using namespace std::chrono;

MarmotEntityScript::MarmotEntityScript(Node* node) : EntityScript(node) {
	// ...
}

void MarmotEntityScript::OnGameTick(Game& game, high_resolution_clock::duration deltaTime) {
	// TODO: Implement marmot logic
}
