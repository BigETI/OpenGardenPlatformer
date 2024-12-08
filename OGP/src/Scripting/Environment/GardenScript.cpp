#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include <magic_enum.hpp>

#include <Klein/Engine.hpp>
#include <Klein/Collections/EGridResizingRule.hpp>
#include <Klein/Collections/ResizableGrid.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include <OGP/Cells/EGardenCellType.hpp>
#include <OGP/Entities/EGardenEntityType.hpp>
#include <OGP/Environment/GardenData.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/ClimbableCellScript.hpp>
#include <OGP/Scripting/Cells/DeadlyCellScript.hpp>
#include <OGP/Scripting/Cells/DiggableCellScript.hpp>
#include <OGP/Scripting/Cells/DoorCellScript.hpp>
#include <OGP/Scripting/Cells/MoleHillCellScript.hpp>
#include <OGP/Scripting/Cells/SolidCellScript.hpp>
#include <OGP/Scripting/Cells/TopDeadlyCellScript.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/LiftEntityScript.hpp>
#include <OGP/Scripting/Entities/MarmotEntityScript.hpp>
#include <OGP/Scripting/Entities/PickupEntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Entities/QuestionMarkEntityScript.hpp>
#include <OGP/Scripting/Entities/WormEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenCameraScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;

using namespace Klein;
using namespace Klein::Collections;
using namespace Klein::Math;
using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Cells;
using namespace OGP::Entities;
using namespace OGP::Environment;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

GardenScript::GardenScript(Node* node) :
	Script(node),
	timeInGameSeconds(static_cast<size_t>(0)),
	score(static_cast<size_t>(0)),
	harvestableCount(static_cast<size_t>(0)),
	redKeyCount(static_cast<size_t>(0)),
	yellowKeyCount(static_cast<size_t>(0)),
	greenKeyCount(static_cast<size_t>(0)) {
	// ...
}

const string& GardenScript::GetGardenName() const noexcept {
	return gardenName;
}

string& GardenScript::GetGardenName(std::string& result) const {
	return result = gardenName;
}

size_t GardenScript::GetTimeInGameSeconds() const noexcept {
	return timeInGameSeconds;
}
size_t GardenScript::GetScore() const noexcept {
	return score;
}

void GardenScript::SetScore(size_t score) noexcept {
	this->score = score;
}

void GardenScript::AddScore(size_t score) noexcept {
	SetScore(this->score + score);
}

const ResizableGrid<weak_ptr<CellScript>>& GardenScript::GetGardenCells() const noexcept {
	return gardenCells;
}

ResizableGrid<weak_ptr<CellScript>>& GardenScript::GetGardenCells() noexcept {
	return gardenCells;
}

const vector<weak_ptr<EntityScript>>& GardenScript::GetEntities() const noexcept {
	return entities;
}

bool GardenScript::TryGettingEntity(const EntityScript& entity, shared_ptr<EntityScript>& result) const noexcept {
	bool ret(false);
	for (const auto& target_entity : entities) {
		if (shared_ptr<EntityScript> current_target_entity = target_entity.lock()) {
			if (current_target_entity.get() == &entity) {
				result = current_target_entity;
				ret = true;
				break;
			}
		}
	}
	return ret;
}

bool GardenScript::RemoveEntity(shared_ptr<EntityScript> entity) noexcept {
	bool ret(false);
	for (int index(static_cast<int>(entities.size()) - 1); index >= 0; index--) {
		if (shared_ptr<EntityScript> current_entity = entities.at(index).lock()) {
			if (current_entity == entity) {
				entities.erase(entities.begin() + static_cast<size_t>(index));
				ret = true;
			}
		}
		else {
			entities.erase(entities.begin() + static_cast<size_t>(index));
		}
	}
	return ret;
}

size_t GardenScript::GetHarvestableCount() const noexcept {
	return harvestableCount;
}

void GardenScript::IncrementHarvestableCount() noexcept {
	harvestableCount++;
}

void GardenScript::DecrementHarvestableCount() noexcept {
	if (harvestableCount > static_cast<size_t>(0)) {
		--harvestableCount;
	}
}

size_t GardenScript::GetRedKeyCount() const noexcept {
	return redKeyCount;
}

size_t GardenScript::GetYellowKeyCount() const noexcept {
	return yellowKeyCount;
}

size_t GardenScript::GetGreenKeyCount() const noexcept {
	return greenKeyCount;
}

void GardenScript::AddRedKey() noexcept {
	++redKeyCount;
}

bool GardenScript::UseRedKey() noexcept {
	bool ret(redKeyCount > static_cast<size_t>(0));
	if (ret) {
		--redKeyCount;
	}
	return ret;
}

void GardenScript::AddYellowKey() noexcept {
	++yellowKeyCount;
}

bool GardenScript::UseYellowKey() noexcept {
	bool ret(yellowKeyCount > static_cast<size_t>(0));
	if (ret) {
		--yellowKeyCount;
	}
	return ret;
}

