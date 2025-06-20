#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Cells/EMoleHillState.hpp"
#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class MoleHillCellScript : public CellScript {
	public:

		OGP_API MoleHillCellScript(Klein::SceneManagement::Node* node);

		OGP_API OGP::Cells::EMoleHillState GetMoleHillState() const noexcept;
		OGP_API virtual bool IsDeadly() const noexcept override;

	protected:

		OGP_API virtual void OnInitialize(Klein::Engine& engine) override;
		OGP_API virtual void OnFrameRender(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		OGP::Cells::EMoleHillState moleHillState;
		std::chrono::high_resolution_clock::duration elapsedTime;
	};
}
