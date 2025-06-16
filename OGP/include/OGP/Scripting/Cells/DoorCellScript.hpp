#pragma once

#include <chrono>

#include <Klein/Engine.hpp>
#include <Klein/EventSystem/Event.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Exportables/Exportable.hxx"
#include "../Entities/EntityScript.hpp"
#include "CellScript.hpp"

namespace OGP::Scripting::Cells {
	class DoorCellScript : public CellScript {
	public:

		Klein::EventSystem::Event<> OnOpened;
		Klein::EventSystem::Event<> OnClosed;

		OGP_API DoorCellScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsSolid() const noexcept override;
		OGP_API virtual bool IsTopDeadly() const noexcept override;
		OGP_API virtual bool Interact(OGP::Scripting::Entities::EntityScript& sourceEntity) noexcept override;

	protected:

		OGP_API bool Open();
		OGP_API bool Close();
		OGP_API virtual void OnGameTick(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;
		OGP_API virtual void OnFrameRender(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		bool isOpen;
		bool wasInteractedWithLastFrame;
	};
}
