#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Hashing/StringHash.hpp>
#include <Klein/Math/Rectangle.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/Physics/Intersection.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Audio/AudioPlayerScript.hpp>
#include <Klein/Scripting/Physics/AABBColliderScript.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Scripting/Audio/SoundEffectsScript.hpp>
#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Environment/EKillerType.hpp>
#include <OGP/Environment/GlobalWorld.hpp>
#include <OGP/Scripting/Entities/HumanoidEntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Entities/QuestionMarkEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Hashing;
using namespace Klein::Math;
using namespace Klein::Physics;
using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Audio;
using namespace Klein::Scripting::Physics;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Entities;
using namespace OGP::Environment;
using namespace OGP::Scripting::Audio;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

static const ResourceID collectResourceID(string("SoundEffects/Collect.wav"));
static const StringHash upKeyboardKeyStringHash("Keyboard.KeyCode.265");
static const StringHash downKeyboardKeyStringHash("Keyboard.KeyCode.264");
static const StringHash leftKeyboardKeyStringHash("Keyboard.KeyCode.263");
static const StringHash rightKeyboardKeyStringHash("Keyboard.KeyCode.262");
static const StringHash aKeyboardKeyStringHash("Keyboard.KeyCode.65");
static const StringHash sKeyboardKeyStringHash("Keyboard.KeyCode.83");
static const StringHash enterKeyboardKeyStringHash("Keyboard.KeyCode.257");
static const StringHash numpadEnterKeyboardKeyStringHash("Keyboard.KeyCode.335");
static const StringHash f3KeyboardKeyStringHash("Keyboard.KeyCode.292");
static const string gamepadPrefix("Gamepad.");
static const string xAxisLeftGamepadStickSuffix(".0");
static const string yAxisLeftGamepadStickSuffix(".1");
static const string upGamepadButtonSuffix(".Button.1");
static const string downGamepadButtonSuffix(".Button.3");
static const string leftGamepadButtonSuffix(".Button.4");
static const string rightGamepadButtonSuffix(".Button.2");
static const string aGamepadButtonSuffix(".Button.7");
static const string bGamepadButtonSuffix(".Button.6");
static const string startGamepadButtonSuffix(".Button.15");
constexpr static const float analogPressValueThreshold(0.5f);

PlayerEntityScript::PlayerEntityScript(Node* node) :
	HumanoidEntityScript(node),
	isAlive(true),
	hasNotWonYet(true),
	score(static_cast<size_t>(0)),
	redKeyCount(static_cast<size_t>(0)),
	yellowKeyCount(static_cast<size_t>(0)),
	greenKeyCount(static_cast<size_t>(0)),
	remainingGarlicEffectTime(high_resolution_clock::duration::zero()),
	remainingMushroomEffectTime(high_resolution_clock::duration::zero()),
	isInQuestionMark(false) {
	if (shared_ptr<SpriteRendererScript> sprite_renderer = GetSpriteRenderer().lock()) {
		sprite_renderer->SetLayerIndex(2U);
	}
	shared_ptr<AABBColliderScript> collider(GetNode().CreateNewChild()->EnsureScript<AABBColliderScript>());
	collider->SetLocalCollisionRectangle(Rectangle<float>(Vector2<float>(), Vector2<float>(0.5f, 0.875f)));
	this->collider = collider;
	OnWalkingStarted += []() {
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("StartWalking");
		}
	};
	OnWalkingFinished += []() {
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("StopWalking");
		}
	};
	OnClimbingStarted += []() {
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("StartClimbing");
		}
	};
	OnClimbingFinished += []() {
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("StopClimbing");
		}
	};
	OnFallingStarted += []() {
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("Slip");
		}
	};
	OnFallingFinished += []() {
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("Land");
		}
	};
	OnMounted += []() {
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("Mount");
		}
	};
	OnDismounted += []() {
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("Dismount");
		}
	};
}

HumanoidInput PlayerEntityScript::GetInput(const Engine& engine) const noexcept {
	return keyboardInput.GetMergedWith(analogGamepadInput).GetMergedWith(digitalGamepadInput);
}

bool PlayerEntityScript::IsAlive() const noexcept {
	return isAlive;
}

