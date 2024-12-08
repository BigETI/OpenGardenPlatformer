#pragma once

#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class TopDeadlyCellScript : public CellScript {
	public:

		OGP_API TopDeadlyCellScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsTopDeadly() const noexcept override;
	};
}
