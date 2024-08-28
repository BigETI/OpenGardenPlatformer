#include <chrono>
#include <cstddef>
#include <memory>

#include <OGP/Entities/EGardenEntityType.hpp>
#include <OGP/Entities/GardenEntityData.hpp>
#include <OGP/Game.hpp>
#include <OGP/Math/Vector2.hpp>
#include <OGP/ResourceManagement/ResourceID.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Environment/GardenScript.hpp>
#include <OGP/Scripting/Rendering/SpriteRendererScript.hpp>
#include <OGP/Scripting/Script.hpp>

using namespace OGP;
using namespace OGP::Entities;
using namespace OGP::Math;
using namespace OGP::ResourceManagement;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;
using namespace OGP::Scripting::Rendering;
using namespace std;
using namespace std::chrono;

const ResourceID debugEntitiesTextureResourceID("Debug/Textures/Entities.png");
const Vector2<float> debugEntitySourceRectangleSize(Vector2<float>(1.0f, 1.0f) / 8.0f);

Rectangle<float> GetDebugEntitySourceRectangle(EGardenEntityType gardenEntityType) {
	return Rectangle<float>(Vector2<float>(static_cast<float>(static_cast<int>(gardenEntityType) % 8), static_cast<float>(static_cast<int>(gardenEntityType) / 8)) / 8.0f, debugEntitySourceRectangleSize);
}

EntityScript::EntityScript(Node* node) : Script(node) {
	gardenEntityData.type = EGardenEntityType::Player;
	shared_ptr<SpriteRendererScript> sprite_renderer(node->EnsureScript<SpriteRendererScript>());
	sprite_renderer->SetLayerIndex(1U);
	spriteRenderer = sprite_renderer;
}

const GardenEntityData& EntityScript::GetGardenEntityData() const noexcept {
	return gardenEntityData;
}

const Vector2<size_t>& EntityScript::GetCurrentPosition() const noexcept {
	return currentPosition;
}

Vector2<float> EntityScript::GetToBeRenderedPosition() const noexcept {
	Vector2<float> ret;
	if (shared_ptr<EntityScript> mounted_at_entity = mountedAtEntity.lock()) {
		ret = mounted_at_entity->GetToBeRenderedPosition();
	}
	else {
		ret = currentPosition.GetConverted<float>();
	}
	return ret;
}

void EntityScript::SetCurrentPosition(const Vector2<size_t>& currentPosition) noexcept {
	this->currentPosition = currentPosition;
}

weak_ptr<SpriteRendererScript> EntityScript::GetSpriteRenderer() const noexcept {
	return spriteRenderer;
}

weak_ptr<GardenScript> EntityScript::GetGarden() const noexcept {
	return garden;
}

weak_ptr<EntityScript> EntityScript::GetMountedAtEntity() const noexcept {
	return mountedAtEntity;
}

bool EntityScript::MountAt(const shared_ptr<EntityScript>& mountedAtEntity) noexcept {
	bool ret(false);
	if (mountedAtEntity && (mountedAtEntity.get() != this)) {
		if (shared_ptr<EntityScript> mounted_at_entity = this->mountedAtEntity.lock()) {
			if (mounted_at_entity != mountedAtEntity) {
				this->mountedAtEntity = mountedAtEntity;
				ret = true;
			}
		}
		else {
			this->mountedAtEntity = mountedAtEntity;
			ret = true;
		}
	}
	return ret;
}

bool EntityScript::Dismount() noexcept {
	bool ret(!mountedAtEntity.expired());
	mountedAtEntity.reset();
	return ret;
}

void EntityScript::Spawn(const GardenEntityData& gardenEntityData, shared_ptr<GardenScript> garden) {
	this->gardenEntityData = gardenEntityData;
	if (shared_ptr<SpriteRendererScript> sprite_renderer = spriteRenderer.lock()) {
		sprite_renderer->SetResourceID(debugEntitiesTextureResourceID);
		sprite_renderer->SetSourceRectangle(GetDebugEntitySourceRectangle(gardenEntityData.type));
	}
	this->garden = garden;
	SetCurrentPosition(gardenEntityData.position);
	OnSpawned(gardenEntityData);
}

bool EntityScript::Interact(EntityScript& sourceEntity) {
	return false;
}

void EntityScript::OnBeforeFrameRender(Game& game, high_resolution_clock::duration deltaTime) {
	if (shared_ptr<SpriteRendererScript> sprite_renderer = spriteRenderer.lock()) {
		GetNode().SetLocalPosition(GetToBeRenderedPosition());
	}
}
