#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Easing.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Entities/EGardenEntityType.hpp>
#include <OGP/Entities/ELiftMovementState.hpp>
#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/LiftEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::SceneManagement;

using namespace OGP::Entities;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

constexpr const float maximalMovementSpeed(80.0f / 8.0f);
constexpr const float maximalParkingTime(1.0f);

LiftEntityScript::LiftEntityScript(Node* node) :
	EntityScript(node),
	liftMovementState(ELiftMovementState::Up),
	movementProgress(0.0f),
	parkingProgress(0.0f),
	hasStartedToMove(true),
	isFinishingToMove(false) {
	// ...
}

Vector2<float> LiftEntityScript::GetToBeRenderedPosition() const noexcept {
	Vector2<float> ret(EntityScript::GetToBeRenderedPosition());
	switch (liftMovementState) {
	case ELiftMovementState::Up:
		ret += Vector2<float>(0.0f, GetAnimatedMovementProgress());
		break;
	case ELiftMovementState::Down:
		ret += Vector2<float>(0.0f, -GetAnimatedMovementProgress());
		break;
	case ELiftMovementState::Left:
		ret += Vector2<float>(-GetAnimatedMovementProgress(), 0.0f);
		break;
	case ELiftMovementState::Right:
		ret += Vector2<float>(GetAnimatedMovementProgress(), 0.0f);
		break;
	default:
		break;
	}
	return ret;
}

void LiftEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	switch (gardenEntityData.type) {
	case EGardenEntityType::UpMovingLift:
		liftMovementState = ELiftMovementState::Up;
		break;
	case EGardenEntityType::DownMovingLift:
		liftMovementState = ELiftMovementState::Down;
		break;
	case EGardenEntityType::LeftMovingLift:
		liftMovementState = ELiftMovementState::Left;
		break;
	case EGardenEntityType::RightMovingLift:
		liftMovementState = ELiftMovementState::Right;
		break;
	default:
		break;
	}
	movementProgress = 0.0f;
	parkingProgress = 0.0f;
	EntityScript::Spawn(gardenEntityData, garden);
}

bool LiftEntityScript::Interact(EntityScript& sourceEntity) {
	bool ret(false);
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		shared_ptr<EntityScript> lift_entity;
		ret = (GetCurrentPosition() == sourceEntity.GetCurrentPosition()) && ((liftMovementState == ELiftMovementState::ParkingFromMovingUp) || (liftMovementState == ELiftMovementState::ParkingFromMovingDown) || (liftMovementState == ELiftMovementState::ParkingFromMovingLeft) || (liftMovementState == ELiftMovementState::ParkingFromMovingRight)) && garden->TryGettingEntity(*this, lift_entity) && sourceEntity.MountAt(lift_entity);
	}
	return ret;
}

