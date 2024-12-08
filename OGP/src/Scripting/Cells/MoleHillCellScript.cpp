#include <chrono>
#include <cmath>
#include <cstdint>
#include <memory>

#include <iostream>

#include <Klein/Engine.hpp>
#include <Klein/Rendering/Color.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/MoleHillCellScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Rendering;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Scripting::Cells;

constexpr const high_resolution_clock::duration moleAppearanceLoopTime(4s);
constexpr const float moleAppearanceRatio(0.125f);

MoleHillCellScript::MoleHillCellScript(Node* node) : CellScript(node) {
	// ...
}

bool MoleHillCellScript::IsDeadly() const noexcept {
	return fmod(duration<float>(high_resolution_clock::now() - spawnTime).count() / duration<float>(moleAppearanceLoopTime).count(), 1.0f) >= (1.0f - moleAppearanceRatio);
}

void MoleHillCellScript::OnInitialize(Engine& engine) {
	spawnTime = high_resolution_clock::now();
}

void MoleHillCellScript::OnFrameRender(Engine& engine, high_resolution_clock::duration deltaTime) {
	if (shared_ptr<SpriteRendererScript> foreground_sprite_renderer = GetForegroundSpriteRenderer().lock()) {
		foreground_sprite_renderer->SetColor(Color<uint8_t>(0xFF, 0xFF, 0xFF, IsDeadly() ? 0xFF : 0x0));
	}
}
