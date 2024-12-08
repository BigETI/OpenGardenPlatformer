#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class ClimbableCellScript : public CellScript {
	public:

		OGP_API ClimbableCellScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsClimbable() const noexcept override;
		OGP_API virtual bool IsClimbingUpAllowed() const noexcept override;

	protected:

		OGP_API virtual void OnFrameRender(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;
	};
}
