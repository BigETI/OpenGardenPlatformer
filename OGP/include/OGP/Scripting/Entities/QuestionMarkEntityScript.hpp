#pragma once

#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class QuestionMarkEntityScript : public EntityScript {
	public:

		OGP_API QuestionMarkEntityScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool Interact(EntityScript& sourceEntity) override;
	};
}
