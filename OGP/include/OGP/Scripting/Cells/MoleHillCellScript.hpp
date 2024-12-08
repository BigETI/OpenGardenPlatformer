#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class MoleHillCellScript : public CellScript {
	public:

		OGP_API MoleHillCellScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsDeadly() const noexcept override;

	protected:

		OGP_API virtual void OnInitialize(Klein::Engine& engine) override;
		OGP_API virtual void OnFrameRender(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		std::chrono::high_resolution_clock::time_point spawnTime;
	};
}
