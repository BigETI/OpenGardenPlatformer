#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Easing.hpp>
#include <Klein/Math/Utility.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/Rendering/IRenderer.hpp>
#include <Klein/Scripting/Rendering/CameraScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Environment/GardenCameraScript.hpp>

using namespace std;
using namespace std::chrono;
using namespace std::literals::chrono_literals;

using namespace Klein;
using namespace Klein::Math;
using namespace Klein::Rendering;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::Rendering;

using namespace OGP::Scripting;
using namespace OGP::Scripting::Entities;
using namespace OGP::Scripting::Environment;

constexpr static const float beginningCameraZoom(88.0f);
constexpr static const float endingCameraZoom(64.0f);
constexpr static const high_resolution_clock::duration cameraZoomAnimationTime(1s);
constexpr static const float cameraMovementSmoothing(0.125f);

GardenCameraScript::GardenCameraScript(Node* node) :
	Script(node),
	cameraZoom(beginningCameraZoom),
	elapsedCameraAnimationTime(high_resolution_clock::duration::zero()),
	cameraMovementSmoothing(1.0f),
	engine(nullptr) {
	// ...
}

const weak_ptr<EntityScript>& GardenCameraScript::GetToSpectateEntity() const noexcept {
	return toSpectateEntity;
}

const Vector2<size_t>& GardenCameraScript::GetGardenBounds() const noexcept {
	return gardenBounds;
}

void GardenCameraScript::Spectate(const weak_ptr<EntityScript>& toSpectateEntity, const Vector2<size_t>& gardenBounds) noexcept {
	this->toSpectateEntity = toSpectateEntity;
	this->gardenBounds = gardenBounds;
}

void GardenCameraScript::OnInitialize(Engine& engine) {
	this->engine = &engine;
	shared_ptr<CameraScript> camera(GetNode().EnsureScript<CameraScript>());
	camera->SetCameraZoom(cameraZoom);
	GetNode().SetLocalPosition(GetTargetPosition());
	this->camera = camera;
}

void GardenCameraScript::OnFrameRender(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	if (shared_ptr<CameraScript> current_camera = camera.lock()) {
		elapsedCameraAnimationTime = min(elapsedCameraAnimationTime + deltaTime, cameraZoomAnimationTime);
		if (elapsedCameraAnimationTime >= cameraZoomAnimationTime) {
			cameraMovementSmoothing = ::cameraMovementSmoothing;
		}
		auto t = duration<float>(elapsedCameraAnimationTime).count() / duration<float>(cameraZoomAnimationTime).count();
		cameraZoom = GetLinearInterpolated(beginningCameraZoom, endingCameraZoom, Easing::EaseInOut(t));
		current_camera->SetCameraZoom(cameraZoom);
	}

	Vector2<float> target_position(GetTargetPosition());
	GetNode().SetLocalPosition(
		Vector2<float>(
			GetLinearInterpolated(GetNode().GetLocalPosition().x, target_position.x, cameraMovementSmoothing),
			GetLinearInterpolated(GetNode().GetLocalPosition().y, target_position.y, cameraMovementSmoothing)
		)
	);
}

Vector2<float> GardenCameraScript::GetTargetPosition() const noexcept {
	Vector2<float> garden_size(gardenBounds.GetConverted<float>());
	Vector2<float> half_garden_size(garden_size * 0.5f);
	if (!engine) {
		return half_garden_size;
	}
	if (shared_ptr<EntityScript> to_spectate_entity = toSpectateEntity.lock()) {
		if (engine->GetRenderers().size() <= static_cast<size_t>(0)) {
			return half_garden_size;
		}
		const shared_ptr<IRenderer>& renderer(engine->GetRenderers().at(static_cast<size_t>(0)));
		Vector2<float> view_size(renderer->GetWidth() / cameraZoom, renderer->GetHeight() / cameraZoom);
		Vector2<float> half_view_size(view_size * 0.5f);
		Vector2<float> bottom_left_constraint(
			((view_size.x > garden_size.x) ? half_garden_size.x : half_view_size.x) - 0.5f,
			((view_size.y > garden_size.y) ? half_garden_size.y : half_view_size.y) - 0.5f
		);
		Vector2<float> top_right_constraint(
			((view_size.x > garden_size.x) ? half_garden_size.x : (garden_size.x - half_view_size.x)) - 0.5f,
			((view_size.y > garden_size.y) ? half_garden_size.y : (garden_size.y - half_view_size.y)) - 0.5f
		);
		Vector2<float> entity_position(to_spectate_entity->GetToBeRenderedPosition());
		return Vector2<float>(
			Vector2<float>(
				clamp(entity_position.x, bottom_left_constraint.x, top_right_constraint.x),
				clamp(entity_position.y, bottom_left_constraint.y, top_right_constraint.y)
			)
		);
	}
	return half_garden_size;
}
