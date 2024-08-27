#pragma once

#include "../../Entities/GardenEntityData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Math/Vector2.hpp"
#include "../../SceneManagement/Node.hpp"
#include "../Environment/GardenScript.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class PickupEntityScript : public EntityScript {
	public:

		OGP_API PickupEntityScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool Interact(const OGP::Math::Vector2<int>& relativeSourcePosition) override;

	protected:

		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden) override;
	};
}
