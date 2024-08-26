#pragma once

#include "../../Exportables/Exportable.hxx"
#include "../../SceneManagement/Node.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class ClimbableCellScript : public CellScript {
	public:

		OGP_API ClimbableCellScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool IsClimbable() const noexcept override;
		OGP_API virtual bool IsClimbingUpAllowed() const noexcept override;
	};
}
