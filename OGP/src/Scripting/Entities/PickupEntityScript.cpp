#include <memory>

#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Entities/EGardenEntityType.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/PickupEntityScript.hpp>
#include <OGP/Scripting/Entities/PlayerEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace std;

using namespace Klein::Math;
using namespace Klein::SceneManagement;

using namespace OGP::Entities;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

PickupEntityScript::PickupEntityScript(Node* node) : EntityScript(node) {
	// ...
}

bool PickupEntityScript::Interact(EntityScript& sourceEntity) {
	bool ret(false);
	PlayerEntityScript* source_player_entity(dynamic_cast<PlayerEntityScript*>(&sourceEntity));
	if (source_player_entity && (GetCurrentPosition() == sourceEntity.GetCurrentPosition())) {
		if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
			ret = true;
			switch (GetGardenEntityData().type) {
			case EGardenEntityType::RedKey:
				source_player_entity->AddRedKey();
				break;
			case EGardenEntityType::YellowKey:
				source_player_entity->AddYellowKey();
				break;
			case EGardenEntityType::GreenKey:
				source_player_entity->AddGreenKey();
				break;
			case EGardenEntityType::Apple:
				garden->DecrementHarvestableCount();
				source_player_entity->AddScore(static_cast<size_t>(100));
				break;
			case EGardenEntityType::Lemon:
				garden->DecrementHarvestableCount();
				source_player_entity->AddScore(static_cast<size_t>(80));
				break;
			case EGardenEntityType::Cherry:
			case EGardenEntityType::Spinach:
				garden->DecrementHarvestableCount();
				source_player_entity->AddScore(static_cast<size_t>(60));
				break;
			case EGardenEntityType::Pineapple:
			case EGardenEntityType::Carrot:
				garden->DecrementHarvestableCount();
				source_player_entity->AddScore(static_cast<size_t>(40));
				break;
			case EGardenEntityType::Garlic:
				garden->DecrementHarvestableCount();
				source_player_entity->ActivateGarlicEffect();
				source_player_entity->AddScore(static_cast<size_t>(100));
				break;
			case EGardenEntityType::Mushroom:
				garden->DecrementHarvestableCount();
				source_player_entity->ActivateMushroomEffect();
				source_player_entity->AddScore(static_cast<size_t>(80));
				break;
			case EGardenEntityType::Sunflower:
				source_player_entity->AddScore(static_cast<size_t>(100));
				break;
			case EGardenEntityType::Tulip:
				source_player_entity->AddScore(static_cast<size_t>(80));
				break;
			case EGardenEntityType::YellowDaisy:
				source_player_entity->AddScore(static_cast<size_t>(60));
				break;
			case EGardenEntityType::Rose:
				source_player_entity->AddScore(static_cast<size_t>(40));
				break;
			default:
				ret = false;
			}
			if (ret) {
				GetNode().Destroy();
			}
		}
	}
	return ret;
}

void PickupEntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	EntityScript::Spawn(gardenEntityData, garden);
	switch (GetGardenEntityData().type) {
	case EGardenEntityType::Apple:
	case EGardenEntityType::Lemon:
	case EGardenEntityType::Cherry:
	case EGardenEntityType::Spinach:
	case EGardenEntityType::Pineapple:
	case EGardenEntityType::Carrot:
	case EGardenEntityType::Garlic:
	case EGardenEntityType::Mushroom:
		garden->IncrementHarvestableCount();
		break;
	default:
		break;
	}
}
