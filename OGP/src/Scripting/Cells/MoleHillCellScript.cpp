#include <chrono>
#include <cmath>
#include <cstdint>
#include <memory>

#include <iostream>

#include <Klein/Engine.hpp>
#include <Klein/Rendering/Color.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Environment/EGardenState.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/MoleHillCellScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Rendering;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Environment;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Environment;

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

void MoleHillCellScript::OnFrameRender(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		if (garden->GetGardenState() != EGardenState::Playing) {
			spawnTime += deltaTime;
		}
	}
	if (shared_ptr<SpriteRendererScript> foreground_sprite_renderer = GetForegroundSpriteRenderer().lock()) {
		foreground_sprite_renderer->SetTexture2DVisibility(IsDeadly());
	}
}
