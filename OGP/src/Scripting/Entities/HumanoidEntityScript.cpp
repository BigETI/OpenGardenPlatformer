#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Entities/EHumanoidMovementState.hpp>
#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Entities/HumanoidInput.hpp>
#include <OGP/Environment/EKillerType.hpp>
#include <OGP/Environment/GlobalWorld.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/HumanoidEntityScript.hpp>
#include <OGP/Scripting/Entities/LiftEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::SceneManagement;

using namespace OGP::Entities;
using namespace OGP::Environment;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

HumanoidEntityScript::HumanoidEntityScript(Node* node) :
	EntityScript(node),
	movementProgress(0.0f),
	movementState(EHumanoidMovementState::Standing),
	hasExitedLift(false) {
	// ...
}

const Vector2<size_t>& HumanoidEntityScript::GetTargetPosition() const noexcept {
	return targetPosition;
}

float HumanoidEntityScript::GetMovementProgress() const noexcept {
	return movementProgress;
}

bool HumanoidEntityScript::IsAlive() const noexcept {
	return true;
}

float HumanoidEntityScript::GetMaximalMovementSpeed() const noexcept {
	return GlobalWorld::GetSpeed(8.0f);
}

bool HumanoidEntityScript::Kill(EKillerType killerType) {
	return false;
}

bool HumanoidEntityScript::Win() {
	return false;
}

Vector2<float> HumanoidEntityScript::GetToBeRenderedPosition() const noexcept {
	bool is_movable(true);
	if (shared_ptr<EntityScript> mounted_at_entity = GetMountedAtEntity().lock()) {
		if (shared_ptr<LiftEntityScript> mounted_at_lift_entity = dynamic_pointer_cast<LiftEntityScript>(mounted_at_entity)) {
			switch (mounted_at_lift_entity->GetLiftMovementState()) {
			case ELiftMovementState::Up:
			case ELiftMovementState::Down:
			case ELiftMovementState::Left:
			case ELiftMovementState::Right:
				is_movable = false;
				break;
			case ELiftMovementState::ParkingFromMovingUp:
			case ELiftMovementState::ParkingFromMovingDown:
			case ELiftMovementState::ParkingFromMovingLeft:
			case ELiftMovementState::ParkingFromMovingRight:
				break;
			}
		}
		else {
			is_movable = false;
		}
	}
	return is_movable ? (EntityScript::GetToBeRenderedPosition() + toBeRenderedAtOffset) : EntityScript::GetToBeRenderedPosition();
}

void HumanoidEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	targetPosition = gardenEntityData.position;
	movementProgress = 0.0f;
	EntityScript::Spawn(gardenEntityData, garden);
}

HumanoidInput HumanoidEntityScript::GetInput(const Engine& engine) const noexcept {
	return HumanoidInput();
}

