#pragma once

#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class SolidCellScript : public CellScript {
	public:

		OGP_API SolidCellScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsSolid() const noexcept override;
	};
}
