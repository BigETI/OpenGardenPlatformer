#pragma once

#include <chrono>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Exportables/Exportable.hxx"
#include "../Entities/EntityScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenCameraScript : public Klein::Scripting::Script {
	public:

		OGP_API GardenCameraScript(Klein::SceneManagement::Node* node);

		OGP_API std::weak_ptr<OGP::Scripting::Entities::EntityScript> GetToSpectateEntity() const noexcept;
		OGP_API void SetToSpectateEntity(std::weak_ptr<OGP::Scripting::Entities::EntityScript> toSpectateEntity);

	protected:

		OGP_API virtual void OnInitialize(Klein::Engine& engine) override;
		OGP_API virtual void OnFrameRender(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		std::weak_ptr<OGP::Scripting::Entities::EntityScript> toSpectateEntity;
	};
}
