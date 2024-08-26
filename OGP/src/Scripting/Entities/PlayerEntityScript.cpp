#include <chrono>
#include <cstddef>
#include <memory>

#include <OGP/Environment/GardenEntityData.hpp>
#include <OGP/Game.hpp>
#include <OGP/Math/Vector2.hpp>
#include <OGP/ResourceManagement/ResourceID.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>
#include <OGP/Scripting/Rendering/SpriteRendererScript.hpp>

using namespace OGP;
using namespace OGP::Environment;
using namespace OGP::Hashing;
using namespace OGP::Math;
using namespace OGP::ResourceManagement;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;
using namespace OGP::Scripting::Rendering;
using namespace std;
using namespace std::chrono;

const StringHash wKeyStringHash("Keyboard.KeyCode.87");
const StringHash aKeyStringHash("Keyboard.KeyCode.65");
const StringHash sKeyStringHash("Keyboard.KeyCode.83");
const StringHash dKeyStringHash("Keyboard.KeyCode.68");
const StringHash qKeyStringHash("Keyboard.KeyCode.81");
const StringHash eKeyStringHash("Keyboard.KeyCode.69");
constexpr const float movementSpeed(80.0f / 8.0f);

PlayerEntityScript::PlayerEntityScript(Node* node) :
	EntityScript(node),
	isAlive(true),
	hasNotWonYet(true),
	movementProgress(0.0f),
	isWalkingLeft(false),
	isWalkingRight(false),
	isWalkingUp(false),
	isWalkingDown(false),
	isDiggingLeft(false),
	isDiggingRight(false) {
	if (shared_ptr<SpriteRendererScript> sprite_renderer = GetSpriteRenderer().lock()) {
		sprite_renderer->SetLayerIndex(0U);
	}
}

bool PlayerEntityScript::IsAlive() const noexcept {
	return isAlive;
}

void PlayerEntityScript::Kill() {
	if (isAlive) {
		isAlive = false;
		OnDied();
	}
}

void PlayerEntityScript::Win() {
	if (hasNotWonYet) {
		hasNotWonYet = true;
		OnWon();
	}
}

void PlayerEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	targetPosition = gardenEntityData.position;
	isAlive = true;
	hasNotWonYet = true;
	movementProgress = 0.0f;
	EntityScript::Spawn(gardenEntityData, garden);
}

