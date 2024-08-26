#pragma once

#include <chrono>
#include <cstddef>

#include "../../Exportables/Exportable.hxx"
#include "../../Game.hpp"
#include "../../SceneManagement/Node.hpp"
#include "../Script.hpp"

namespace OGP::Scripting::Rendering {
	class CameraScript : public Script {
	public:

		OGP_API CameraScript(OGP::SceneManagement::Node* node);

		OGP_API std::size_t GetCameraIndex() const noexcept;
		OGP_API void SetCameraIndex(std::size_t cameraIndex) noexcept;
		OGP_API float GetCameraZoom() const noexcept;
		OGP_API void SetCameraZoom(float cameraZoom) noexcept;

	protected:

		OGP_API virtual void OnFrameRender(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		std::size_t cameraIndex;
		float cameraZoom;
	};
}