void LiftEntityScript::OnGameTick(Engine& engine, high_resolution_clock::duration deltaTime) {
	movementProgress += duration<float>(deltaTime).count() * maximalMovementSpeed;
	parkingProgress += duration<float>(deltaTime).count();
	do {
		if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
			switch (liftMovementState) {
			case ELiftMovementState::Up:
				parkingProgress = 0.0f;
				if (IsAtTopBound(*garden)) {
					ParkFromMovingUp();
				}
				else {
					Vector2<size_t> target_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
					if (garden->IsSolidAt(target_position)) {
						ParkFromMovingUp();
					}
					else {
						float movement_progress(min(movementProgress, 1.0f));
						if (movement_progress >= 1.0f) {
							hasStartedToMove = false;
							SetCurrentPosition(target_position);
							if (IsAtTopBound(*garden) || garden->IsSolidAt(target_position)) {
								ParkFromMovingUp();
							}
							else {
								isFinishingToMove = IsMovingUpReachesEnd(*garden);
							}
						}
					}
				}
				break;
			case ELiftMovementState::Down:
				parkingProgress = 0.0f;
				if (IsAtBottomBound(*garden)) {
					ParkFromMovingDown();
				}
				else {
					if (GetCurrentPosition().y > static_cast<size_t>(1)) {
						Vector2<size_t> target_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
						if (garden->IsSolidAt(target_position)) {
							ParkFromMovingDown();
						}
						else {
							float movement_progress(min(movementProgress, 1.0f));
							if (movement_progress >= 1.0f) {
								hasStartedToMove = false;
								SetCurrentPosition(target_position);
								if (IsAtBottomBound(*garden) || garden->IsSolidAt(target_position)) {
									ParkFromMovingDown();
								}
								else {
									isFinishingToMove = IsMovingDownReachesEnd(*garden);
								}
							}
						}
					}
					else {
						ParkFromMovingDown();
					}
				}
				break;
			case ELiftMovementState::Left:
				parkingProgress = 0.0f;
				if (IsAtLeftBound(*garden)) {
					ParkFromMovingLeft();
				}
				else {
					if (GetCurrentPosition().x > static_cast<size_t>(1)) {
						Vector2<size_t> target_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
						if (garden->IsSolidAt(target_position)) {
							ParkFromMovingLeft();
						}
						else {
							float movement_progress(min(movementProgress, 1.0f));
							if (movement_progress >= 1.0f) {
								hasStartedToMove = false;
								SetCurrentPosition(target_position);
								if (IsAtLeftBound(*garden) || garden->IsSolidAt(target_position)) {
									ParkFromMovingLeft();
								}
								else {
									isFinishingToMove = IsMovingLeftReachesEnd(*garden);
								}
							}
						}
					}
					else {
						ParkFromMovingLeft();
					}
				}
				break;
			case ELiftMovementState::Right:
				parkingProgress = 0.0f;
				if (IsAtRightBound(*garden)) {
					ParkFromMovingRight();
				}
				else {
					Vector2<size_t> target_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
					if (garden->IsSolidAt(target_position)) {
						ParkFromMovingRight();
					}
					else {
						float movement_progress(min(movementProgress, 1.0f));
						if (movement_progress >= 1.0f) {
							hasStartedToMove = false;
							SetCurrentPosition(target_position);
							if (IsAtRightBound(*garden) || garden->IsSolidAt(target_position)) {
								ParkFromMovingRight();
							}
							else {
								isFinishingToMove = IsMovingRightReachesEnd(*garden);
							}
						}
					}
				}
				break;
			case ELiftMovementState::ParkingFromMovingUp:
				movementProgress = 0.0f;
				if (parkingProgress >= maximalParkingTime) {
					if (IsAtTopBound(*garden)) {
						if (!SwitchToMovingLeft(*garden)) {
							if (IsAtLeftBound(*garden)) {
								if (!SwitchToMovingDown(*garden)) {
									if (IsAtBottomBound(*garden)) {
										SwitchToMovingRight(*garden);
									}
									else {
										SwitchToMovingUp(*garden);
									}
								}
							}
							else {
								SwitchToMovingRight(*garden);
							}
						}
					}
					else if (!SwitchToMovingDown(*garden)) {
						if (IsAtBottomBound(*garden)) {
							if (!SwitchToMovingRight(*garden)) {
								if (IsAtRightBound(*garden)) {
									SwitchToMovingUp(*garden);
								}
								else {
									SwitchToMovingLeft(*garden);
								}
							}
						}
						else {
							SwitchToMovingUp(*garden);
						}
					}
				}
				break;
			case ELiftMovementState::ParkingFromMovingDown:
				movementProgress = 0.0f;
				if (parkingProgress >= maximalParkingTime) {
					if (IsAtBottomBound(*garden)) {
						if (!SwitchToMovingRight(*garden)) {
							if (IsAtRightBound(*garden)) {
								if (!SwitchToMovingUp(*garden)) {
									if (IsAtTopBound(*garden)) {
										SwitchToMovingLeft(*garden);
									}
									else {
										SwitchToMovingDown(*garden);
									}
								}
							}
							else {
								SwitchToMovingLeft(*garden);
							}
						}
					}
					else if (!SwitchToMovingUp(*garden)) {
						if (IsAtTopBound(*garden)) {
							if (!SwitchToMovingLeft(*garden)) {
								if (IsAtLeftBound(*garden)) {
									SwitchToMovingDown(*garden);
								}
								else {
									SwitchToMovingRight(*garden);
								}
							}
						}
						else {
							SwitchToMovingDown(*garden);
						}
					}
				}
				break;
			case ELiftMovementState::ParkingFromMovingLeft:
				movementProgress = 0.0f;
				if (parkingProgress >= maximalParkingTime) {
					if (IsAtLeftBound(*garden)) {
						if (!SwitchToMovingDown(*garden)) {
							if (IsAtBottomBound(*garden)) {
								if (!SwitchToMovingRight(*garden)) {
									if (IsAtRightBound(*garden)) {
										SwitchToMovingUp(*garden);
									}
									else {
										SwitchToMovingLeft(*garden);
									}
								}
							}
							else {
								SwitchToMovingUp(*garden);
							}
						}
					}
					else if (!SwitchToMovingRight(*garden)) {
						if (IsAtRightBound(*garden)) {
							if (!SwitchToMovingUp(*garden)) {
								if (IsAtTopBound(*garden)) {
									SwitchToMovingLeft(*garden);
								}
								else {
									SwitchToMovingDown(*garden);
								}
							}
						}
						else {
							SwitchToMovingLeft(*garden);
						}
					}
				}
				break;
			case ELiftMovementState::ParkingFromMovingRight:
				movementProgress = 0.0f;
				if (parkingProgress >= maximalParkingTime) {
					if (IsAtRightBound(*garden)) {
						if (!SwitchToMovingUp(*garden)) {
							if (IsAtTopBound(*garden)) {
								if (!SwitchToMovingLeft(*garden)) {
									if (IsAtLeftBound(*garden)) {
										SwitchToMovingDown(*garden);
									}
									else {
										SwitchToMovingRight(*garden);
									}
								}
							}
							else {
								SwitchToMovingDown(*garden);
							}
						}
					}
					else if (!SwitchToMovingLeft(*garden)) {
						if (IsAtLeftBound(*garden)) {
							if (!SwitchToMovingDown(*garden)) {
								if (IsAtBottomBound(*garden)) {
									SwitchToMovingRight(*garden);
								}
								else {
									SwitchToMovingUp(*garden);
								}
							}
						}
						else {
							SwitchToMovingRight(*garden);
						}
					}
				}
				break;
			}
		}
		if (movementProgress >= 1.0f) {
			movementProgress = max(movementProgress - 1.0f, 0.0f);
		}
	} while (movementProgress >= 1.0f);
}

