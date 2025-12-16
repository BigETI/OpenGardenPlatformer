#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include <Klein/Engine.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/Rendering/Color.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Environment/EGardenState.hpp>
#include <OGP/Scripting/Audio/SoundEffectsScript.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/QuestionMarkEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;
using namespace std::literals::chrono_literals;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::Rendering;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Entities;
using namespace OGP::Environment;
using namespace OGP::Scripting::Audio;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

constexpr static float textFontSize(0.25f);
constexpr static float textSpacing(textFontSize * 0.2f);
constexpr static float textPanelMargin(0.25f);
constexpr static float textPanelSpacing(0.25f);

constexpr const static high_resolution_clock::duration textCharacterAnimationTime(15ms);
#ifdef IS_OGP_CXX_STD_17
string::size_type
erase(string& c, const string::value_type& value) {
	size_t ret(static_cast<size_t>(0));
	for (size_t index(static_cast<size_t>(0)); (index + ret) != c.size(); index++) {
		if (c.at(index) == value) {
			for (size_t shift_index(index); (shift_index + static_cast<size_t>(1)) != c.size(); shift_index++) {
				c[shift_index] = c.at(shift_index + 1);
			}
			++ret;
		}
	}
	if (ret > static_cast<size_t>(0)) {
		c[c.size() - ret] = '\0';
		c.resize(c.size() - ret);
	}
	return ret;
}
#endif
QuestionMarkEntityScript::QuestionMarkEntityScript(Node* node) :
	EntityScript(node),
	isTextPanelVisible(false),
	elapsedTextCharacterAnimationTime(high_resolution_clock::duration::zero()),
	animationLineIndex(static_cast<size_t>(0)),
	animationCharacterIndex(static_cast<size_t>(0)) {
	shared_ptr<Node> text_panel_root_node(node->CreateNewChild());
	shared_ptr<SpriteRendererScript> panel_sprite_renderer(text_panel_root_node->CreateNewChild()->EnsureScript<SpriteRendererScript>());
	panel_sprite_renderer->SetLayerIndex(1U);
	panel_sprite_renderer->SetColor(Color<float>(0.0f, 0.0f, 0.0f, 0.5f));
	panel_sprite_renderer->SetTexture2DVisibility(false);
	textPanelRootNode = text_panel_root_node;
	panelSpriteRenderer = panel_sprite_renderer;
}

void QuestionMarkEntityScript::ShowTextPanel() noexcept {
	if (isTextPanelVisible) {
		return;
	}
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		if (garden->GetGardenState() == EGardenState::Playing) {
			garden->SetGardenState(EGardenState::InQuestionMark);
			SetTextPanelVisibility(true);
			elapsedTextCharacterAnimationTime = high_resolution_clock::duration::zero();
			animationLineIndex = static_cast<size_t>(0);
			animationCharacterIndex = static_cast<size_t>(0);
			if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
				sound_effects->PlaySoundEffect("ShowDialog");
			}
		}
	}
}

void QuestionMarkEntityScript::HideTextPanel() noexcept {
	if (!isTextPanelVisible) {
		return;
	}
	if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
		if (garden->GetGardenState() == EGardenState::InQuestionMark) {
			garden->SetGardenState(EGardenState::Playing);
			SetTextPanelVisibility(false);
		}
	}
}

void QuestionMarkEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	EntityScript::Spawn(gardenEntityData, garden);
	string text(gardenEntityData.text);
	erase(text, '\r');
	size_t text_position(static_cast<size_t>(0));
	size_t maximal_line_length(static_cast<size_t>(0));
	lines.clear();
	while ((text_position = text.find('\n')) != std::string::npos) {
		string line(text.substr(0, text_position));
		maximal_line_length = max(maximal_line_length, line.length());
		lines.push_back(line);
		text.erase(0, text_position + static_cast<size_t>(1));
	}
	if (!text.empty()) {
		lines.push_back(text);
		maximal_line_length = max(maximal_line_length, text.length());
	}
	for (const auto& text_sprite_renderer : textSpriteRenderers) {
		if (shared_ptr<SpriteRendererScript> current_text_sprite_renderer = text_sprite_renderer.lock()) {
			current_text_sprite_renderer->GetNode().Destroy();
		}
	}
	textSpriteRenderers.clear();
	if (shared_ptr<Node> text_panel_root_node = textPanelRootNode.lock()) {
		if (shared_ptr<SpriteRendererScript> panel_sprite_renderer = panelSpriteRenderer.lock()) {
			Vector2<float> panel_size((((maximal_line_length * (textFontSize + textSpacing)) - textSpacing) * 0.53125f) + textPanelMargin + textPanelMargin, lines.size() * (textFontSize + textSpacing) - textSpacing + textPanelMargin + textPanelMargin);
			Vector2<float> garden_size(garden->GetGardenCells().GetSize().GetConverted<float>());
			Vector2<float> half_garden_size(garden_size * 0.5f);
			Vector2<float> target_world_position(Vector2<float>(panel_size.x * -0.5f, panel_size.y + 1.0f) + GetNode().GetLocalPosition());
			text_panel_root_node->SetLocalPosition(
				Vector2<float>(
					(panel_size.x > garden_size.x) ?
					(half_garden_size.x - 0.5f) :
					((target_world_position.x < textPanelSpacing) ? (textPanelSpacing - 0.5f) : (((target_world_position.x + panel_size.x + textPanelSpacing) > garden_size.x) ? (garden_size.x - panel_size.x - textPanelSpacing - 0.5f) : target_world_position.x)),
					(panel_size.y > garden_size.y) ?
					(half_garden_size.y - 0.5f) :
					((target_world_position.y < textPanelSpacing) ? (textPanelSpacing - 0.5f) : (((target_world_position.y + panel_size.y + textPanelSpacing) > garden_size.y) ? (garden_size.y - panel_size.y - textPanelSpacing - 0.5f) : target_world_position.y))
				) - GetNode().GetLocalPosition()
			);
			panel_sprite_renderer->SetPivot(Vector2<float>());
			panel_sprite_renderer->GetNode().SetLocalPosition(Vector2<float>());
			panel_sprite_renderer->GetNode().SetScale(panel_size);
			for (size_t index(static_cast<size_t>(0)); index != lines.size(); index++) {
				shared_ptr<SpriteRendererScript> text_sprite_renderer(text_panel_root_node->CreateNewChild()->EnsureScript<SpriteRendererScript>());
				text_sprite_renderer->GetNode().SetLocalPosition(Vector2<float>(textPanelMargin, (index * -(textFontSize + textSpacing)) - textPanelMargin));
				text_sprite_renderer->SetPivot(Vector2<float>());
				text_sprite_renderer->SetLayerIndex(0U);
				text_sprite_renderer->SetTextFontSize(0.25f);
				text_sprite_renderer->SetTextSpacing(0.2f * 0.25f);
				text_sprite_renderer->SetTexture2DVisibility(false);
				text_sprite_renderer->SetTextVisibility(false);
				textSpriteRenderers.push_back(text_sprite_renderer);
			}
		}
	}
}

void QuestionMarkEntityScript::OnGameTick(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	if (isTextPanelVisible && (animationLineIndex < lines.size())) {
		elapsedTextCharacterAnimationTime += deltaTime;
		while ((animationLineIndex < lines.size()) && (elapsedTextCharacterAnimationTime > textCharacterAnimationTime)) {
			elapsedTextCharacterAnimationTime -= textCharacterAnimationTime;
			const string& line(lines.at(animationLineIndex));
			if (animationCharacterIndex < line.length()) {
				++animationCharacterIndex;
				if (animationLineIndex < textSpriteRenderers.size()) {
					if (shared_ptr<SpriteRendererScript> text_sprite_renderer = textSpriteRenderers.at(animationLineIndex).lock()) {
						text_sprite_renderer->SetText(line.substr(static_cast<size_t>(0), animationCharacterIndex));
					}
				}
			}
			else {
				++animationLineIndex;
				animationCharacterIndex = static_cast<size_t>(0);
				if (animationLineIndex < lines.size()) {
					elapsedTextCharacterAnimationTime = high_resolution_clock::duration::zero();
					break;
				}
			}
		}
	}
}

void QuestionMarkEntityScript::SetTextPanelVisibility(bool isTextPanelVisible) noexcept {
	this->isTextPanelVisible = isTextPanelVisible;
	if (shared_ptr<SpriteRendererScript> panel_sprite_renderer = panelSpriteRenderer.lock()) {
		panel_sprite_renderer->SetTexture2DVisibility(isTextPanelVisible);
	}
	for (const auto& text_sprite_renderer : textSpriteRenderers) {
		if (shared_ptr<SpriteRendererScript> current_text_sprite_renderer = text_sprite_renderer.lock()) {
			current_text_sprite_renderer->SetTextVisibility(isTextPanelVisible);
			if (isTextPanelVisible) {
				current_text_sprite_renderer->SetText(string());
			}
		}
	}
}
