#include <memory>

#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Entities/EGardenEntityType.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/PickupEntityScript.hpp>
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
	if (GetCurrentPosition() == sourceEntity.GetCurrentPosition()) {
		if (shared_ptr<GardenScript> garden = GetGarden().lock()) {
			ret = true;
			switch (GetGardenEntityData().type) {
			case EGardenEntityType::RedKey:
				garden->AddRedKey();
				break;
			case EGardenEntityType::YellowKey:
				garden->AddYellowKey();
				break;
			case EGardenEntityType::GreenKey:
				garden->AddGreenKey();
				break;
			case EGardenEntityType::Apple:
			case EGardenEntityType::Sunflower:
				garden->DecrementHarvestableCount();
				garden->AddScore(static_cast<size_t>(100));
				break;
			case EGardenEntityType::Lemon:
			case EGardenEntityType::Tulip:
				garden->DecrementHarvestableCount();
				garden->AddScore(static_cast<size_t>(80));
				break;
			case EGardenEntityType::Cherry:
			case EGardenEntityType::Spinach:
			case EGardenEntityType::YellowDaisy:
				garden->DecrementHarvestableCount();
				garden->AddScore(static_cast<size_t>(60));
				break;
			case EGardenEntityType::Pineapple:
			case EGardenEntityType::Carrot:
			case EGardenEntityType::Rose:
				garden->DecrementHarvestableCount();
				garden->AddScore(static_cast<size_t>(40));
				break;
			case EGardenEntityType::Garlic:
				garden->DecrementHarvestableCount();
				garden->AddScore(static_cast<size_t>(100));
				// TODO: Implement logic for garlic
				break;
			case EGardenEntityType::Mushroom:
				garden->DecrementHarvestableCount();
				garden->AddScore(static_cast<size_t>(80));
				// TODO: Implement logic for mushroom
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
	case EGardenEntityType::Sunflower:
	case EGardenEntityType::Lemon:
	case EGardenEntityType::Tulip:
	case EGardenEntityType::Cherry:
	case EGardenEntityType::Spinach:
	case EGardenEntityType::YellowDaisy:
	case EGardenEntityType::Pineapple:
	case EGardenEntityType::Carrot:
	case EGardenEntityType::Rose:
	case EGardenEntityType::Garlic:
	case EGardenEntityType::Mushroom:
		garden->IncrementHarvestableCount();
		break;
	default:
		break;
	}
}
