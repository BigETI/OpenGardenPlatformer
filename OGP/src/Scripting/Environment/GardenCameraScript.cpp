#include <chrono>
#include <memory>

#include <OGP/Game.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Environment/GardenCameraScript.hpp>
#include <OGP/Scripting/Rendering/CameraScript.hpp>
#include <OGP/Scripting/Script.hpp>

using namespace OGP;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;
using namespace OGP::Scripting::Rendering;
using namespace std;
using namespace std::chrono;

GardenCameraScript::GardenCameraScript(Node* node) : Script(node) {
	// ...
}

weak_ptr<EntityScript> GardenCameraScript::GetToSpectateEntity() const noexcept {
	return toSpectateEntity;
}

void GardenCameraScript::SetToSpectateEntity(weak_ptr<EntityScript> toSpectateEntity) {
	this->toSpectateEntity = toSpectateEntity;
}

void GardenCameraScript::OnInitialize(Game& game) {
	GetNode().AddScript<CameraScript>()->SetCameraZoom(64.0f);
}

void GardenCameraScript::OnFrameRender(Game& game, high_resolution_clock::duration deltaTime) {
	
	// TODO: Add support for constraining view at garden bounds
	if (shared_ptr<EntityScript> to_spectate_entity = toSpectateEntity.lock()) {
		GetNode().SetLocalPosition(to_spectate_entity->GetToBeRenderedPosition());
	}
}
