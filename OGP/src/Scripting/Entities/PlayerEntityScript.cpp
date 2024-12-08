#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/Hashing/StringHash.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::Hashing;
using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Entities;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

const StringHash wKeyStringHash("Keyboard.KeyCode.87");
const StringHash aKeyStringHash("Keyboard.KeyCode.65");
const StringHash sKeyStringHash("Keyboard.KeyCode.83");
const StringHash dKeyStringHash("Keyboard.KeyCode.68");
const StringHash qKeyStringHash("Keyboard.KeyCode.81");
const StringHash eKeyStringHash("Keyboard.KeyCode.69");
constexpr const float maximalMovementSpeed(80.0f / 8.0f);

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

bool PlayerEntityScript::Kill() {
	bool ret(isAlive);
	if (ret) {
		isAlive = false;
		OnDied();
	}
	return ret;
}

bool PlayerEntityScript::Win() {
	bool ret(hasNotWonYet);
	if (ret) {
		hasNotWonYet = true;
		OnWon();
	}
	return ret;
}

Vector2<float> PlayerEntityScript::GetToBeRenderedPosition() const noexcept {
	return EntityScript::GetToBeRenderedPosition() + toBeRenderedAtOffset;
}

void PlayerEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	targetPosition = gardenEntityData.position;
	isAlive = true;
	hasNotWonYet = true;
	movementProgress = 0.0f;
	EntityScript::Spawn(gardenEntityData, garden);
}

void PlayerEntityScript::OnGameTick(Engine& engine, high_resolution_clock::duration deltaTime) {
	if (isAlive) {
		if (shared_ptr<GardenScript> current_garden = GetGarden().lock()) {

			// TODO: Move input handler to a separate script
			// TODO: Add support for more keys
			// TODO: Add gamepad support
			// TODO: Add mouse support
			for (const auto& input_event : engine.GetCurrentInputEvents()) {
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

			shared_ptr<EntityScript> mounted_at_entity = GetMountedAtEntity().lock();
			bool is_walking_or_climbing_enabled(mounted_at_entity || current_garden->IsWalkableAt(GetCurrentPosition()) || current_garden->IsClimbableAt(GetCurrentPosition()));
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
			movementProgress += delta_time * maximalMovementSpeed;
			do {
				if (current_garden->IsWinnableAt(GetCurrentPosition())) {
					Win();
					toBeRenderedAtOffset = Vector2<float>();
					break;
				}
				if (current_garden->IsDeadlyAt(GetCurrentPosition()) || current_garden->IsSolidAt(GetCurrentPosition())) {
					Kill();
					toBeRenderedAtOffset = Vector2<float>();
					break;
				}
				current_garden->InteractAt(GetCurrentPosition(), *this);
				mounted_at_entity = GetMountedAtEntity().lock();
				if (GetCurrentPosition().y > static_cast<size_t>(0)) {
					Vector2<size_t> bottom_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
					if (current_garden->IsSolidAt(bottom_position) && current_garden->IsTopDeadlyAt(bottom_position)) {
						Kill();
						toBeRenderedAtOffset = Vector2<float>();
						break;
					}
				}
				float movement_progress(min(movementProgress, 1.0f));
				bool is_falling(false);
				if (mounted_at_entity) {
					Vector2<int> relative_movement(targetPosition.GetConverted<int>() - GetCurrentPosition().GetConverted<int>());
					SetCurrentPosition(mounted_at_entity->GetCurrentPosition());
					Vector2<int> signed_target_position(GetCurrentPosition().GetConverted<int>() + relative_movement);
					targetPosition = Vector2<size_t>((signed_target_position.x < 0) ? static_cast<size_t>(0) : static_cast<size_t>(signed_target_position.x), (signed_target_position.y < 0) ? static_cast<size_t>(0) : static_cast<size_t>(signed_target_position.y));
					toBeRenderedAtOffset = Vector2<float>();
				}
				else if (!current_garden->IsWalkableAt(GetCurrentPosition()) && !current_garden->IsClimbableAt(GetCurrentPosition())) {
					is_falling = true;
				}
				Vector2<int> remaining_movement(is_falling ? Vector2<int>(0, -1) : (targetPosition.GetConverted<int>() - GetCurrentPosition().GetConverted<int>()));
				bool is_not_moving(true);
				if (remaining_movement.y > 0) {
					if (current_garden->IsClimbingUpAllowedAt(GetCurrentPosition())) {
						is_not_moving = false;
						if (movement_progress >= 1.0f) {
							SetCurrentPosition(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
							toBeRenderedAtOffset = Vector2<float>();
						}
						else {
							toBeRenderedAtOffset = Vector2<float>(0.0f, movement_progress);
						}
					}
				}
				else if (remaining_movement.y < 0) {
					if (!mounted_at_entity && (GetCurrentPosition().y > static_cast<size_t>(0)) && !current_garden->IsSolidAt(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)))) {
						is_not_moving = false;
						if ((movement_progress >= 0.5f) && current_garden->IsTopDeadlyAt(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)))) {
							Kill();
							break;
						}
						if (movement_progress >= 1.0f) {
							SetCurrentPosition(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
							toBeRenderedAtOffset = Vector2<float>();
						}
						else {
							toBeRenderedAtOffset = Vector2<float>(0.0f, -movement_progress);
						}
					}
				}
				if (is_not_moving) {
					if (remaining_movement.x < 0) {
						if ((GetCurrentPosition().x > static_cast<size_t>(0))) {
							Vector2<size_t> left_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
							current_garden->InteractAt(left_position, *this);
							if (!current_garden->IsSolidAt(left_position)) {
								is_not_moving = false;
								if (movement_progress >= 1.0f) {
									SetCurrentPosition(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
									if (mounted_at_entity) {
										Dismount();
									}
									toBeRenderedAtOffset = Vector2<float>();
								}
								else {
									toBeRenderedAtOffset = Vector2<float>(-movement_progress, 0.0f);
								}
							}
						}
					}
					else if (remaining_movement.x > 0) {
						Vector2<size_t> right_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
						current_garden->InteractAt(right_position, *this);
						if (!current_garden->IsSolidAt(right_position)) {
							is_not_moving = false;
							if (movement_progress >= 1.0f) {
								SetCurrentPosition(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
								if (mounted_at_entity) {
									Dismount();
								}
								toBeRenderedAtOffset = Vector2<float>();
							}
							else {
								toBeRenderedAtOffset = Vector2<float>(movement_progress, 0.0f);
							}
						}
					}
				}
				if (is_not_moving) {
					movementProgress = 0.0f;
					targetPosition = GetCurrentPosition();
					toBeRenderedAtOffset = Vector2<float>();
				}
				else if (movementProgress >= 1.0f) {
					movementProgress = max(movementProgress - 1.0f, 0.0f);
					if (is_falling && (targetPosition == (GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0))))) {
						targetPosition = GetCurrentPosition();
						toBeRenderedAtOffset = Vector2<float>();
					}
				}
			} while (movementProgress >= 1.0f);
		}
	}
}
