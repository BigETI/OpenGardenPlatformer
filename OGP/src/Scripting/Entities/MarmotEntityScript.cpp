#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Rectangle.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Physics/AABBColliderScript.hpp>

#include <OGP/Entities/HumanoidInput.hpp>
#include <OGP/Scripting/Entities/HumanoidEntityScript.hpp>
#include <OGP/Scripting/Entities/MarmotEntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Physics;

using namespace OGP::Entities;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

MarmotEntityScript::MarmotEntityScript(Node* node) : HumanoidEntityScript(node) {
	shared_ptr<AABBColliderScript> collider(GetNode().CreateNewChild()->EnsureScript<AABBColliderScript>());
	collider->SetLocalCollisionRectangle(Rectangle<float>(Vector2<float>(), Vector2<float>(0.5f, 1.0f)));
}

bool MarmotEntityScript::IsDeadly() const noexcept {
	return true;
}

HumanoidInput MarmotEntityScript::GetInput(const Engine& engine) {
	HumanoidInput ret;
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		Vector2<size_t> target_position(GetGardenEntityData().position);
		garden->EnumerateEntities<PlayerEntityScript>(
			[this, &target_position](const PlayerEntityScript& player) {
				Vector2<size_t> player_position(player.GetCurrentPosition());
				if (GetGardenEntityData().bounds.IsContained(player_position.GetConverted<std::int64_t>())) {
					target_position = player_position;
				}
			});
		Vector2<size_t> current_position(GetCurrentPosition());
		if (current_position != target_position) {
			if ((current_position.y < target_position.y) && garden->IsClimbingUpAllowedAt(current_position)) {
				ret.isWalkingUp = true;
			}
			else if (current_position.x != target_position.x) {
				ret.isWalkingLeft = current_position.x > target_position.x;
				ret.isWalkingRight = !ret.isWalkingLeft;
			}
			else {
				ret.isWalkingDown = true;
			}
		}
	}
	return ret;
}
