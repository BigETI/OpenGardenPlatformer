#include <chrono>
#include <cstdint>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Rendering/Color.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Cells/EGardenCellType.hpp>
#include <OGP/Environment/EDirection.hpp>
#include <OGP/Scripting/Audio/SoundEffectsScript.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/DoorCellScript.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Rendering;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Cells;
using namespace OGP::Environment;
using namespace OGP::Scripting::Audio;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

DoorCellScript::DoorCellScript(Node* node) : CellScript(node), isOpen(false), wasInteractedWithLastFrame(false) {
	// ...
}

bool DoorCellScript::IsSolid() const noexcept {
	return !isOpen;
}

bool DoorCellScript::IsTopDeadly() const noexcept {
	return true;
}

bool DoorCellScript::Interact(EntityScript& sourceEntity) noexcept {
	wasInteractedWithLastFrame = true;
	if (isOpen) {
		return false;
	}
	bool is_key_used(false);
	if (PlayerEntityScript* source_player_entity = dynamic_cast<PlayerEntityScript*>(&sourceEntity)) {
		switch (GetGardenCellType()) {
		case EGardenCellType::RedDoor:
		case EGardenCellType::AutomaticallyClosingRedDoor:
			is_key_used = source_player_entity->UseRedKey();
			break;
		case EGardenCellType::YellowDoor:
		case EGardenCellType::AutomaticallyClosingYellowDoor:
			is_key_used = source_player_entity->UseYellowKey();
			break;
		case EGardenCellType::GreenDoor:
		case EGardenCellType::AutomaticallyClosingGreenDoor:
			is_key_used = source_player_entity->UseGreenKey();
			break;
		default:
			break;
		}
	}
	return is_key_used && Open();
}

bool DoorCellScript::Open() {
	bool ret(!isOpen);
	if (ret) {
		isOpen = true;
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("OpenDoor");
		}
		OnOpened();
	}
	return ret;
}

bool DoorCellScript::Close() {
	bool ret(isOpen);
	if (ret) {
		isOpen = false;
		if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
			sound_effects->PlaySoundEffect("CloseDoor");
		}
		OnClosed();
	}
	return ret;
}

void DoorCellScript::OnGameTick(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	if (wasInteractedWithLastFrame) {
		wasInteractedWithLastFrame = false;
	}
	else if (isOpen && ((GetGardenCellType() == EGardenCellType::AutomaticallyClosingRedDoor) || (GetGardenCellType() == EGardenCellType::AutomaticallyClosingYellowDoor) || (GetGardenCellType() == EGardenCellType::AutomaticallyClosingGreenDoor))) {
		Close();
	}
}

void DoorCellScript::OnFrameRender(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	for (const auto& foreground_sprite_renderer : GetForegroundSpriteRenderers()) {
		if (shared_ptr<SpriteRendererScript> current_foreground_sprite_renderer = foreground_sprite_renderer.lock()) {
			current_foreground_sprite_renderer->SetTexture2DVisibility(!isOpen);
		}
	}
}
