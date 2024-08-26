#pragma once

#include <chrono>

#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../SceneManagement/Node.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class MoleHillCellScript : public CellScript {
	public:

		OGP_API MoleHillCellScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool IsDeadly() const noexcept override;

	protected:

		OGP_API virtual void OnInitialize(OGP::Game& game) override;
		OGP_API virtual void OnFrameRender(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		std::chrono::high_resolution_clock::time_point spawnTime;
	};
}
