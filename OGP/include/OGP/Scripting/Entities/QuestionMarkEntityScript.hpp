#pragma once

#include "../../Exportables/Exportable.hxx"
#include "../../Math/Vector2.hpp"
#include "../../SceneManagement/Node.hpp"
#include "EntityScript.hpp"

namespace OGP::Scripting::Entities {
	class QuestionMarkEntityScript : public EntityScript {
	public:

		OGP_API QuestionMarkEntityScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool Interact(const OGP::Math::Vector2<int>& relativeSourcePosition) override;
	};
}