void GardenScript::AddGreenKey() noexcept {
	++greenKeyCount;
}

bool GardenScript::UseGreenKey() noexcept {
	bool ret(greenKeyCount > static_cast<size_t>(0));
	if (ret) {
		--greenKeyCount;
	}
	return ret;
}

bool GardenScript::IsCompletionEnabled() const noexcept {
	return harvestableCount == static_cast<size_t>(0);
}

void GardenScript::LoadGardenFromGardenData(const GardenData& gardenData) {
	shared_ptr<GardenScript> garden;
	if (GetNode().TryGettingScript<GardenScript>(garden)) {
		gardenName = gardenData.name;
		timeInGameSeconds = gardenData.timeInGameSeconds;
		for (auto cell : gardenCells.GetCells()) {
			if (shared_ptr<CellScript> current_cell = cell.lock()) {
				current_cell->GetNode().Destroy();
			}
		}
		gardenCells.Resize(gardenData.cells.GetSize(), EGridResizingRule::Naive);
		Vector2<size_t> cell_grid_size(gardenCells.GetSize());
		EGardenCellType garden_cell_type;
		for (Vector2<size_t> cell_position; cell_position.y != cell_grid_size.y; cell_position.y++) {
			for (cell_position.x = static_cast<size_t>(0U); cell_position.x != cell_grid_size.x; cell_position.x++) {
				if (gardenData.cells.TryGettingCell(cell_position, garden_cell_type)) {
					shared_ptr<Node> cell_node(GetNode().CreateNewChild());
					shared_ptr<CellScript> cell;
					switch (garden_cell_type) {
					case EGardenCellType::Fence:
					case EGardenCellType::Bush:
						cell = cell_node->AddScript<TopDeadlyCellScript>();
						break;
					case EGardenCellType::Ground:
					case EGardenCellType::Trap:
					case EGardenCellType::GrassGround:
					case EGardenCellType::GrassTrap:
						cell = cell_node->AddScript<DiggableCellScript>();
						break;
					case EGardenCellType::HardGround:
					case EGardenCellType::HardGrassGround:
						cell = cell_node->AddScript<SolidCellScript>();
						break;
					case EGardenCellType::MoleHill:
						cell = cell_node->AddScript<MoleHillCellScript>();
						break;
					case EGardenCellType::Ladder:
					case EGardenCellType::LadderShowingUpWhenFinished:
					case EGardenCellType::LadderVanishingWhenFinished:
					case EGardenCellType::Rope:
						cell = cell_node->AddScript<ClimbableCellScript>();
						break;
					case EGardenCellType::RedDoor:
					case EGardenCellType::AutomaticallyClosingRedDoor:
					case EGardenCellType::YellowDoor:
					case EGardenCellType::AutomaticallyClosingYellowDoor:
					case EGardenCellType::GreenDoor:
					case EGardenCellType::AutomaticallyClosingGreenDoor:
						cell = cell_node->AddScript<DoorCellScript>();
						break;
					case EGardenCellType::Water:
					case EGardenCellType::SurfaceWater:
						cell = cell_node->AddScript<DeadlyCellScript>();
						break;
					default:
						cell = cell_node->AddScript<CellScript>();
					}
					cell->UpdateProperties(garden_cell_type, garden);
					cell_node->SetLocalPosition(cell_position.GetConverted<float>());
					gardenCells.SetCell(cell_position, cell);
				}
			}
		}
		for (const auto& entity_data : gardenData.entities) {
			shared_ptr<Node> entity_node(GetNode().CreateNewChild(string(magic_enum::enum_name(entity_data.type))));
			shared_ptr<EntityScript> entity;
			switch (entity_data.type) {
			case EGardenEntityType::Player:
			{
				shared_ptr<PlayerEntityScript> player_entity(entity_node->AddScript<PlayerEntityScript>());
				entity = player_entity;
				player_entity->OnDied += [OnFailed = &OnFailed]() {
					(*OnFailed)();
				};
				player_entity->OnWon += [OnCompleted = &OnCompleted]() {
					(*OnCompleted)();
				};
				shared_ptr<GardenCameraScript> garden_camera(GetNode().CreateNewChild("Camera")->AddScript<GardenCameraScript>());
				garden_camera->SetToSpectateEntity(player_entity);
				break;
			}
			case EGardenEntityType::Marmot:
				entity = entity_node->AddScript<MarmotEntityScript>();
				break;
			case EGardenEntityType::LeftMovingWorm:
			case EGardenEntityType::RightMovingWorm:
				entity = entity_node->AddScript<WormEntityScript>();
				break;
			case EGardenEntityType::UpMovingLift:
			case EGardenEntityType::DownMovingLift:
			case EGardenEntityType::LeftMovingLift:
			case EGardenEntityType::RightMovingLift:
				entity = entity_node->AddScript<LiftEntityScript>();
				break;
			case EGardenEntityType::RedKey:
			case EGardenEntityType::YellowKey:
			case EGardenEntityType::GreenKey:
			case EGardenEntityType::Apple:
			case EGardenEntityType::Lemon:
			case EGardenEntityType::Cherry:
			case EGardenEntityType::Pineapple:
			case EGardenEntityType::Garlic:
			case EGardenEntityType::Mushroom:
			case EGardenEntityType::Spinach:
			case EGardenEntityType::Carrot:
			case EGardenEntityType::Sunflower:
			case EGardenEntityType::Tulip:
			case EGardenEntityType::YellowDaisy:
			case EGardenEntityType::Rose:
				entity = entity_node->AddScript<PickupEntityScript>();
				break;
			case EGardenEntityType::QuestionMark:
				entity = entity_node->AddScript<QuestionMarkEntityScript>();
				break;
			default:
				entity = entity_node->AddScript<EntityScript>();
			}
			entity->OnDeinitialized += [this, entity = weak_ptr<EntityScript>(entity)](Engine& engine) {
				if (shared_ptr<EntityScript> current_entity = entity.lock()) {
					this->RemoveEntity(current_entity);
				}
			};
			entity->Spawn(entity_data, garden);
			entities.push_back(entity);
		}
	}
	else {
		cerr << "Failed to get script reference to garden." << endl;
	}
}

