#include <chrono>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <Klein/Engine.hpp>
#include <Klein/Math/Rectangle.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Cells/EGardenCellType.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Cells;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

const ResourceID groundTextureResourceID(string("Textures/Environment/Ground.png"));
constexpr inline const Vector2<float> groundSourceRectangleSize(0.1f, 0.125f);
const ResourceID debugCellsTextureResourceID(string("Debug/Textures/Cells.png"));
constexpr inline const Vector2<float> debugCellSourceRectangleSize(0.125f, 0.125f);
#ifndef IS_OGP_CXX_STD_17
constexpr
#endif
inline static bool IsCellGroundConnectable(const shared_ptr<CellScript>& cell) noexcept {
	return !cell || cell->IsGroundConnectable();
}

constexpr inline Rectangle<float> GetDebugCellSourceRectangle(EGardenCellType gardenCellType) {
	return Rectangle<float>(Vector2<float>(static_cast<float>(static_cast<int>(gardenCellType) % 8), static_cast<float>(static_cast<int>(gardenCellType) / 8)) / 8.0f, debugCellSourceRectangleSize);
}

const Rectangle<float> airCellSourceRectangle = GetDebugCellSourceRectangle(EGardenCellType::Air);

CellScript::CellScript(Node* node) : Script(node), wasGroundConnectable(false) {
	// ...
}

weak_ptr<GardenScript> CellScript::GetGarden() const noexcept {
	return garden;
}

EGardenCellType CellScript::GetGardenCellType() const noexcept {
	return gardenCellType;
}

void CellScript::UpdateProperties(EGardenCellType gardenCellType, const Vector2<size_t>& position, weak_ptr<GardenScript> garden) {
	this->gardenCellType = gardenCellType;
	this->garden = garden;
	this->position = position;

	// TODO: Add support for cell texture animation

	shared_ptr<Node> background_sprite_renderer_node(GetNode().CreateNewChild());
	shared_ptr<SpriteRendererScript> background_sprite_renderer(background_sprite_renderer_node->EnsureScript<SpriteRendererScript>());
	background_sprite_renderer->SetTexture2DResourceID(debugCellsTextureResourceID);
	background_sprite_renderer->SetTexture2DSourceRectangle(airCellSourceRectangle);
	background_sprite_renderer->SetLayerIndex(5U);
	backgroundSpriteRenderer = background_sprite_renderer;

	shared_ptr<SpriteRendererScript> foreground_sprite_renderer;
	switch (gardenCellType) {
	case EGardenCellType::Ground:
	case EGardenCellType::HardGround:
	case EGardenCellType::Trap:
	case EGardenCellType::GrassGround:
	case EGardenCellType::HardGrassGround:
	case EGardenCellType::GrassTrap:
		for (size_t index(static_cast<size_t>(0)); index != static_cast<size_t>(4); index++) {
			foreground_sprite_renderer = AppendForegroundSpriteRenderer();
			foreground_sprite_renderer->GetNode().SetLocalPosition(Vector2<float>(((index % static_cast<size_t>(2)) == static_cast<size_t>(0)) ? -0.25f : 0.25f, ((index / static_cast<size_t>(2)) == static_cast<size_t>(0)) ? 0.25f : -0.25f));
			foreground_sprite_renderer->GetNode().SetLocalScale(Vector2<float>(0.5f, 0.5f));
			foreground_sprite_renderer->SetTexture2DResourceID(groundTextureResourceID);
		}
		if (shared_ptr<GardenScript> current_garden = garden.lock()) {
			UpdateVisuals(current_garden);
		}
		break;
	default:
		foreground_sprite_renderer = AppendForegroundSpriteRenderer();
		foreground_sprite_renderer->SetTexture2DResourceID(debugCellsTextureResourceID);
		foreground_sprite_renderer->SetTexture2DSourceRectangle(GetDebugCellSourceRectangle(gardenCellType));
		break;
	}
}

const weak_ptr<SpriteRendererScript>& CellScript::GetBackgroundSpriteRenderer() const noexcept {
	return backgroundSpriteRenderer;
}

const vector<weak_ptr<SpriteRendererScript>>& CellScript::GetForegroundSpriteRenderers() const noexcept {
	return foregroundSpriteRenderers;
}

void CellScript::UpdateVisuals() {
	if (shared_ptr<GardenScript> current_garden = garden.lock()) {
		UpdateVisuals(current_garden);
	}
}

bool CellScript::IsSolid() const noexcept {
	return false;
}

bool CellScript::IsTopWalkable() const noexcept {
	return (IsSolid() || IsClimbingUpAllowed()) && !IsDeadly() && !IsTopDeadly();
}

bool CellScript::IsClimbable() const noexcept {
	return IsClimbingUpAllowed();
}

