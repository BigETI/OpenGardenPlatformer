#pragma once

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include "../../Entities/HumanoidInput.hpp"
#include "../../Exportables/Exportable.hxx"
#include "HumanoidEntityScript.hpp"

namespace OGP::Scripting::Entities {
	class MarmotEntityScript : public HumanoidEntityScript {
	public:

		OGP_API MarmotEntityScript(Klein::SceneManagement::Node* node);

		OGP_API virtual bool IsDeadly() const noexcept override;

	protected:

		OGP_API virtual OGP::Entities::HumanoidInput GetInput(const Klein::Engine& engine) const noexcept override;
	};
}
