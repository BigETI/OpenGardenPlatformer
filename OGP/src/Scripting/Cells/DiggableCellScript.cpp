#include <algorithm>
#include <chrono>
#include <cstdint>

#include <iostream>

#include <Klein/Engine.hpp>
#include <Klein/Rendering/Color.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Cells/EGardenCellType.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/DiggableCellScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Rendering;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Cells;
using namespace OGP::Scripting::Cells;

// TODO: Check, how much time a cell recorvers from being digged.
constexpr const high_resolution_clock::duration digRecoveryTime(20s / 4);

DiggableCellScript::DiggableCellScript(Node* node) : CellScript(node), lastDigTimePoint() {
	// ...
}

bool DiggableCellScript::IsSolid() const noexcept {
	return (GetGardenCellType() != EGardenCellType::Trap) && (GetGardenCellType() != EGardenCellType::GrassTrap) && (high_resolution_clock::now() - lastDigTimePoint) > digRecoveryTime;
}

bool DiggableCellScript::Dig() noexcept {
	lastDigTimePoint = high_resolution_clock::now();
	return true;
}

void DiggableCellScript::OnFrameRender(Engine& engine, high_resolution_clock::duration deltaTime) {
	high_resolution_clock::duration dig_time(high_resolution_clock::now() - lastDigTimePoint);
	if (shared_ptr<SpriteRendererScript> foreground_sprite_renderer = GetForegroundSpriteRenderer().lock()) {
		foreground_sprite_renderer->SetColor(Color<uint8_t>(0xFF, 0xFF, 0xFF, (dig_time > digRecoveryTime) ? 0xFF : static_cast<uint8_t>(max(duration<float>(dig_time).count() / duration<float>(digRecoveryTime).count(), 0.0f) * 255.0f)));
	}
}
