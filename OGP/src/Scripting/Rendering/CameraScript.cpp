#include <cstddef>
#include <chrono>

#include <OGP/Game.hpp>
#include <OGP/Rendering/RenderingContext.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Rendering/CameraScript.hpp>
#include <OGP/Scripting/Script.hpp>

using namespace OGP;
using namespace OGP::Rendering;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting;
using namespace OGP::Scripting::Rendering;
using namespace std;
using namespace std::chrono;

CameraScript::CameraScript(Node* node) : Script(node), cameraIndex(static_cast<size_t>(0U)), cameraZoom(1.0f) {
	// ...
}

size_t CameraScript::GetCameraIndex() const noexcept {
	return cameraIndex;
}

void CameraScript::SetCameraIndex(size_t cameraIndex) noexcept {
	this->cameraIndex = cameraIndex;
}

float CameraScript::GetCameraZoom() const noexcept {
	return cameraZoom;
}

void CameraScript::SetCameraZoom(float cameraZoom) noexcept {
	this->cameraZoom = cameraZoom;
}

void CameraScript::OnFrameRender(Game& game, high_resolution_clock::duration deltaTime) {
	if (cameraIndex >= game.GetRenderingContexts().size()) {
		game.GetRenderingContexts().resize(cameraIndex + static_cast<size_t>(1U));
	}
	RenderingContext& rendering_context(game.GetRenderingContexts().at(cameraIndex));
	rendering_context.SetCameraPosition(GetNode().GetPosition());
	rendering_context.SetCameraRotation(GetNode().GetRotation());
	rendering_context.SetCameraZoom(cameraZoom);
}
