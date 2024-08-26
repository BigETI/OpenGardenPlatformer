#include <chrono>
#include <cmath>
#include <cstdint>
#include <memory>

#include <iostream>

#include <OGP/Game.hpp>
#include <OGP/Rendering/Color.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/MoleHillCellScript.hpp>
#include <OGP/Scripting/Rendering/SpriteRendererScript.hpp>

using namespace OGP;
using namespace OGP::Rendering;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Rendering;
using namespace std;
using namespace std::chrono;

constexpr const high_resolution_clock::duration moleAppearanceLoopTime(4s);
constexpr const float moleAppearanceRatio(0.125f);

MoleHillCellScript::MoleHillCellScript(Node* node) : CellScript(node) {
	// ...
}

bool MoleHillCellScript::IsDeadly() const noexcept {
	return fmod(duration<float>(high_resolution_clock::now() - spawnTime).count() / duration<float>(moleAppearanceLoopTime).count(), 1.0f) >= (1.0f - moleAppearanceRatio);
}

void MoleHillCellScript::OnInitialize(Game& game) {
	spawnTime = high_resolution_clock::now();
}

void MoleHillCellScript::OnFrameRender(Game& game, high_resolution_clock::duration deltaTime) {
	if (shared_ptr<SpriteRendererScript> foreground_sprite_renderer = GetForegroundSpriteRenderer().lock()) {
		foreground_sprite_renderer->SetColor(Color<uint8_t>(0xFF, 0xFF, 0xFF, IsDeadly() ? 0xFF : 0x0));
	}
}
