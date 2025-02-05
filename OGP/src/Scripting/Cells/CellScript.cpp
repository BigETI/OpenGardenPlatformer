#include <memory>
#include <string>

#include <Klein/Math/Rectangle.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>

#include <OGP/Cells/EGardenCellType.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;

using namespace Klein::Math;
using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Cells;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Environment;

const ResourceID debugCellsTextureResourceID(string("Debug/Textures/Cells.png"));
const Vector2<float> debugCellSourceRectangleSize(Vector2<float>(1.0f, 1.0f) / 8.0f);

Rectangle<float> GetDebugCellSourceRectangle(EGardenCellType gardenCellType) {
	return Rectangle<float>(Vector2<float>(static_cast<float>(static_cast<int>(gardenCellType) % 8), static_cast<float>(static_cast<int>(gardenCellType) / 8)) / 8.0f, debugCellSourceRectangleSize);
}

const Rectangle<float> airCellSourceRectangle = GetDebugCellSourceRectangle(EGardenCellType::Air);

CellScript::CellScript(Node* node) : Script(node) {
	// ...
}

weak_ptr<GardenScript> CellScript::GetGarden() const noexcept {
	return garden;
}

EGardenCellType CellScript::GetGardenCellType() const noexcept {
	return gardenCellType;
}

void CellScript::UpdateProperties(EGardenCellType gardenCellType, weak_ptr<GardenScript> garden) {
	this->gardenCellType = gardenCellType;
	this->garden = garden;

	shared_ptr<Node> background_sprite_renderer_node(GetNode().CreateNewChild());
	shared_ptr<SpriteRendererScript> background_sprite_renderer(background_sprite_renderer_node->EnsureScript<SpriteRendererScript>());
	background_sprite_renderer->SetResourceID(debugCellsTextureResourceID);
	background_sprite_renderer->SetSourceRectangle(airCellSourceRectangle);
	background_sprite_renderer->SetLayerIndex(3U);
	backgroundSpriteRenderer = background_sprite_renderer;
	
	shared_ptr<Node> foreground_sprite_renderer_node(GetNode().CreateNewChild());
	shared_ptr<SpriteRendererScript> foreground_sprite_renderer(foreground_sprite_renderer_node->EnsureScript<SpriteRendererScript>());
	foreground_sprite_renderer->SetResourceID(debugCellsTextureResourceID);
	foreground_sprite_renderer->SetSourceRectangle(GetDebugCellSourceRectangle(gardenCellType));
	foreground_sprite_renderer->SetLayerIndex(2U);
	foregroundSpriteRenderer = foreground_sprite_renderer;
	
	// TODO: Add support for cell texture animation
}

const weak_ptr<SpriteRendererScript>& CellScript::GetBackgroundSpriteRenderer() const noexcept {
	return backgroundSpriteRenderer;
}

const weak_ptr<SpriteRendererScript>& CellScript::GetForegroundSpriteRenderer() const noexcept {
	return foregroundSpriteRenderer;
}

bool CellScript::IsSolid() const noexcept {
	return false;
}

bool CellScript::IsTopWalkable() const noexcept {
	return IsSolid() || IsClimbingUpAllowed();
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

bool CellScript::Dig() noexcept {
	return false;
}

bool CellScript::Interact() noexcept {
	return false;
}
