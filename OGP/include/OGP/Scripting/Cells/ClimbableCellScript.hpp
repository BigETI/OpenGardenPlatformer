#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Environment/ESideFlags.hpp"
#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class ClimbableCellScript : public CellScript {
	public:

		OGP_API ClimbableCellScript(Klein::SceneManagement::Node* node);

		OGP_API bool IsClimbable() const noexcept override;
		OGP_API bool IsClimbingUpAllowed() const noexcept override;
		OGP_API OGP::Environment::ESideFlags GetConnectableSideFlags() const noexcept override;

	protected:

		OGP_API void OnFrameRender(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;
	};
}
