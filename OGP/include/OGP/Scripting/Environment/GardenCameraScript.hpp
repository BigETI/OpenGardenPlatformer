#pragma once

#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/CameraScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Exportables/Exportable.hxx"
#include "../Entities/EntityScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenCameraScript : public Klein::Scripting::Script {
	public:

		OGP_API GardenCameraScript(Klein::SceneManagement::Node* node);

		OGP_API const std::weak_ptr<OGP::Scripting::Entities::EntityScript>& GetToSpectateEntity() const noexcept;
		OGP_API const Klein::Math::Vector2<std::size_t>& GetGardenBounds() const noexcept;
		OGP_API void Spectate(const std::weak_ptr<OGP::Scripting::Entities::EntityScript>& toSpectateEntity, const Klein::Math::Vector2<std::size_t>& gardenBounds) noexcept;

	protected:

		OGP_API virtual void OnInitialize(Klein::Engine& engine) override;
		OGP_API virtual void OnFrameRender(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		float cameraZoom;
		std::chrono::high_resolution_clock::duration elapsedCameraAnimationTime;
		float cameraMovementSmoothing;
		std::weak_ptr<Klein::Scripting::Rendering::CameraScript> camera;
		Klein::Engine* engine;
		std::weak_ptr<OGP::Scripting::Entities::EntityScript> toSpectateEntity;
		Klein::Math::Vector2<std::size_t> gardenBounds;

		Klein::Math::Vector2<float> GetTargetPosition() const noexcept;
	};
}