bool CellScript::IsClimbingUpAllowed() const noexcept {
	return false;
}

bool CellScript::IsDeadly() const noexcept {
	return false;
}

bool CellScript::IsTopDeadly() const noexcept {
	return IsDeadly();
}

bool CellScript::IsGroundConnectable() const noexcept {
	return
		(gardenCellType == EGardenCellType::Ground) ||
		(gardenCellType == EGardenCellType::HardGround) ||
		(gardenCellType == EGardenCellType::Trap) ||
		(gardenCellType == EGardenCellType::GrassGround) ||
		(gardenCellType == EGardenCellType::HardGrassGround) ||
		(gardenCellType == EGardenCellType::GrassTrap) ||
		(gardenCellType == EGardenCellType::Water) ||
		(gardenCellType == EGardenCellType::SurfaceWater);
}

bool CellScript::Dig() noexcept {
	return false;
}

bool CellScript::Interact(EntityScript& sourceEntity) noexcept {
	return false;
}

void CellScript::OnFrameRender(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	bool is_ground_connectable(IsGroundConnectable());
	if (wasGroundConnectable != is_ground_connectable) {
		wasGroundConnectable = is_ground_connectable;
		if (shared_ptr<GardenScript> current_garden = garden.lock()) {
			bool is_top_available((position.y + static_cast<size_t>(1)) < current_garden->GetGardenCells().GetSize().y);
			bool is_bottom_available(position.y > static_cast<size_t>(0));
			bool is_left_available(position.x > static_cast<size_t>(0));
			bool is_right_available((position.x + static_cast<size_t>(1)) < current_garden->GetGardenCells().GetSize().x);
			if (is_top_available) {
				current_garden->GetCellAt(position + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)))->UpdateVisuals(current_garden);
				if (is_left_available) {
					current_garden->GetCellAt(Vector2<size_t>(position.x - static_cast<size_t>(1), position.y + static_cast<size_t>(1)))->UpdateVisuals(current_garden);
				}
				if (is_right_available) {
					current_garden->GetCellAt(position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(1)))->UpdateVisuals(current_garden);
				}
			}
			if (is_bottom_available) {
				current_garden->GetCellAt(position - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)))->UpdateVisuals(current_garden);
				if (is_left_available) {
					current_garden->GetCellAt(position - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(1)))->UpdateVisuals(current_garden);
				}
				if (is_right_available) {
					current_garden->GetCellAt(Vector2<size_t>(position.x + static_cast<size_t>(1), position.y - static_cast<size_t>(1)))->UpdateVisuals(current_garden);
				}
			}
			if (is_left_available) {
				current_garden->GetCellAt(position - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)))->UpdateVisuals(current_garden);
			}
			if (is_right_available) {
				current_garden->GetCellAt(position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0)))->UpdateVisuals(current_garden);
			}
			UpdateVisuals(current_garden);
		}
	}
}

