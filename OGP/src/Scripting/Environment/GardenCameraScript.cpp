#include <chrono>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Scripting/Rendering/CameraScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Environment/GardenCameraScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Scripting;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

GardenCameraScript::GardenCameraScript(Node* node) : Script(node) {
	// ...
}

weak_ptr<EntityScript> GardenCameraScript::GetToSpectateEntity() const noexcept {
	return toSpectateEntity;
}

void GardenCameraScript::SetToSpectateEntity(weak_ptr<EntityScript> toSpectateEntity) {
	this->toSpectateEntity = toSpectateEntity;
}

void GardenCameraScript::OnInitialize(Engine& engine) {
	GetNode().AddScript<CameraScript>()->SetCameraZoom(64.0f);
}

void GardenCameraScript::OnFrameRender(Engine& engine, high_resolution_clock::duration deltaTime) {
	
	// TODO: Add support for constraining view at garden bounds
	if (shared_ptr<EntityScript> to_spectate_entity = toSpectateEntity.lock()) {
		GetNode().SetLocalPosition(to_spectate_entity->GetToBeRenderedPosition());
	}
}
