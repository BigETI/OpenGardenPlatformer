#include <chrono>
#include <cstdint>
#include <memory>

#include <OGP/Cells/EGardenCellType.hpp>
#include <OGP/Game.hpp>
#include <OGP/Rendering/Color.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/ClimbableCellScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>
#include <OGP/Scripting/Rendering/SpriteRendererScript.hpp>

using namespace OGP::Cells;
using namespace OGP::Rendering;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Environment;
using namespace OGP::Scripting::Rendering;
using namespace std;
using namespace std::chrono;

ClimbableCellScript::ClimbableCellScript(Node* node) : CellScript(node) {
	// ...
}

bool ClimbableCellScript::IsClimbable() const noexcept {
	bool ret(false);
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		switch (GetGardenCellType()) {
		case EGardenCellType::Ladder:
			ret = true;
			break;
		case EGardenCellType::Rope:
			ret = true;
			break;
		case EGardenCellType::LadderShowingUpWhenFinished:
			ret = garden->IsCompletionEnabled();
			break;
		case EGardenCellType::LadderVanishingWhenFinished:
			ret = !garden->IsCompletionEnabled();
			break;
		default:
			break;
		}
	}
	return ret;
}

bool ClimbableCellScript::IsClimbingUpAllowed() const noexcept {
	return IsClimbable() && (GetGardenCellType() != EGardenCellType::Rope);
}

void ClimbableCellScript::OnFrameRender(Game& game, high_resolution_clock::duration deltaTime) {
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		if (shared_ptr<SpriteRendererScript> foreground_sprite_renderer = GetForegroundSpriteRenderer().lock()) {
			switch (GetGardenCellType()) {
			case EGardenCellType::LadderShowingUpWhenFinished:
				foreground_sprite_renderer->SetColor(Color<uint8_t>(0xFF, 0xFF, 0xFF, garden->IsCompletionEnabled() ? 0xFF : 0x0));
				break;
			case EGardenCellType::LadderVanishingWhenFinished:
				foreground_sprite_renderer->SetColor(Color<uint8_t>(0xFF, 0xFF, 0xFF, garden->IsCompletionEnabled() ? 0x0 : 0xFF));
				break;
			default:
				break;
			}
		}
	}
}