bool LiftEntityScript::IsAtTopBound(const GardenScript& garden) const noexcept {
	return IsAtTopBound(garden, GetCurrentPosition().y);
}

bool LiftEntityScript::IsAtTopBound(const GardenScript& garden, size_t targetYPosition) const noexcept {
	return targetYPosition == GetGardenEntityData().bounds.top;
}

bool LiftEntityScript::IsAtBottomBound(const GardenScript& garden) const noexcept {
	return IsAtBottomBound(garden, GetCurrentPosition().y);
}

bool LiftEntityScript::IsAtBottomBound(const GardenScript& garden, size_t targetYPosition) const noexcept {
	return targetYPosition == GetGardenEntityData().bounds.bottom;
}

bool LiftEntityScript::IsAtLeftBound(const GardenScript& garden) const noexcept {
	return IsAtLeftBound(garden, GetCurrentPosition().x);
}

bool LiftEntityScript::IsAtLeftBound(const GardenScript& garden, size_t targetXPosition) const noexcept {
	return targetXPosition == GetGardenEntityData().bounds.left;
}

bool LiftEntityScript::IsAtRightBound(const GardenScript& garden) const noexcept {
	return IsAtRightBound(garden, GetCurrentPosition().x);
}

bool LiftEntityScript::IsAtRightBound(const GardenScript& garden, size_t targetXPosition) const noexcept {
	return targetXPosition == GetGardenEntityData().bounds.right;
}

bool LiftEntityScript::SwitchToMovingUp(const GardenScript& garden) noexcept {
	bool ret(false);
	Vector2<size_t> target_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
	if (GetGardenEntityData().bounds.IsContained(target_position.GetConverted<int64_t>()) && !garden.IsSolidAt(target_position)) {
		liftMovementState = ELiftMovementState::Up;
		SwitchToMoving();
		isFinishingToMove = IsMovingUpReachesEnd(garden);
		ret = true;
	}
	return ret;
}

