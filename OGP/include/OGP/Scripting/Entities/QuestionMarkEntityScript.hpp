#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Entities/GardenEntityData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Scripting/Environment/GardenScript.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class QuestionMarkEntityScript : public EntityScript {
	public:

		OGP_API QuestionMarkEntityScript(Klein::SceneManagement::Node* node);

		OGP_API void ShowTextPanel() noexcept;
		OGP_API void HideTextPanel() noexcept;
		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;
		OGP_API virtual void OnGameTick(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		std::weak_ptr<Klein::SceneManagement::Node> textPanelRootNode;
		std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript> panelSpriteRenderer;
		std::vector<std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript>> textSpriteRenderers;
		bool isTextPanelVisible;
		std::vector<std::string> lines;
		std::chrono::high_resolution_clock::duration elapsedTextCharacterAnimationTime;
		size_t animationLineIndex;
		size_t animationCharacterIndex;

		void SetTextPanelVisibility(bool isTextPanelVisible) noexcept;
	};
}