void HumanoidEntityScript::OnGameTick(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	if (!IsAlive()) {
		return;
	}
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		if (garden->GetGardenState() != EGardenState::Playing) {
			return;
		}
		HumanoidInput input = GetInput(engine);
		shared_ptr<EntityScript> mounted_at_entity = GetMountedAtEntity().lock();
		Vector2<size_t> current_position(GetCurrentPosition());
		bool is_walking_or_climbing_enabled(mounted_at_entity || garden->IsWalkableAt(current_position) || garden->IsClimbableAt(current_position));
		float delta_time(duration<float>(deltaTime).count());
		if (mounted_at_entity || (current_position == targetPosition)) {
			targetPosition = current_position;
			if (input.isWalkingUp || input.isWalkingDown || input.isWalkingLeft || input.isWalkingRight) {
				if (input.isWalkingLeft && is_walking_or_climbing_enabled) {
					targetPosition = (targetPosition.x > static_cast<size_t>(0)) ? (targetPosition - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0))) : targetPosition;
				}
				if (input.isWalkingRight && is_walking_or_climbing_enabled) {
					targetPosition += Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0));
				}
				if (input.isWalkingUp) {
					targetPosition += Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1));
				}
				if (input.isWalkingDown) {
					targetPosition = (targetPosition.y > static_cast<size_t>(0)) ? (targetPosition - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1))) : targetPosition;
				}
			}
		}
		if (input.isDiggingLeft && (current_position.x > static_cast<size_t>(0)) && (current_position.y > static_cast<size_t>(0)) && !garden->IsSolidAt(current_position - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)))) {
			garden->DigAt(current_position - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(1)));
		}
		if (input.isDiggingRight && (current_position.y > static_cast<size_t>(0)) && !garden->IsSolidAt(current_position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)))) {
			garden->DigAt(current_position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)) - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
		}
		bool is_not_movable(false);
		if (shared_ptr<LiftEntityScript> mounted_at_lift_entity = dynamic_pointer_cast<LiftEntityScript>(mounted_at_entity)) {
			UpdateMovementState(EHumanoidMovementState::Mounted);
			switch (mounted_at_lift_entity->GetLiftMovementState()) {
			case ELiftMovementState::Up:
			case ELiftMovementState::Down:
			case ELiftMovementState::Left:
			case ELiftMovementState::Right:
				is_not_movable = true;
				break;
			case ELiftMovementState::ParkingFromMovingUp:
			case ELiftMovementState::ParkingFromMovingDown:
			case ELiftMovementState::ParkingFromMovingLeft:
			case ELiftMovementState::ParkingFromMovingRight:
				break;
			}
		}
		else if (mounted_at_entity) {
			UpdateMovementState(EHumanoidMovementState::Mounted);
			is_not_movable = true;
		}
		if (is_not_movable) {
			movementProgress = 0.0f;
			return;
		}
		movementProgress += delta_time * GetMaximalMovementSpeed();
		do {
			if (garden->IsWinnableAt(current_position)) {
				if (Win()) {
					toBeRenderedAtOffset = Vector2<float>();
				}
				break;
			}
			if (garden->IsDeadlyAt(current_position) || garden->IsSolidAt(current_position)) {
				if (Kill(EKillerType::Cell)) {
					toBeRenderedAtOffset = Vector2<float>();
				}
				break;
			}
			garden->InteractAt(current_position, *this);
			mounted_at_entity = GetMountedAtEntity().lock();
			if ((current_position.y > static_cast<size_t>(0)) && (movementState == EHumanoidMovementState::Falling)) {
				Vector2<size_t> bottom_position(current_position - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
				if (garden->IsTopDeadlyAt(bottom_position) || garden->IsDeadlyAt(bottom_position)) {
					if (Kill(EKillerType::Cell)) {
						toBeRenderedAtOffset = Vector2<float>();
					}
					break;
				}
			}
			float movement_progress(min(movementProgress, 1.0f));
			EHumanoidMovementState movement_state(EHumanoidMovementState::Standing);
			if (mounted_at_entity) {
				Vector2<int> relative_movement(targetPosition.GetConverted<int>() - current_position.GetConverted<int>());
				SetCurrentPosition(mounted_at_entity->GetCurrentPosition());
				current_position = GetCurrentPosition();
				Vector2<int> signed_target_position(current_position.GetConverted<int>() + relative_movement);
				targetPosition = Vector2<size_t>((signed_target_position.x < 0) ? static_cast<size_t>(0) : static_cast<size_t>(signed_target_position.x), (signed_target_position.y < 0) ? static_cast<size_t>(0) : static_cast<size_t>(signed_target_position.y));
				toBeRenderedAtOffset = Vector2<float>();
				movement_state = EHumanoidMovementState::Mounted;
			}
			else if (garden->IsClimbableAt(current_position)) {
				movement_state = EHumanoidMovementState::Climbing;
			}
			else if (hasExitedLift || garden->IsWalkableAt(current_position)) {
				movement_state = (current_position == targetPosition) ? EHumanoidMovementState::Standing : EHumanoidMovementState::Walking;
			}
			else {
				movement_state = EHumanoidMovementState::Falling;
			}
			UpdateMovementState(movement_state);
			Vector2<int> remaining_movement((movement_state == EHumanoidMovementState::Falling) ? Vector2<int>(0, -1) : (targetPosition.GetConverted<int>() - current_position.GetConverted<int>()));
			bool is_not_moving(true);
			if (remaining_movement.y > 0) {
				if (garden->IsClimbingUpAllowedAt(current_position)) {
					is_not_moving = false;
					if (movement_progress >= 1.0f) {
						SetCurrentPosition(current_position + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
						current_position = GetCurrentPosition();
						hasExitedLift = false;
						toBeRenderedAtOffset = Vector2<float>();
					}
					else {
						toBeRenderedAtOffset = Vector2<float>(0.0f, movement_progress);
					}
				}
			}
			else if (remaining_movement.y < 0) {
				if (!mounted_at_entity && (current_position.y > static_cast<size_t>(0)) && !garden->IsSolidAt(current_position - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)))) {
					is_not_moving = false;
					if ((movement_progress >= 0.5f) && garden->IsTopDeadlyAt(current_position - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)))) {
						Kill(EKillerType::Cell);
						break;
					}
					if (movement_progress >= 1.0f) {
						SetCurrentPosition(current_position - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
						current_position = GetCurrentPosition();
						hasExitedLift = false;
						toBeRenderedAtOffset = Vector2<float>();
					}
					else {
						toBeRenderedAtOffset = Vector2<float>(0.0f, -movement_progress);
					}
				}
			}
			if (is_not_moving) {
				if (remaining_movement.x < 0) {
					if ((current_position.x > static_cast<size_t>(0))) {
						Vector2<size_t> left_position(current_position - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
						garden->InteractAt(left_position, *this);
						if (!garden->IsSolidAt(left_position)) {
							is_not_moving = false;
							if (mounted_at_entity) {
								if (shared_ptr<LiftEntityScript> mounted_at_lift_entity = dynamic_pointer_cast<LiftEntityScript>(mounted_at_entity)) {
									switch (mounted_at_lift_entity->GetLiftMovementState()) {
									case ELiftMovementState::Up:
									case ELiftMovementState::Down:
									case ELiftMovementState::Left:
									case ELiftMovementState::Right:
										break;
									case ELiftMovementState::ParkingFromMovingUp:
									case ELiftMovementState::ParkingFromMovingDown:
									case ELiftMovementState::ParkingFromMovingLeft:
									case ELiftMovementState::ParkingFromMovingRight:
										is_not_moving = false;
										if (Dismount()) {
											hasExitedLift = true;
											targetPosition.y = current_position.y;
										}
										break;
									}
								}
							}
							else {
								is_not_moving = false;
							}
							if (movement_progress >= 1.0f) {
								SetCurrentPosition(current_position - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
								current_position = GetCurrentPosition();
								hasExitedLift = false;
								toBeRenderedAtOffset = Vector2<float>();
							}
							else {
								toBeRenderedAtOffset = Vector2<float>(-movement_progress, 0.0f);
							}

						}
					}
				}
				else if (remaining_movement.x > 0) {
					Vector2<size_t> right_position(current_position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
					garden->InteractAt(right_position, *this);
					if (!garden->IsSolidAt(right_position)) {
						is_not_moving = false;
						if (mounted_at_entity) {
							if (shared_ptr<LiftEntityScript> mounted_at_lift_entity = dynamic_pointer_cast<LiftEntityScript>(mounted_at_entity)) {
								switch (mounted_at_lift_entity->GetLiftMovementState()) {
								case ELiftMovementState::Up:
								case ELiftMovementState::Down:
								case ELiftMovementState::Left:
								case ELiftMovementState::Right:
									break;
								case ELiftMovementState::ParkingFromMovingUp:
								case ELiftMovementState::ParkingFromMovingDown:
								case ELiftMovementState::ParkingFromMovingLeft:
								case ELiftMovementState::ParkingFromMovingRight:
									is_not_moving = false;
									if (Dismount()) {
										hasExitedLift = true;
										targetPosition.y = current_position.y;
									}
									break;
								}
							}
						}
						else {
							is_not_moving = false;
						}
						if (movement_progress >= 1.0f) {
							SetCurrentPosition(current_position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
							current_position = GetCurrentPosition();
							hasExitedLift = false;
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
				targetPosition = current_position;
				toBeRenderedAtOffset = Vector2<float>();
			}
			else if (movementProgress >= 1.0f) {
				movementProgress = max(movementProgress - 1.0f, 0.0f);
				if ((movement_state == EHumanoidMovementState::Falling) && (targetPosition == (current_position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0))))) {
					targetPosition = current_position;
					toBeRenderedAtOffset = Vector2<float>();
				}
			}
		} while (movementProgress >= 1.0f);
	}
}

void HumanoidEntityScript::UpdateMovementState(EHumanoidMovementState movementState) {
	if (this->movementState == movementState) {
		return;
	}
	EHumanoidMovementState old_movement_state(this->movementState);
	this->movementState = movementState;
	switch (old_movement_state) {
		case EHumanoidMovementState::Standing:
			switch (movementState) {
			case EHumanoidMovementState::Standing:
				break;
			case EHumanoidMovementState::Walking:
				OnWalkingStarted();
				break;
			case EHumanoidMovementState::Climbing:
				OnClimbingStarted();
				break;
			case EHumanoidMovementState::Falling:
				OnFallingStarted();
				break;
			case EHumanoidMovementState::Mounted:
				OnMounted();
				break;
			}
			break;
		case EHumanoidMovementState::Walking:
			switch (movementState) {
			case EHumanoidMovementState::Standing:
				OnWalkingFinished();
				break;
			case EHumanoidMovementState::Walking:
				break;
			case EHumanoidMovementState::Climbing:
				OnWalkingFinished();
				OnClimbingStarted();
				break;
			case EHumanoidMovementState::Falling:
				OnWalkingFinished();
				OnFallingStarted();
				break;
			case EHumanoidMovementState::Mounted:
				OnWalkingFinished();
				OnMounted();
				break;
			}
			break;
		case EHumanoidMovementState::Climbing:
			switch (movementState) {
			case EHumanoidMovementState::Standing:
				OnClimbingFinished();
				break;
			case EHumanoidMovementState::Walking:
				OnClimbingFinished();
				OnWalkingStarted();
				break;
			case EHumanoidMovementState::Climbing:
				break;
			case EHumanoidMovementState::Falling:
				OnClimbingFinished();
				OnFallingStarted();
				break;
			case EHumanoidMovementState::Mounted:
				OnClimbingFinished();
				OnMounted();
				break;
			}
			break;
		case EHumanoidMovementState::Falling:
			switch (movementState) {
			case EHumanoidMovementState::Standing:
				OnFallingFinished();
				break;
			case EHumanoidMovementState::Walking:
				OnFallingFinished();
				OnWalkingStarted();
				break;
			case EHumanoidMovementState::Climbing:
				OnFallingFinished();
				OnClimbingStarted();
				break;
			case EHumanoidMovementState::Falling:
				break;
			case EHumanoidMovementState::Mounted:
				OnFallingFinished();
				OnMounted();
				break;
			}
			break;
		case EHumanoidMovementState::Mounted:
			switch (movementState) {
			case EHumanoidMovementState::Standing:
				OnDismounted();
				break;
			case EHumanoidMovementState::Walking:
				OnDismounted();
				OnWalkingStarted();
				break;
			case EHumanoidMovementState::Climbing:
				OnDismounted();
				OnClimbingStarted();
				break;
			case EHumanoidMovementState::Falling:
				OnDismounted();
				OnFallingStarted();
				break;
			case EHumanoidMovementState::Mounted:
				break;
			}
			break;
	}
}
