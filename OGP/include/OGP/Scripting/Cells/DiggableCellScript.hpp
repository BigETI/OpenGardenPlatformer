#pragma once

#include <chrono>

#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../SceneManagement/Node.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class DiggableCellScript : public CellScript {
	public:

		OGP_API DiggableCellScript(OGP::SceneManagement::Node* node);

		OGP_API virtual bool IsSolid() const noexcept override;
		OGP_API virtual bool Dig() noexcept override;

	protected:

		OGP_API virtual void OnFrameRender(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		std::chrono::high_resolution_clock::time_point lastDigTimePoint;
	};
}
