#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>

#include <Klein/Engine.hpp>
#include <Klein/Math/Bounds.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>
#include <Klein/Scripting/Script.hpp>
#include <Klein/Scripting/UI/CanvasScript.hpp>
#include <Klein/Scripting/UI/ImageScript.hpp>
#include <Klein/Rendering/Color.hpp>
#include <Klein/ResourceManagement/FileSystem.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>

#include <OGP/Scripting/Audio/MusicPlayerScript.hpp>

#include <OGPGame/Game.hpp>
#include <OGPGame/Scripting/IntroductionScript.hpp>

using namespace std;
using namespace std::chrono;
using namespace std::literals::chrono_literals;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting;
using namespace Klein::Scripting::Rendering;
using namespace Klein::Scripting::UI;
using namespace Klein::Rendering;
using namespace Klein::ResourceManagement;

using namespace OGP::Scripting::Audio;

using namespace OGPGame;
using namespace OGPGame::Scripting;

constexpr static const high_resolution_clock::duration maximalFadingInAnimationTime(1s);
constexpr static const high_resolution_clock::duration maximalShowingAnimationTime(3s);
constexpr static const high_resolution_clock::duration maximalFadingOutAnimationTime(1s);

IntroductionScript::IntroductionScript(Node* node) :
	Script(node),
	spriteResourceIDs({ ResourceID("Textures/Introduction/Publisher.png"), ResourceID("Textures/Introduction/Game.png") }),
	currentSpriteResourceIDIndex(static_cast<size_t>(0)),
	animationState(EIntroductionAnimationState::FadingIn),
	elapsedAnimationTime(high_resolution_clock::duration::zero()),
	isIntroducing(true),
	isSkippingIntroductionStep(false) {
	shared_ptr<CanvasScript> canvas(node->CreateNewChild()->AddScript<CanvasScript>());
	canvas->SetRenderingContextIndex(static_cast<size_t>(1));
	shared_ptr<ImageScript> image(canvas->GetNode().CreateNewChild()->AddScript<ImageScript>());
	image->SetAnchor(Bounds<float>(1.0f, 0.0f, 0.0f, 1.0f));
	image->SetImageResourceID(spriteResourceIDs.at(static_cast<size_t>(0)));
	this->canvas = canvas;
	this->image = image;
}

void IntroductionScript::OnInitialize(Engine& engine) {
	elapsedAnimationTime = high_resolution_clock::duration::zero();
	if (MusicPlayerScript* music_player = MusicPlayerScript::GetGlobalMusicPlayer()) {
		music_player->EnqueueMusicToPlay("DGARDEN2.mp3", 1s);
	}
}

void IntroductionScript::OnGameTick(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	for (const auto& input_event : engine.GetCurrentInputEvents()) {
		const string& input_event_name(input_event.GetNameHash().GetString());
		if (input_event.IsPressing() && (input_event_name.starts_with("Keyboard") || ((input_event_name.starts_with("Gamepad") || input_event_name.starts_with("Mouse")) && input_event_name.contains("Button")))) {
			isSkippingIntroductionStep = true;
		}
	}
}

void IntroductionScript::OnFrameRender(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	if (isIntroducing) {
		elapsedAnimationTime += deltaTime;
		bool is_repeating_loop(true);
		while (is_repeating_loop) {
			is_repeating_loop = false;
			switch (animationState) {
			case EIntroductionAnimationState::FadingIn:
				if (elapsedAnimationTime >= maximalFadingInAnimationTime) {
					elapsedAnimationTime -= maximalFadingInAnimationTime;
					animationState = EIntroductionAnimationState::Showing;
				}
				if (isSkippingIntroductionStep) {
					elapsedAnimationTime = high_resolution_clock::duration::zero();
					SkipIntroductionStep();
				}
				break;
			case EIntroductionAnimationState::Showing:
				if (elapsedAnimationTime >= maximalShowingAnimationTime) {
					elapsedAnimationTime -= maximalShowingAnimationTime;
					animationState = EIntroductionAnimationState::FadingOut;
				}
				if (isSkippingIntroductionStep) {
					elapsedAnimationTime = high_resolution_clock::duration::zero();
					SkipIntroductionStep();
				}
				break;
			case EIntroductionAnimationState::FadingOut:
				if (elapsedAnimationTime >= maximalFadingOutAnimationTime) {
					elapsedAnimationTime -= maximalFadingOutAnimationTime;
					SkipIntroductionStep();
				}
				else if (isSkippingIntroductionStep) {
					elapsedAnimationTime = high_resolution_clock::duration::zero();
					SkipIntroductionStep();
				}
				break;
			}
		}
		if (shared_ptr<ImageScript> current_image = image.lock()) {
			float opacity;
			switch (animationState) {
			case EIntroductionAnimationState::FadingIn:
				opacity = clamp(duration<float>(elapsedAnimationTime).count() / duration<float>(maximalFadingInAnimationTime).count(), 0.0f, 1.0f);
				break;
			case EIntroductionAnimationState::Showing:
				opacity = 1.0f;
				break;
			case EIntroductionAnimationState::FadingOut:
				opacity = clamp(duration<float>(maximalFadingOutAnimationTime - elapsedAnimationTime).count() / duration<float>(maximalFadingOutAnimationTime).count(), 0.0f, 1.0f);
				break;
			}
			current_image->SetColor(Color<float>(1.0f, 1.0f, 1.0f, opacity));
		}
	}
}

void IntroductionScript::SkipIntroductionStep() {
	isSkippingIntroductionStep = false;
	++currentSpriteResourceIDIndex;
	if (currentSpriteResourceIDIndex < spriteResourceIDs.size()) {
		if (shared_ptr<ImageScript> current_image = image.lock()) {
			current_image->SetImageResourceID(spriteResourceIDs.at(currentSpriteResourceIDIndex));
		}
		animationState = EIntroductionAnimationState::FadingIn;
	}
	else {
		isIntroducing = false;
		Game::ShowMainMenuScene();
		GetNode().Destroy();
	}
}
