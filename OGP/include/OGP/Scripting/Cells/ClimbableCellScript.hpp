#pragma once

#include <chrono>

#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../SceneManagement/Node.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class ClimbableCellScript : public CellScript {
	public:

		OGP_API ClimbableCellScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool IsClimbable() const noexcept override;
		OGP_API virtual bool IsClimbingUpAllowed() const noexcept override;

	protected:

		OGP_API virtual void OnFrameRender(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;
	};
}
