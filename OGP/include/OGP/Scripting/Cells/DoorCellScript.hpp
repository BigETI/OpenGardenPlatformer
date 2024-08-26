#pragma once

#include <chrono>

#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../SceneManagement/Node.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class DoorCellScript : public CellScript {
	public:

		OGP_API DoorCellScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool IsSolid() const noexcept override;
		OGP_API virtual bool IsTopDeadly() const noexcept override;
		OGP_API virtual bool Interact() noexcept override;

	protected:

		OGP_API virtual void OnFrameRender(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		bool isOpen;
	};
}
