#pragma once

#include "../../Exportables/Exportable.hxx"
#include "../../SceneManagement/Node.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class SolidCellScript : public CellScript {
	public:

		OGP_API SolidCellScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool IsSolid() const noexcept override;
	};
}
