#pragma once

#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class DeadlyCellScript : public CellScript {
	public:

		OGP_API DeadlyCellScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsDeadly() const noexcept override;
	};
}
