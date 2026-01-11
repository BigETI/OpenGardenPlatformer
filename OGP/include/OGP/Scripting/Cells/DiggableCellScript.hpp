#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Environment/ESideFlags.hpp"
#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class DiggableCellScript : public CellScript {
	public:

		OGP_API DiggableCellScript(Klein::SceneManagement::Node* node);

		OGP_API bool IsSolid() const noexcept override;
		OGP_API OGP::Environment::ESideFlags GetConnectableSideFlags() const noexcept override;
		OGP_API bool Dig() noexcept override;

	protected:

		OGP_API virtual void OnFrameRender(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		std::chrono::high_resolution_clock::time_point lastDigTimePoint;
	};
}
