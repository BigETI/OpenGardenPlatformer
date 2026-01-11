#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Easing.hpp>
#include <Klein/Math/Rectangle.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Physics/AABBColliderScript.hpp>

#include <OGP/Entities/EWormMovementState.hpp>
#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Environment/EGardenState.hpp>
#include <OGP/Environment/GlobalWorld.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Entities/WormEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Physics;

using namespace OGP::Entities;
using namespace OGP::Environment;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

constexpr const float maximalMovementTickCount(16.0f);
constexpr const float maximalTurningTickCount(6.0f);

WormEntityScript::WormEntityScript(Node* node) :
	EntityScript(node),
	wormMovementState(EWormMovementState::Left),
	elapsedTime(high_resolution_clock::duration::zero()),
	hasStartedToMove(true),
	isFinishingToMove(false) {
	shared_ptr<AABBColliderScript> collider(GetNode().CreateNewChild()->EnsureScript<AABBColliderScript>());
	collider->SetLocalCollisionRectangle(Rectangle<float>(Vector2<float>(0.0f, -0.375f), Vector2<float>(1.0f, 0.25f)));
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
	case EWormMovementState::TurningLeft:
		break;
	case EWormMovementState::TurningRight:
		break;
	}
	return ret;
}

bool WormEntityScript::IsDeadly() const noexcept {
	return true;
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
	elapsedTime = high_resolution_clock::duration::zero();
	hasStartedToMove = true;
	EntityScript::Spawn(gardenEntityData, garden);
}

void WormEntityScript::OnGameTick(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		if (garden->GetGardenState() != EGardenState::Playing) {
			return;
		}
		elapsedTime += deltaTime;
		bool is_repeating_loop(true);
		while (is_repeating_loop) {
			is_repeating_loop = false;
			high_resolution_clock::duration maximal_time;
			switch (wormMovementState) {
			case EWormMovementState::Left:
				if (IsLeftMovable(*garden)) {
					maximal_time = GlobalWorld::GetDuration(maximalMovementTickCount);
					if (elapsedTime >= maximal_time) {
						elapsedTime -= maximal_time;
						is_repeating_loop = true;
						SetCurrentPosition(GetCurrentPosition() - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
						if (IsLeftMovable(*garden)) {
							hasStartedToMove = false;
							isFinishingToMove = IsMovingLeftReachesEnd(*garden);
						}
						else {
							wormMovementState = EWormMovementState::TurningRight;
						}
					}
				}
				else {
					is_repeating_loop = true;
					wormMovementState = EWormMovementState::TurningRight;
				}
				break;
			case EWormMovementState::Right:
				if (IsRightMovable(*garden)) {
					maximal_time = GlobalWorld::GetDuration(maximalMovementTickCount);
					if (elapsedTime >= maximal_time) {
						elapsedTime -= maximal_time;
						is_repeating_loop = true;
						SetCurrentPosition(GetCurrentPosition() + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)));
						if (IsRightMovable(*garden)) {
							hasStartedToMove = false;
							isFinishingToMove = IsMovingRightReachesEnd(*garden);
						}
						else {
							wormMovementState = EWormMovementState::TurningLeft;
						}
					}
				}
				else {
					is_repeating_loop = true;
					wormMovementState = EWormMovementState::TurningLeft;
				}
				break;
			case EWormMovementState::TurningLeft:
				maximal_time = GlobalWorld::GetDuration(maximalTurningTickCount);
				if (elapsedTime >= maximal_time) {
					elapsedTime -= maximal_time;
					is_repeating_loop = true;
					if (IsLeftMovable(*garden)) {
						hasStartedToMove = true;
						isFinishingToMove = IsMovingLeftReachesEnd(*garden);
						wormMovementState = EWormMovementState::Left;
					}
					else {
						wormMovementState = EWormMovementState::TurningRight;
					}
				}
				break;
			case EWormMovementState::TurningRight:
				maximal_time = GlobalWorld::GetDuration(maximalTurningTickCount);
				if (elapsedTime >= maximal_time) {
					elapsedTime -= maximal_time;
					is_repeating_loop = true;
					if (IsRightMovable(*garden)) {
						hasStartedToMove = true;
						isFinishingToMove = IsMovingRightReachesEnd(*garden);
						wormMovementState = EWormMovementState::Right;
					}
					else {
						wormMovementState = EWormMovementState::TurningLeft;
					}
				}
				break;
			}
		}
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
	high_resolution_clock::duration maximal_movement_time(GlobalWorld::GetDuration(maximalMovementTickCount));
	float movement_progress(duration<float>(min(elapsedTime, maximal_movement_time)).count() / duration<float>(maximal_movement_time).count());
	return hasStartedToMove ? (isFinishingToMove ? Easing::GetEasedInOut(movement_progress) : Easing::GetEasedIn(movement_progress)) : (isFinishingToMove ? Easing::GetEasedOut(movement_progress) : movement_progress);
}