void PlayerEntityScript::OnGameTick(Game& game, high_resolution_clock::duration deltaTime) {
	if (isAlive) {
		if (shared_ptr<GardenScript> current_garden = GetGarden().lock()) {

			// TODO: Move input handler to a separate script
			// TODO: Add support for more keys
			// TODO: Add gamepad support
			// TODO: Add mouse support
			for (const auto& input_event : game.GetCurrentInputEvents()) {
				if (input_event.GetNameHash() == wKeyStringHash) {
					isWalkingUp = input_event.IsPressing();
				}
				if (input_event.GetNameHash() == aKeyStringHash) {
					isWalkingLeft = input_event.IsPressing();
				}
				if (input_event.GetNameHash() == sKeyStringHash) {
					isWalkingDown = input_event.IsPressing();
				}
				if (input_event.GetNameHash() == dKeyStringHash) {
					isWalkingRight = input_event.IsPressing();
				}
				if (input_event.GetNameHash() == qKeyStringHash) {
					isDiggingLeft = input_event.IsPressing();
				}
				if (input_event.GetNameHash() == eKeyStringHash) {
					isDiggingRight = input_event.IsPressing();
				}
			}

			bool is_walking_or_climbing_enabled(current_garden->IsWalkableAt(GetCurrentPosition()) || current_garden->IsClimbableAt(GetCurrentPosition()));
			float delta_time(duration<float>(deltaTime).count());
			if (GetCurrentPosition() == targetPosition) {
				if (isWalkingUp || isWalkingDown || isWalkingLeft || isWalkingRight) {
					if (isWalkingLeft && is_walking_or_climbing_enabled) {
						targetPosition = (targetPosition.x > static_cast<size_t>(0)) ? (targetPosition - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0))) : targetPosition;
					}
					if (isWalkingRight && is_walking_or_climbing_enabled) {
						targetPosition += Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0));
					}
					if (isWalkingUp) {
						targetPosition += Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1));
					}
					if (isWalkingDown) {
						targetPosition = (targetPosition.y > static_cast<size_t>(0)) ? (targetPosition - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1))) : targetPosition;
					}
				}
			}
			if (isDiggingLeft && (GetCurrentPosition().x > static_cast<size_t>(0)) && (GetCurrentPosition().y > static_cast<size_t>(0)) && !current_garden->IsSolidAt(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)))) {
				current_garden->DigAt(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(1)));
			}
			if (isDiggingRight && (GetCurrentPosition().y > static_cast<size_t>(0)) && !current_garden->IsSolidAt(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)))) {
				current_garden->DigAt(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)) - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
			}
			movementProgress += delta_time * movementSpeed;
			do {
				if (current_garden->IsWinnableAt(GetCurrentPosition())) {
					Win();
					break;
				}
				if (current_garden->IsDeadlyAt(GetCurrentPosition()) || current_garden->IsSolidAt(GetCurrentPosition())) {
					Kill();
					break;
				}
				current_garden->InteractAt(GetCurrentPosition(), this);
				if (GetCurrentPosition().y > static_cast<size_t>(0)) {
					Vector2<size_t> bottom_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
					if (current_garden->IsSolidAt(bottom_position) && current_garden->IsTopDeadlyAt(bottom_position)) {
						Kill();
						break;
					}
				}
				float movement_progress(min(movementProgress, 1.0f));
				bool is_falling(!current_garden->IsWalkableAt(GetCurrentPosition()) && !current_garden->IsClimbableAt(GetCurrentPosition()));
				Vector2<int> remaining_movement(is_falling ? Vector2<int>(0, -1) : (targetPosition.GetConverted<int>() - GetCurrentPosition().GetConverted<int>()));
				bool is_not_moving(true);
				if (remaining_movement.y > 0) {
					if (current_garden->IsClimbingUpAllowedAt(GetCurrentPosition())) {
						is_not_moving = false;
						GetNode().SetLocalPosition(GetCurrentPosition().GetConverted<float>() + Vector2<float>(0.0f, movement_progress));
						if (movement_progress >= 1.0f) {
							SetCurrentPosition(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
						}
					}
				}
				else if (remaining_movement.y < 0) {
					if ((GetCurrentPosition().y > static_cast<size_t>(0)) && !current_garden->IsSolidAt(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)))) {
						is_not_moving = false;
						GetNode().SetLocalPosition(GetCurrentPosition().GetConverted<float>() - Vector2<float>(0.0f, movement_progress));
						if ((movement_progress >= 0.5f) && current_garden->IsTopDeadlyAt(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)))) {
							Kill();
							break;
						}
						if (movement_progress >= 1.0f) {
							SetCurrentPosition(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
						}
					}
				}
				if (is_not_moving) {
					if (remaining_movement.x < 0) {
						if ((GetCurrentPosition().x > static_cast<size_t>(0))) {
							Vector2<size_t> left_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
							current_garden->InteractAt(left_position, this);
							if (!current_garden->IsSolidAt(left_position)) {
								is_not_moving = false;
								GetNode().SetLocalPosition(GetCurrentPosition().GetConverted<float>() - Vector2<float>(movement_progress, 0.0f));
								if (movement_progress >= 1.0f) {
									SetCurrentPosition(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
								}
							}
						}
					}
					else if (remaining_movement.x > 0) {
						Vector2<size_t> right_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
						current_garden->InteractAt(right_position, this);
						if (!current_garden->IsSolidAt(right_position)) {
							is_not_moving = false;
							GetNode().SetLocalPosition(GetCurrentPosition().GetConverted<float>() + Vector2<float>(movement_progress, 0.0f));
							if (movement_progress >= 1.0f) {
								SetCurrentPosition(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
							}
						}
					}
				}
				if (is_not_moving) {
					movementProgress = 0.0f;
					targetPosition = GetCurrentPosition();
				}
				else if (movementProgress >= 1.0f) {
					movementProgress = max(movementProgress - 1.0f, 0.0f);
					if (is_falling && (targetPosition == (GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0))))) {
						targetPosition = GetCurrentPosition();
					}
				}
			} while (movementProgress >= 1.0f);
		}
	}
}
