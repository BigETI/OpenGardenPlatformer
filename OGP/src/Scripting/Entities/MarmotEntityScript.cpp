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

// TODO: Match marmot behaviour from original game

HumanoidInput MarmotEntityScript::GetInput(const Engine& engine) const noexcept {
	HumanoidInput ret;
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		Vector2<size_t> current_position(GetCurrentPosition());
		Vector2<size_t> target_position(GetGardenEntityData().position);
		garden->EnumerateEntities<PlayerEntityScript>(
			[this, &current_position, &target_position](const PlayerEntityScript& player) {
				if (player.IsMushroomEffectActive()) {
					return;
				}
				Vector2<size_t> player_position(player.GetCurrentPosition());
				if (GetGardenEntityData().bounds.IsContained(player_position.GetConverted<std::int64_t>())) {
					if (player.IsGarlicEffectActive()) {
						Vector2<int> delta(player_position.GetConverted<int>() - current_position.GetConverted<int>());
						Vector2<int> unsigned_delta(abs(delta.x), abs(delta.y));
						if ((unsigned_delta.x <= 2) && (unsigned_delta.y <= 2)) {
							Vector2<int> to_be_clamped_target_position;
							if (delta.x < 0) {
								to_be_clamped_target_position = player_position.GetConverted<int>() + Vector2<int>(2, 0);
							}
							else if (delta.x > 0) {
								to_be_clamped_target_position = player_position.GetConverted<int>() - Vector2<int>(2, 0);
							}
							else if (delta.y < 0) {
								to_be_clamped_target_position = player_position.GetConverted<int>() + Vector2<int>(0, 2);
							}
							else if (delta.y > 0) {
								to_be_clamped_target_position = player_position.GetConverted<int>() - Vector2<int>(0, 2);
							}
							else {
								to_be_clamped_target_position = player_position.GetConverted<int>();
							}
							to_be_clamped_target_position.x = max(to_be_clamped_target_position.x, 0);
							to_be_clamped_target_position.y = max(to_be_clamped_target_position.y, 0);
							target_position = to_be_clamped_target_position.GetConverted<size_t>();
						}
						else {
							target_position = player_position;
						}
					}
					else {
						target_position = player_position;
					}
				}
			});
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
