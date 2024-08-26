#include <chrono>
#include <cstdint>
#include <memory>

#include <OGP/Environment/EGardenCellType.hpp>
#include <OGP/Game.hpp>
#include <OGP/Rendering/Color.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/DoorCellScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>
#include <OGP/Scripting/Rendering/SpriteRendererScript.hpp>

using namespace OGP;
using namespace OGP::Environment;
using namespace OGP::Rendering;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Environment;
using namespace OGP::Scripting::Rendering;
using namespace std;
using namespace std::chrono;

DoorCellScript::DoorCellScript(Node* node) : CellScript(node), isOpen(false) {
	// ...
}

bool DoorCellScript::IsSolid() const noexcept {
	return !isOpen;
}

bool DoorCellScript::IsTopDeadly() const noexcept {
	return true;
}

bool DoorCellScript::Interact() noexcept {
	bool ret(false);
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		switch (GetGardenCellType()) {
		case EGardenCellType::RedDoor:
		case EGardenCellType::AutomaticallyClosingRedDoor:
			ret = garden->UseRedKey();
			if (ret) {
				isOpen = true;
			}
			break;
		case EGardenCellType::YellowDoor:
		case EGardenCellType::AutomaticallyClosingYellowDoor:
			ret = garden->UseYellowKey();
			if (ret) {
				isOpen = true;
			}
			break;
		case EGardenCellType::GreenDoor:
		case EGardenCellType::AutomaticallyClosingGreenDoor:
			ret = garden->UseGreenKey();
			if (ret) {
				isOpen = true;
			}
			break;
		default:
		}
	}
	return ret;
}

void DoorCellScript::OnFrameRender(Game& game, high_resolution_clock::duration deltaTime) {
	if (shared_ptr<SpriteRendererScript> foreground_sprite_renderer = GetForegroundSpriteRenderer().lock()) {
		foreground_sprite_renderer->SetColor(Color<uint8_t>(0xFF, 0xFF, 0xFF, isOpen ? 0x0 : 0xFF));
	}
}