shared_ptr<CellScript> GardenScript::GetCellAt(const Vector2<size_t>& position) const noexcept {
	shared_ptr<CellScript> ret;
	weak_ptr<CellScript> garden_cell;
	if (gardenCells.TryGettingCell(position, garden_cell)) {
		ret = garden_cell.lock();
	}
	return ret;
}

vector<shared_ptr<EntityScript>>& GardenScript::GetEntitiesAt(const Vector2<size_t>& position, vector<shared_ptr<EntityScript>>& result) const noexcept {
	result.clear();
	for (auto entity : entities) {
		if (shared_ptr<EntityScript> current_entity = entity.lock()) {
			if (current_entity->GetCurrentPosition() == position) {
				result.push_back(current_entity);
			}
		}
	}
	return result;
}

bool GardenScript::IsSolidAt(const Vector2<size_t>& position) const noexcept {
	shared_ptr<CellScript> garden_cell(GetCellAt(position));
	return !garden_cell || garden_cell->IsSolid();
}

bool GardenScript::IsWalkableAt(const Vector2<size_t>& position) const noexcept {
	shared_ptr<CellScript> garden_cell(GetCellAt(position - Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1))));
	return (position.y == static_cast<size_t>(0)) || (garden_cell && garden_cell->IsTopWalkable());
}

bool GardenScript::IsClimbableAt(const Vector2<size_t>& position) const noexcept {
	shared_ptr<CellScript> garden_cell(GetCellAt(position));
	return !garden_cell || garden_cell->IsClimbable();
}

bool GardenScript::IsClimbingUpAllowedAt(const Vector2<size_t>& position) const noexcept {
	shared_ptr<CellScript> garden_cell(GetCellAt(position));
	return garden_cell && garden_cell->IsClimbingUpAllowed() && !IsSolidAt(position + Vector2<size_t>(static_cast<size_t>(0), static_cast<size_t>(1)));
}

bool GardenScript::IsDeadlyAt(const Vector2<size_t>& position) const noexcept {
	shared_ptr<CellScript> garden_cell(GetCellAt(position));
	return garden_cell && garden_cell->IsDeadly();
}

bool GardenScript::IsTopDeadlyAt(const Vector2<size_t>& position) const noexcept {
	shared_ptr<CellScript> garden_cell(GetCellAt(position));
	return garden_cell && garden_cell->IsTopDeadly();
}

bool GardenScript::IsWinnableAt(const Vector2<size_t>& position) const noexcept {
	bool ret(false);
	if (IsCompletionEnabled()) {
		shared_ptr<CellScript> garden_cell(GetCellAt(position));
		ret = ((position.y + static_cast<size_t>(1)) >= gardenCells.GetSize().y) && garden_cell && garden_cell->IsClimbingUpAllowed();
	}
	return ret;
}

bool GardenScript::DigAt(const Vector2<size_t>& position) noexcept {
	shared_ptr<CellScript> garden_cell(GetCellAt(position));
	return garden_cell && garden_cell->Dig();
}

bool GardenScript::InteractAt(const Vector2<size_t>& position, EntityScript& sourceEntity) noexcept {
	bool ret(false);
	shared_ptr<CellScript> garden_cell(GetCellAt(position));
	vector<shared_ptr<EntityScript>> target_entities;
	ret = garden_cell && garden_cell->Interact();
	for (const auto& target_entity : GetEntitiesAt(position, target_entities)) {
		if (target_entity.get() != &sourceEntity) {
			ret = target_entity->Interact(sourceEntity) || ret;
		}
	}
	return ret;
}
