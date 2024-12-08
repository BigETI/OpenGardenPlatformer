#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Easing.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Entities/EWormMovementState.hpp>
#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Entities/WormEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::SceneManagement;

using namespace OGP::Entities;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

constexpr const float maximalMovementSpeed(20.0f / 8.0f);

WormEntityScript::WormEntityScript(Node* node) :
	EntityScript(node),
	wormMovementState(EWormMovementState::Left),
	movementProgress(0.0f),
	hasStartedToMove(true),
	isFinishingToMove(false) {
	// ...
}

Vector2<float> WormEntityScript::GetToBeRenderedPosition() const noexcept {
	Vector2<float> ret(EntityScript::GetToBeRenderedPosition());
	switch (wormMovementState) {
	case EWormMovementState::Left:
		ret -= Vector2<float>(GetAnimatedMovementProgress(), 0.0f);
		break;
	case EWormMovementState::Right:
		ret += Vector2<float>(GetAnimatedMovementProgress(), 0.0f);
		break;
	}
	return ret;
}

void WormEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	switch (gardenEntityData.type) {
	case EGardenEntityType::LeftMovingWorm:
		wormMovementState = EWormMovementState::Left;
		isFinishingToMove = IsMovingLeftReachesEnd(*garden);
		break;
	case EGardenEntityType::RightMovingWorm:
		wormMovementState = EWormMovementState::Right;
		isFinishingToMove = IsMovingRightReachesEnd(*garden);
		break;
	default:
		break;
	}
	movementProgress = 0.0f;
	hasStartedToMove = true;
	EntityScript::Spawn(gardenEntityData, garden);
}

bool WormEntityScript::Interact(EntityScript& sourceEntity) {
	bool ret(false);
	if (sourceEntity.GetCurrentPosition() == GetCurrentPosition()) {
		if (PlayerEntityScript* player_entity = dynamic_cast<PlayerEntityScript*>(&sourceEntity)) {
			ret = player_entity->Kill();
		}
	}
	return ret;
}

void WormEntityScript::OnGameTick(Engine& engine, high_resolution_clock::duration deltaTime) {
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		movementProgress += duration<float>(deltaTime).count() * maximalMovementSpeed;
		do {
			switch (wormMovementState) {
			case EWormMovementState::Left:
				if (IsLeftMovable(*garden)) {
					if (movementProgress >= 1.0f) {
						SetCurrentPosition(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
						if (IsLeftMovable(*garden)) {
							hasStartedToMove = false;
							isFinishingToMove = IsMovingLeftReachesEnd(*garden);
						}
						else {
							wormMovementState = EWormMovementState::Right;
							hasStartedToMove = true;
							isFinishingToMove = IsMovingRightReachesEnd(*garden);
						}
					}
				}
				else {
					wormMovementState = EWormMovementState::Right;
				}
				break;
			case EWormMovementState::Right:
				if (IsRightMovable(*garden)) {
					if (movementProgress >= 1.0f) {
						SetCurrentPosition(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
						if (IsRightMovable(*garden)) {
							hasStartedToMove = false;
							isFinishingToMove = IsMovingRightReachesEnd(*garden);
						}
						else {
							wormMovementState = EWormMovementState::Left;
							hasStartedToMove = true;
							isFinishingToMove = IsMovingLeftReachesEnd(*garden);
						}
					}
				}
				else {
					wormMovementState = EWormMovementState::Left;
					hasStartedToMove = true;
					isFinishingToMove = IsMovingLeftReachesEnd(*garden);
				}
				break;
			}
			if (movementProgress >= 1.0f) {
				movementProgress -= 1.0f;
			}
		} while (movementProgress >= 1.0f);
	}
}

bool WormEntityScript::IsLeftMovable(GardenScript& garden) const noexcept {
	bool ret(false);
	if ((GetCurrentPosition().x > static_cast<size_t>(0)) && (GetCurrentPosition().x > GetGardenEntityData().bounds.left)) {
		Vector2<size_t> target_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
		ret = garden.IsWalkableAt(target_position) && !garden.IsSolidAt(target_position);
	}
	return ret;
}

bool WormEntityScript::IsRightMovable(GardenScript& garden) const noexcept {
	bool ret(false);
	if (GetCurrentPosition().x < GetGardenEntityData().bounds.right) {
		Vector2<size_t> target_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
		ret = garden.IsWalkableAt(target_position) && !garden.IsSolidAt(target_position);
	}
	return ret;
}

bool WormEntityScript::IsAtLeftBound(const GardenScript& garden) const noexcept {
	return IsAtLeftBound(garden, GetCurrentPosition().x);
}

bool WormEntityScript::IsAtLeftBound(const GardenScript& garden, size_t targetXPosition) const noexcept {
	return targetXPosition == GetGardenEntityData().bounds.left;
}

bool WormEntityScript::IsAtRightBound(const GardenScript& garden) const noexcept {
	return IsAtRightBound(garden, GetCurrentPosition().x);
}

bool WormEntityScript::IsAtRightBound(const GardenScript& garden, size_t targetXPosition) const noexcept {
	return targetXPosition == GetGardenEntityData().bounds.right;
}

bool WormEntityScript::IsMovingLeftReachesEnd(const GardenScript& garden) const noexcept {
	bool ret(true);
	if (GetCurrentPosition().x > static_cast<size_t>(1)) {
		Vector2<size_t> target_position(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(2), static_cast<size_t>(0)));
		ret = IsAtLeftBound(garden, GetCurrentPosition().x - static_cast<size_t>(1)) || garden.IsSolidAt(target_position);
	}
	return ret;
}

bool WormEntityScript::IsMovingRightReachesEnd(const GardenScript& garden) const noexcept {
	Vector2<size_t> target_position(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(2), static_cast<size_t>(0)));
	return IsAtRightBound(garden, GetCurrentPosition().x + static_cast<size_t>(1)) || garden.IsSolidAt(target_position);
}

float WormEntityScript::GetAnimatedMovementProgress() const noexcept {
	return hasStartedToMove ? (isFinishingToMove ? Easing::EaseInOut(movementProgress) : Easing::EaseIn(movementProgress)) : (isFinishingToMove ? Easing::EaseOut(movementProgress) : movementProgress);
}
