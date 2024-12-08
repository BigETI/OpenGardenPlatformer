#pragma once

#include <chrono>
#include <cstddef>
#include <memory>

#include <Klein/Engine.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Rendering/SpriteRendererScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Entities/GardenEntityData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Scripting/Environment/GardenScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenScript;
}

namespace OGP::Scripting::Entities {
	class EntityScript : public Klein::Scripting::Script {
	public:

		Klein::EventSystem::Event<const OGP::Entities::GardenEntityData&> OnSpawned;

		OGP_API EntityScript(Klein::SceneManagement::Node* node);

		OGP_API const OGP::Entities::GardenEntityData& GetGardenEntityData() const noexcept;
		OGP_API const Klein::Math::Vector2<std::size_t>& GetCurrentPosition() const noexcept;
		OGP_API virtual Klein::Math::Vector2<float> GetToBeRenderedPosition() const noexcept;
		OGP_API void SetCurrentPosition(const Klein::Math::Vector2<std::size_t>& currentPosition) noexcept;
		OGP_API std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript> GetSpriteRenderer() const noexcept;
		OGP_API std::weak_ptr<OGP::Scripting::Environment::GardenScript> GetGarden() const noexcept;
		OGP_API std::weak_ptr<EntityScript> GetMountedAtEntity() const noexcept;
		OGP_API bool MountAt(const std::shared_ptr<EntityScript>& mountedAtEntity) noexcept;
		OGP_API bool Dismount() noexcept;
		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden);
		OGP_API virtual bool Interact(EntityScript& sourceEntity);
		OGP_API virtual void OnBeforeFrameRender(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		OGP::Entities::GardenEntityData gardenEntityData;
		Klein::Math::Vector2<std::size_t> currentPosition;
		std::weak_ptr<Klein::Scripting::Rendering::SpriteRendererScript> spriteRenderer;
		std::weak_ptr<OGP::Scripting::Environment::GardenScript> garden;
		std::weak_ptr<EntityScript> mountedAtEntity;
	};
}
