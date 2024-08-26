#pragma once

#include <chrono>
#include <memory>

#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../SceneManagement/Node.hpp"
#include "../Entities/EntityScript.hpp"
#include "../Script.hpp"

namespace OGP::Scripting::Environment {
	class GardenCameraScript : public OGP::Scripting::Script {
	public:

		OGP_API GardenCameraScript(OGP::SceneManagement::Node* node);

		OGP_API std::weak_ptr<OGP::Scripting::Entities::EntityScript> GetToSpectateEntity() const noexcept;
		OGP_API void SetToSpectateEntity(std::weak_ptr<OGP::Scripting::Entities::EntityScript> toSpectateEntity);

	protected:

		OGP_API virtual void OnInitialize(OGP::Game& game) override;
		OGP_API virtual void OnFrameRender(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		std::weak_ptr<OGP::Scripting::Entities::EntityScript> toSpectateEntity;
	};
}
