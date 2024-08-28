#pragma once

#include "../../Exportables/Exportable.hxx"
#include "../../SceneManagement/Node.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class QuestionMarkEntityScript : public EntityScript {
	public:

		OGP_API QuestionMarkEntityScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool Interact(EntityScript& sourceEntity) override;
	};
}
