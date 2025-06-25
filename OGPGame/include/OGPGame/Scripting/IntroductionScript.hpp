#pragma once

#include <array>
#include <chrono>
#include <cstddef>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Script.hpp>
#include <Klein/Scripting/UI/CanvasScript.hpp>
#include <Klein/Scripting/UI/ImageScript.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>

#include "../EIntroductionAnimationState.hpp"

namespace OGPGame::Scripting {
	class IntroductionScript : public Klein::Scripting::Script {
	public:

		IntroductionScript(Klein::SceneManagement::Node* node);

	protected:

		void OnInitialize(Klein::Engine& engine) override;
		void OnGameTick(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;
		void OnFrameRender(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		std::weak_ptr<Klein::Scripting::UI::CanvasScript> canvas;
		std::weak_ptr<Klein::Scripting::UI::ImageScript> image;
		std::array<Klein::ResourceManagement::ResourceID, 2> spriteResourceIDs;
		std::size_t currentSpriteResourceIDIndex;
		OGPGame::EIntroductionAnimationState animationState;
		std::chrono::high_resolution_clock::duration elapsedAnimationTime;
		bool isIntroducing;
		bool isSkippingIntroductionStep;

		void SkipIntroductionStep();
	};
}
