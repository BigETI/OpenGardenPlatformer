#include <chrono>
#include <cstdint>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Rendering/Color.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Cells/EGardenCellType.hpp>
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
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

DoorCellScript::DoorCellScript(Node* node) : CellScript(node), isOpen(false) {
	// ...
}

bool DoorCellScript::IsSolid() const noexcept {
	return !isOpen;
}

bool DoorCellScript::IsTopDeadly() const noexcept {
	return true;
}

bool DoorCellScript::Interact(EntityScript& sourceEntity) noexcept {
	bool ret(false);
	if (PlayerEntityScript* source_player_entity = dynamic_cast<PlayerEntityScript*>(&sourceEntity)) {
		switch (GetGardenCellType()) {
		case EGardenCellType::RedDoor:
		case EGardenCellType::AutomaticallyClosingRedDoor:
			ret = source_player_entity->UseRedKey();
			if (ret) {
				isOpen = true;
			}
			break;
		case EGardenCellType::YellowDoor:
		case EGardenCellType::AutomaticallyClosingYellowDoor:
			ret = source_player_entity->UseYellowKey();
			if (ret) {
				isOpen = true;
			}
			break;
		case EGardenCellType::GreenDoor:
		case EGardenCellType::AutomaticallyClosingGreenDoor:
			ret = source_player_entity->UseGreenKey();
			if (ret) {
				isOpen = true;
			}
			break;
		default:
			break;
		}
	}
	return ret;
}

void DoorCellScript::OnFrameRender(Engine& engine, high_resolution_clock::duration deltaTime) {
	if (shared_ptr<SpriteRendererScript> foreground_sprite_renderer = GetForegroundSpriteRenderer().lock()) {
		foreground_sprite_renderer->SetColor(Color<uint8_t>(0xFF, 0xFF, 0xFF, isOpen ? 0x0 : 0xFF));
	}
}
