#include <chrono>
#include <cmath>
#include <cstdint>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Rendering/Color.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Cells/EMoleHillState.hpp>
#include <OGP/Environment/EGardenState.hpp>
#include <OGP/Environment/GlobalWorld.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/MoleHillCellScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Rendering;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Cells;
using namespace OGP::Environment;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Environment;

constexpr static const float moleAppearanceLoopTickCount(90.0f);
constexpr static const float startingAnimationTickCount(4.0f);
constexpr static const float appearingTickCount(17.0f);
constexpr static const float deadlyTickCount(12.0f);
constexpr static const float disappearingTickCount(12.0f);
constexpr static const float endingAnimationTickCount(moleAppearanceLoopTickCount - startingAnimationTickCount - appearingTickCount - deadlyTickCount - disappearingTickCount);

MoleHillCellScript::MoleHillCellScript(Node* node) : CellScript(node), moleHillState(EMoleHillState::Hidden), elapsedTime(high_resolution_clock::duration::zero()) {
	// ...
}

EMoleHillState MoleHillCellScript::GetMoleHillState() const noexcept {
	return moleHillState;
}

bool MoleHillCellScript::IsDeadly() const noexcept {
	return moleHillState == EMoleHillState::Deadly;
}

void MoleHillCellScript::OnInitialize(Engine& engine) {
	elapsedTime = GlobalWorld::GetDuration(endingAnimationTickCount);
}

void MoleHillCellScript::OnFrameRender(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		if (garden->GetGardenState() != EGardenState::Playing) {
			return;
		}
		elapsedTime += deltaTime;
		bool is_repeating_loop(true);
		while (is_repeating_loop) {
			is_repeating_loop = false;
			high_resolution_clock::duration maximal_time;
			switch (moleHillState) {
			case EMoleHillState::Hidden:
				maximal_time = GlobalWorld::GetDuration(endingAnimationTickCount + startingAnimationTickCount);
				if (elapsedTime >= maximal_time) {
					elapsedTime -= maximal_time;
					is_repeating_loop = true;
					moleHillState = EMoleHillState::Appearing;
				}
				break;
			case EMoleHillState::Appearing:
				maximal_time = GlobalWorld::GetDuration(appearingTickCount);
				if (elapsedTime >= maximal_time) {
					elapsedTime -= maximal_time;
					is_repeating_loop = true;
					moleHillState = EMoleHillState::Deadly;
				}
				break;
			case EMoleHillState::Deadly:
				maximal_time = GlobalWorld::GetDuration(deadlyTickCount);
				if (elapsedTime >= maximal_time) {
					elapsedTime -= maximal_time;
					is_repeating_loop = true;
					moleHillState = EMoleHillState::Disappearing;
				}
				break;
			case EMoleHillState::Disappearing:
				maximal_time = GlobalWorld::GetDuration(disappearingTickCount);
				if (elapsedTime >= maximal_time) {
					elapsedTime -= maximal_time;
					is_repeating_loop = true;
					moleHillState = EMoleHillState::Hidden;
				}
				break;
			}
		}
	}
	if (shared_ptr<SpriteRendererScript> foreground_sprite_renderer = GetForegroundSpriteRenderer().lock()) {
		foreground_sprite_renderer->SetTexture2DVisibility(IsDeadly());
	}
}
