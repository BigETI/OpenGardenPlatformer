#pragma once

#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "SolidCellScript.hpp"

namespace OGP::Scripting::Cells {
	class DeadlyCellScript : public SolidCellScript {
	public:

		OGP_API DeadlyCellScript(Klein::SceneManagement::Node* node);

		OGP_API bool IsDeadly() const noexcept override;
	};
}
