#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class DoorCellScript : public CellScript {
	public:

		OGP_API DoorCellScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsSolid() const noexcept override;
		OGP_API virtual bool IsTopDeadly() const noexcept override;
		OGP_API virtual bool Interact() noexcept override;

	protected:

		OGP_API virtual void OnFrameRender(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		bool isOpen;
	};
}