void CellScript::UpdateVisuals(const shared_ptr<GardenScript>& garden) {
	switch (gardenCellType) {
	case EGardenCellType::Ground:
	case EGardenCellType::HardGround:
	case EGardenCellType::Trap:
	case EGardenCellType::GrassGround:
	case EGardenCellType::HardGrassGround:
	case EGardenCellType::GrassTrap:
		if (foregroundSpriteRenderers.size() == static_cast<size_t>(4)) {
			if (shared_ptr<SpriteRendererScript> top_left_foreground_sprite_renderer = foregroundSpriteRenderers.at(static_cast<size_t>(0)).lock()) {
				if (shared_ptr<SpriteRendererScript> top_right_foreground_sprite_renderer = foregroundSpriteRenderers.at(static_cast<size_t>(1)).lock()) {
					if (shared_ptr<SpriteRendererScript> bottom_left_foreground_sprite_renderer = foregroundSpriteRenderers.at(static_cast<size_t>(2)).lock()) {
						if (shared_ptr<SpriteRendererScript> bottom_right_foreground_sprite_renderer = foregroundSpriteRenderers.at(static_cast<size_t>(3)).lock()) {
							bool is_top_connectable(false);
							bool is_bottom_connectable(false);
							bool is_left_connectable(false);
							bool is_right_connectable(false);
							bool is_top_left_connectable(false);
							bool is_bottom_left_connectable(false);
							bool is_top_right_connectable(false);
							bool is_bottom_right_connectable(false);
							if (IsGroundConnectable()) {
								is_top_connectable = ((position.y + static_cast<size_t>(1)) >= garden->GetGardenCells().GetSize().y) || IsCellGroundConnectable(garden->GetCellAt(position + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1))));
								is_bottom_connectable = (position.y < static_cast<size_t>(1)) || IsCellGroundConnectable(garden->GetCellAt(position - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1))));
								is_left_connectable = (position.x < static_cast<size_t>(1)) || IsCellGroundConnectable(garden->GetCellAt(position - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0))));
								is_right_connectable = ((position.x + static_cast<size_t>(1)) >= garden->GetGardenCells().GetSize().x) || IsCellGroundConnectable(garden->GetCellAt(position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(0))));
								is_top_left_connectable = (position.x < static_cast<size_t>(1)) || ((position.y + static_cast<size_t>(1)) >= garden->GetGardenCells().GetSize().y) || IsCellGroundConnectable(garden->GetCellAt(Vector2<size_t>(position.x - static_cast<size_t>(1), position.y + static_cast<size_t>(1))));
								is_bottom_left_connectable = (position.x < static_cast<size_t>(1)) || (position.y < static_cast<size_t>(1)) || IsCellGroundConnectable(garden->GetCellAt(position - Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(1))));
								is_top_right_connectable = ((position.x + static_cast<size_t>(1)) >= garden->GetGardenCells().GetSize().x) || ((position.y + static_cast<size_t>(1)) >= garden->GetGardenCells().GetSize().y) || IsCellGroundConnectable(garden->GetCellAt(position + Vector2<size_t>(static_cast<size_t>(1), static_cast<size_t>(1))));
								is_bottom_right_connectable = ((position.x + static_cast<size_t>(1)) >= garden->GetGardenCells().GetSize().x) || (position.y < static_cast<size_t>(1)) || IsCellGroundConnectable(garden->GetCellAt(Vector2<size_t>(position.x + static_cast<size_t>(1), position.y - static_cast<size_t>(1))));
							}
							int top_left_index(is_top_connectable ? (is_left_connectable ? (is_top_left_connectable ? 0 : 4) : 2) : (is_left_connectable ? 1 : 3));
							int top_right_index(is_top_connectable ? (is_right_connectable ? (is_top_right_connectable ? 5 : 9) : 7) : (is_right_connectable ? 6 : 8));
							int bottom_left_index(is_bottom_connectable ? (is_left_connectable ? (is_bottom_left_connectable ? 10 : 14) : 12) : (is_left_connectable ? 11 : 13));
							int bottom_right_index(is_bottom_connectable ? (is_right_connectable ? (is_bottom_right_connectable ? 15 : 19) : 17) : (is_right_connectable ? 16 : 18));
							switch (gardenCellType) {
							case EGardenCellType::HardGround:
								top_left_index += 20;
								top_right_index += 20;
								bottom_left_index += 20;
								bottom_right_index += 20;
								break;
							case EGardenCellType::GrassGround:
							case EGardenCellType::GrassTrap:
								top_left_index += 40;
								top_right_index += 40;
								bottom_left_index += 40;
								bottom_right_index += 40;
								break;
							case EGardenCellType::HardGrassGround:
								top_left_index += 60;
								top_right_index += 60;
								bottom_left_index += 60;
								bottom_right_index += 60;
								break;
							default:
								break;
							}
							Vector2<float> top_left_offset(static_cast<float>(top_left_index % 10) / 10.0f, static_cast<float>(top_left_index / 10) / 8.0f);
							Vector2<float> top_right_offset(static_cast<float>(top_right_index % 10) / 10.0f, static_cast<float>(top_right_index / 10) / 8.0f);
							Vector2<float> bottom_left_offset(static_cast<float>(bottom_left_index % 10) / 10.0f, static_cast<float>(bottom_left_index / 10) / 8.0f);
							Vector2<float> bottom_right_offset(static_cast<float>(bottom_right_index % 10) / 10.0f, static_cast<float>(bottom_right_index / 10) / 8.0f);
							top_left_foreground_sprite_renderer->SetTexture2DSourceRectangle(Rectangle<float>(top_left_offset, groundSourceRectangleSize));
							top_right_foreground_sprite_renderer->SetTexture2DSourceRectangle(Rectangle<float>(top_right_offset, groundSourceRectangleSize));
							bottom_left_foreground_sprite_renderer->SetTexture2DSourceRectangle(Rectangle<float>(bottom_left_offset, groundSourceRectangleSize));
							bottom_right_foreground_sprite_renderer->SetTexture2DSourceRectangle(Rectangle<float>(bottom_right_offset, groundSourceRectangleSize));
						}
					}
				}
			}
		}
	default:
		break;
	}
}

shared_ptr<SpriteRendererScript> CellScript::AppendForegroundSpriteRenderer() {
	shared_ptr<Node> foreground_sprite_renderer_node(GetNode().CreateNewChild());
	shared_ptr<SpriteRendererScript> ret(foreground_sprite_renderer_node->EnsureScript<SpriteRendererScript>());
	ret->SetLayerIndex(4U);
	foregroundSpriteRenderers.push_back(ret);
	return ret;
}
