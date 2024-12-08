#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/MarmotEntityScript.hpp>

using namespace std::chrono;

using namespace Klein;
using namespace Klein::SceneManagement;

using namespace OGP::Scripting::Entities;

MarmotEntityScript::MarmotEntityScript(Node* node) : EntityScript(node) {
	// ...
}

void MarmotEntityScript::OnGameTick(Engine& engine, high_resolution_clock::duration deltaTime) {
	// TODO: Implement marmot logic
}
