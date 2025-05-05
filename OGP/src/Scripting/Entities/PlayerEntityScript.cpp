#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Hashing/StringHash.hpp>
#include <Klein/Math/Rectangle.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/Physics/Intersection.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Physics/AABBColliderScript.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Environment/EKillerType.hpp>
#include <OGP/Scripting/Entities/HumanoidEntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Hashing;
using namespace Klein::Math;
using namespace Klein::Physics;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Physics;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Entities;
using namespace OGP::Environment;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

const StringHash wKeyStringHash("Keyboard.KeyCode.87");
const StringHash aKeyStringHash("Keyboard.KeyCode.65");
const StringHash sKeyStringHash("Keyboard.KeyCode.83");
const StringHash dKeyStringHash("Keyboard.KeyCode.68");
const StringHash qKeyStringHash("Keyboard.KeyCode.81");
const StringHash eKeyStringHash("Keyboard.KeyCode.69");

PlayerEntityScript::PlayerEntityScript(Node* node) :
	HumanoidEntityScript(node),
	isAlive(true),
	hasNotWonYet(true),
	score(static_cast<size_t>(0)),
	redKeyCount(static_cast<size_t>(0)),
	yellowKeyCount(static_cast<size_t>(0)),
	greenKeyCount(static_cast<size_t>(0)),
	remainingGarlicEffectTime(high_resolution_clock::duration::zero()),
	remainingMushroomEffectTime(high_resolution_clock::duration::zero()) {
	if (shared_ptr<SpriteRendererScript> sprite_renderer = GetSpriteRenderer().lock()) {
		sprite_renderer->SetLayerIndex(0U);
	}
	shared_ptr<AABBColliderScript> collider(GetNode().CreateNewChild()->EnsureScript<AABBColliderScript>());
	collider->SetLocalCollisionRectangle(Rectangle<float>(Vector2<float>(), Vector2<float>(0.5f, 0.875f)));
	this->collider = collider;
}

HumanoidInput PlayerEntityScript::GetInput(const Engine& engine) {
	HumanoidInput ret(HumanoidEntityScript::GetInput(engine));
	for (const auto& input_event : engine.GetCurrentInputEvents()) {
		if (input_event.GetNameHash() == wKeyStringHash) {
			ret.isWalkingUp = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == aKeyStringHash) {
			ret.isWalkingLeft = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == sKeyStringHash) {
			ret.isWalkingDown = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == dKeyStringHash) {
			ret.isWalkingRight = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == qKeyStringHash) {
			ret.isDiggingLeft = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == eKeyStringHash) {
			ret.isDiggingRight = input_event.IsPressing();
		}
	}
	return ret;
}

bool PlayerEntityScript::IsAlive() const noexcept {
	return isAlive;
}

bool PlayerEntityScript::Kill(EKillerType killerType) {
	bool ret(isAlive && ((killerType != EKillerType::Entity) || !IsMushroomEffectActive()));
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

size_t PlayerEntityScript::GetScore() const noexcept {
	return score;
}

void PlayerEntityScript::SetScore(size_t score) noexcept {
	if (this->score != score) {
		this->score = score;
		OnScoreChanged(score);
	}
}

void PlayerEntityScript::AddScore(size_t score) noexcept {
	SetScore(this->score + score);
}

size_t PlayerEntityScript::GetRedKeyCount() const noexcept {
	return redKeyCount;
}

size_t PlayerEntityScript::GetYellowKeyCount() const noexcept {
	return yellowKeyCount;
}

size_t PlayerEntityScript::GetGreenKeyCount() const noexcept {
	return greenKeyCount;
}

void PlayerEntityScript::AddRedKey() noexcept {
	++redKeyCount;
	OnRedKeyCollected();
}

bool PlayerEntityScript::UseRedKey() noexcept {
	bool ret(redKeyCount > static_cast<size_t>(0));
	if (ret) {
		--redKeyCount;
		OnRedKeyUsed();
	}
	return ret;
}

void PlayerEntityScript::AddYellowKey() noexcept {
	++yellowKeyCount;
	OnYellowKeyCollected();
}

bool PlayerEntityScript::UseYellowKey() noexcept {
	bool ret(yellowKeyCount > static_cast<size_t>(0));
	if (ret) {
		--yellowKeyCount;
		OnYellowKeyUsed();
	}
	return ret;
}

void PlayerEntityScript::AddGreenKey() noexcept {
	++greenKeyCount;
	OnGreenKeyCollected();
}

bool PlayerEntityScript::UseGreenKey() noexcept {
	bool ret(greenKeyCount > static_cast<size_t>(0));
	if (ret) {
		--greenKeyCount;
		OnGreenKeyUsed();
	}
	return ret;
}

high_resolution_clock::duration PlayerEntityScript::GetRemainingGarlicEffectTime() const noexcept {
	return remainingGarlicEffectTime;
}

high_resolution_clock::duration PlayerEntityScript::GetRemainingMushroomEffectTime() const noexcept {
	return remainingMushroomEffectTime;
}

bool PlayerEntityScript::IsGarlicEffectActive() const noexcept {
	return remainingGarlicEffectTime > high_resolution_clock::duration::zero();
}

void PlayerEntityScript::ActivateGarlicEffect() noexcept {
	remainingGarlicEffectTime = 10s;
	OnGarlicEffectActivated();
}

bool PlayerEntityScript::IsMushroomEffectActive() const noexcept {
	return remainingMushroomEffectTime > high_resolution_clock::duration::zero();
}

void PlayerEntityScript::ActivateMushroomEffect() noexcept {
	remainingMushroomEffectTime = 10s;
	OnMushroomEffectActivated();
}

void PlayerEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	isAlive = true;
	hasNotWonYet = true;
	HumanoidEntityScript::Spawn(gardenEntityData, garden);
}

void PlayerEntityScript::OnGameTick(Engine& engine, high_resolution_clock::duration deltaTime) {
	HumanoidEntityScript::OnGameTick(engine, deltaTime);
	if (isAlive) {
		if (remainingGarlicEffectTime > high_resolution_clock::duration::zero()) {
			remainingGarlicEffectTime -= deltaTime;
			if (remainingGarlicEffectTime <= high_resolution_clock::duration::zero()) {
				remainingGarlicEffectTime = high_resolution_clock::duration::zero();
				OnGarlicEffectDeactivated();
			}
		}
		if (remainingMushroomEffectTime > high_resolution_clock::duration::zero()) {
			remainingMushroomEffectTime -= deltaTime;
			if (remainingMushroomEffectTime <= high_resolution_clock::duration::zero()) {
				remainingMushroomEffectTime = high_resolution_clock::duration::zero();
				OnMushroomEffectDeactivated();
			}
		}
		if (shared_ptr<AABBColliderScript> collider = this->collider.lock()) {
			collider->EnumerateIntersections(
				[this](Intersection intersection) {
					Node* parent(intersection.destinationCollider->GetNode().GetParent());
					if (parent) {
						shared_ptr<EntityScript> entity;
						if (parent->TryGettingScript<EntityScript>(entity) && entity->IsDeadly()) {
							Kill(EKillerType::Entity);
						}
					}
				}
			);
		}
	}
}