bool LiftEntityScript::SwitchToMovingDown(const GardenScript& garden) noexcept {
	bool ret(false);
	if (GetCurrentPosition().y > static_cast<size_t>(0)) {
		Vector2<size_t> target_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
		if (GetGardenEntityData().bounds.IsContained(target_position.GetConverted<int64_t>()) && !garden.IsSolidAt(target_position)) {
			liftMovementState = ELiftMovementState::Down;
			SwitchToMoving();
			isFinishingToMove = IsMovingDownReachesEnd(garden);
			ret = true;
		}
	}
	return ret;
}

bool LiftEntityScript::SwitchToMovingLeft(const GardenScript& garden) noexcept {
	bool ret(false);
	if (GetCurrentPosition().x > static_cast<size_t>(0)) {
		Vector2<size_t> target_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
		if (GetGardenEntityData().bounds.IsContained(target_position.GetConverted<int64_t>()) && !garden.IsSolidAt(target_position)) {
			liftMovementState = ELiftMovementState::Left;
			SwitchToMoving();
			isFinishingToMove = IsMovingLeftReachesEnd(garden);
			ret = true;
		}
	}
	return ret;
}

bool LiftEntityScript::SwitchToMovingRight(const GardenScript& garden) noexcept {
	bool ret(false);
	Vector2<size_t> target_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
	if (GetGardenEntityData().bounds.IsContained(target_position.GetConverted<int64_t>()) && !garden.IsSolidAt(target_position)) {
		liftMovementState = ELiftMovementState::Right;
		SwitchToMoving();
		isFinishingToMove = IsMovingRightReachesEnd(garden);
		ret = true;
	}
	return ret;
}

void LiftEntityScript::SwitchToMoving() noexcept {
	movementProgress = (parkingProgress - 1.0) * maximalMovementSpeed;
	parkingProgress = 0.0f;
	hasStartedToMove = true;
}

bool LiftEntityScript::IsMovingUpReachesEnd(const GardenScript& garden) const noexcept {
	Vector2<size_t> target_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(2)));
	return IsAtTopBound(garden, GetCurrentPosition().y + static_cast<size_t>(1)) || garden.IsSolidAt(target_position);
}

bool LiftEntityScript::IsMovingDownReachesEnd(const GardenScript& garden) const noexcept {
	bool ret(true);
	if (GetCurrentPosition().y > static_cast<size_t>(1)) {
		Vector2<size_t> target_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(2)));
		ret = IsAtBottomBound(garden, GetCurrentPosition().y - static_cast<size_t>(1)) || garden.IsSolidAt(target_position);
	}
	return ret;
}

bool LiftEntityScript::IsMovingLeftReachesEnd(const GardenScript& garden) const noexcept {
	bool ret(true);
	if (GetCurrentPosition().x > static_cast<size_t>(1)) {
		Vector2<size_t> target_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(2), static_cast<size_t>(0)));
		ret = IsAtLeftBound(garden, GetCurrentPosition().x - static_cast<size_t>(1)) || garden.IsSolidAt(target_position);
	}
	return ret;
}

bool LiftEntityScript::IsMovingRightReachesEnd(const GardenScript& garden) const noexcept {
	Vector2<size_t> target_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(2), static_cast<size_t>(0)));
	return IsAtRightBound(garden, GetCurrentPosition().x + static_cast<size_t>(1)) || garden.IsSolidAt(target_position);
}

void LiftEntityScript::ParkFromMovingUp() noexcept {
	liftMovementState = ELiftMovementState::ParkingFromMovingUp;
	Park();
}

void LiftEntityScript::ParkFromMovingDown() noexcept {
	liftMovementState = ELiftMovementState::ParkingFromMovingDown;
	Park();
}

void LiftEntityScript::ParkFromMovingLeft() noexcept {
	liftMovementState = ELiftMovementState::ParkingFromMovingLeft;
	Park();
}

void LiftEntityScript::ParkFromMovingRight() noexcept {
	liftMovementState = ELiftMovementState::ParkingFromMovingRight;
	Park();
}

void LiftEntityScript::Park() noexcept {
	parkingProgress = min(movementProgress - 1.0f, 0.0f) / maximalMovementSpeed;
	movementProgress = 0.0f;
	isFinishingToMove = false;
}

float LiftEntityScript::GetAnimatedMovementProgress() const noexcept {
	return hasStartedToMove ? (isFinishingToMove ? Easing::EaseInOut(movementProgress) : Easing::EaseIn(movementProgress)) : (isFinishingToMove ? Easing::EaseOut(movementProgress) : movementProgress);
}
