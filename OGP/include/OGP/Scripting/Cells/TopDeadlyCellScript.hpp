#pragma once

#include "../../Exportables/Exportable.hxx"
#include "../../SceneManagement/Node.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class TopDeadlyCellScript : public CellScript {
	public:

		OGP_API TopDeadlyCellScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool IsTopDeadly() const noexcept override;
	};
}
