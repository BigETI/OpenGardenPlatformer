#pragma once

#include "../../Exportables/Exportable.hxx"
#include "../../SceneManagement/Node.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class DeadlyCellScript : public CellScript {
	public:

		OGP_API DeadlyCellScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool IsDeadly() const noexcept override;
	};
}
