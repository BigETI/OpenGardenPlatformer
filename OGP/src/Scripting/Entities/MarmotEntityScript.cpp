#include <chrono>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Rectangle.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Physics/AABBColliderScript.hpp>

#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/MarmotEntityScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Physics;

using namespace OGP::Scripting::Entities;

MarmotEntityScript::MarmotEntityScript(Node* node) : EntityScript(node) {
	shared_ptr<AABBColliderScript> collider(GetNode().CreateNewChild()->EnsureScript<AABBColliderScript>());
	collider->SetLocalCollisionRectangle(Rectangle<float>(Vector2<float>(), Vector2<float>(0.5f, 1.0f)));
}

bool MarmotEntityScript::IsDeadly() const noexcept {
	return true;
}

void MarmotEntityScript::OnGameTick(Engine& engine, high_resolution_clock::duration deltaTime) {
	// TODO: Implement marmot logic
}
