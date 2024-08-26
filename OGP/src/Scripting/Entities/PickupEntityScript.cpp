#include <memory>

#include <OGP/Environment/EGardenEntityType.hpp>
#include <OGP/Math/Vector2.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/PickupEntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>

using namespace OGP::Environment;
using namespace OGP::Math;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;
using namespace std;

PickupEntityScript::PickupEntityScript(Node* node) : EntityScript(node) {
	// ...
}

bool PickupEntityScript::Interact(const Vector2<int>& relativeSourcePosition) {
	bool ret(false);
	if (relativeSourcePosition == Vector2<int>()) {
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
				garden->AddScore(static_cast<size_t>(100));
				break;
			case EGardenEntityType::Lemon:
			case EGardenEntityType::Tulip:
				garden->AddScore(static_cast<size_t>(80));
				break;
			case EGardenEntityType::Cherry:
			case EGardenEntityType::Spinach:
			case EGardenEntityType::YellowDaisy:
				garden->AddScore(static_cast<size_t>(60));
				break;
			case EGardenEntityType::Pineapple:
			case EGardenEntityType::Carrot:
			case EGardenEntityType::Rose:
				garden->AddScore(static_cast<size_t>(40));
				break;
			case EGardenEntityType::Garlic:
				garden->AddScore(static_cast<size_t>(100));
				// TODO: Implement logic for garlic
				break;
			case EGardenEntityType::Mushroom:
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