bool PlayerEntityScript::Kill(EKillerType killerType) {
	bool ret(isAlive && ((killerType != EKillerType::Entity) || !IsMushroomEffectActive()));
	if (ret) {
		isAlive = false;
		if (SoundEffectsScript* global_sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			global_sound_effects->PlaySoundEffect("Die");
		}
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
	if (score > static_cast<size_t>(0)) {
		if (SoundEffectsScript* global_sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			global_sound_effects->PlaySoundEffect("Score");
		}
	}
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

const high_resolution_clock::duration& PlayerEntityScript::GetRemainingGarlicEffectTime() const noexcept {
	return remainingGarlicEffectTime;
}

const high_resolution_clock::duration& PlayerEntityScript::GetRemainingMushroomEffectTime() const noexcept {
	return remainingMushroomEffectTime;
}

bool PlayerEntityScript::IsGarlicEffectActive() const noexcept {
	return remainingGarlicEffectTime > high_resolution_clock::duration::zero();
}

void PlayerEntityScript::ActivateGarlicEffect() noexcept {
	remainingGarlicEffectTime = GlobalWorld::GetDuration(100.0f);
	OnGarlicEffectActivated();
}

bool PlayerEntityScript::IsMushroomEffectActive() const noexcept {
	return remainingMushroomEffectTime > high_resolution_clock::duration::zero();
}

void PlayerEntityScript::ActivateMushroomEffect() noexcept {
	remainingMushroomEffectTime = GlobalWorld::GetDuration(100.0f);
	OnMushroomEffectActivated();
}

void PlayerEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	isAlive = true;
	hasNotWonYet = true;
	HumanoidEntityScript::Spawn(gardenEntityData, garden);
}

void PlayerEntityScript::OnGameTick(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	bool is_skipping_questionmark(false);
	for (const auto& input_event : engine.GetCurrentInputEvents()) {
		if (input_event.GetNameHash() == upKeyboardKeyStringHash) {
			keyboardInput.isWalkingUp = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == downKeyboardKeyStringHash) {
			keyboardInput.isWalkingDown = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == leftKeyboardKeyStringHash) {
			keyboardInput.isWalkingLeft = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == rightKeyboardKeyStringHash) {
			keyboardInput.isWalkingRight = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == aKeyboardKeyStringHash) {
			keyboardInput.isDiggingLeft = input_event.IsPressing();
		}
		if (input_event.GetNameHash() == sKeyboardKeyStringHash) {
			keyboardInput.isDiggingRight = input_event.IsPressing();
		}
		if ((input_event.GetNameHash() == enterKeyboardKeyStringHash) || (input_event.GetNameHash() == numpadEnterKeyboardKeyStringHash)) {
			is_skipping_questionmark = input_event.IsPressing();
		}
		if ((input_event.GetNameHash() == f3KeyboardKeyStringHash) && input_event.IsPressing()) {
			Kill(EKillerType::User);
		}
		const string& input_event_name(input_event.GetNameHash().GetString());
		if (input_event_name.starts_with(gamepadPrefix)) {
			if (input_event_name.ends_with(xAxisLeftGamepadStickSuffix)) {
				float press_value(input_event.GetPressValue());
				analogGamepadInput.isWalkingLeft = press_value <= -analogPressValueThreshold;
				analogGamepadInput.isWalkingRight = press_value >= analogPressValueThreshold;
			}
			if (input_event_name.ends_with(yAxisLeftGamepadStickSuffix)) {
				float press_value(input_event.GetPressValue());
				analogGamepadInput.isWalkingUp = press_value <= -analogPressValueThreshold;
				analogGamepadInput.isWalkingDown = press_value >= analogPressValueThreshold;
			}
			if (input_event_name.ends_with(upGamepadButtonSuffix)) {
				digitalGamepadInput.isWalkingUp = input_event.IsPressing();
			}
			if (input_event_name.ends_with(downGamepadButtonSuffix)) {
				digitalGamepadInput.isWalkingDown = input_event.IsPressing();
			}
			if (input_event_name.ends_with(leftGamepadButtonSuffix)) {
				digitalGamepadInput.isWalkingLeft = input_event.IsPressing();
			}
			if (input_event_name.ends_with(rightGamepadButtonSuffix)) {
				digitalGamepadInput.isWalkingRight = input_event.IsPressing();
			}
			if (input_event_name.ends_with(aGamepadButtonSuffix)) {
				digitalGamepadInput.isDiggingLeft = input_event.IsPressing();
			}
			if (input_event_name.ends_with(bGamepadButtonSuffix)) {
				digitalGamepadInput.isDiggingRight = input_event.IsPressing();
			}
			if (input_event_name.ends_with(startGamepadButtonSuffix)) {
				is_skipping_questionmark = is_skipping_questionmark || input_event.IsPressing();
			}
		}
	}
	HumanoidEntityScript::OnGameTick(engine, deltaTime);
	if (isAlive) {
		if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
			switch (garden->GetGardenState()) {
			case EGardenState::Playing:
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
				if (isInQuestionMark) {
					isInQuestionMark = false;
					if (lastQuestionMarkPosition == GetCurrentPosition()) {
						vector<shared_ptr<EntityScript>> entities;
						for (const auto& entity : garden->GetEntitiesAt(GetCurrentPosition(), entities)) {
							if (shared_ptr<QuestionMarkEntityScript> question_mark_entity = dynamic_pointer_cast<QuestionMarkEntityScript>(entity)) {
								isInQuestionMark = true;
								break;
							}
						}
					}
				}
				if (!isInQuestionMark) {
					vector<shared_ptr<EntityScript>> entities;
					for (const auto& entity : garden->GetEntitiesAt(GetCurrentPosition(), entities)) {
						if (shared_ptr<QuestionMarkEntityScript> question_mark_entity = dynamic_pointer_cast<QuestionMarkEntityScript>(entity)) {
							isInQuestionMark = true;
							lastQuestionMarkPosition = GetCurrentPosition();
							question_mark_entity->ShowTextPanel();
							break;
						}
					}
				}
				break;
			case EGardenState::InQuestionMark:
				if (is_skipping_questionmark) {
					vector<shared_ptr<EntityScript>> entities;
					for (const auto& entity : garden->GetEntitiesAt(GetCurrentPosition(), entities)) {
						if (shared_ptr<QuestionMarkEntityScript> question_mark_entity = dynamic_pointer_cast<QuestionMarkEntityScript>(entity)) {
							question_mark_entity->HideTextPanel();
							break;
						}
					}
				}
				break;
			}
		}
	}
}
