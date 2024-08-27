#pragma once

#include <cstddef>
#include <memory>

#include "../../Entities/GardenEntityData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Math/Vector2.hpp"
#include "../../SceneManagement/Node.hpp"
#include "../../Scripting/Environment/GardenScript.hpp"
#include "../../Scripting/Rendering/SpriteRendererScript.hpp"
#include "../Script.hpp"

namespace OGP::Scripting::Environment {
	class GardenScript;
}

namespace OGP::Scripting::Entities {
	class EntityScript : public OGP::Scripting::Script {
	public:

		OGP::EventSystem::Event<const OGP::Entities::GardenEntityData&> OnSpawned;

		OGP_API EntityScript(OGP::SceneManagement::Node* node);

		OGP_API const OGP::Entities::GardenEntityData& GetGardenEntityData() const noexcept;
		OGP_API const OGP::Math::Vector2<std::size_t>& GetCurrentPosition() const noexcept;
		OGP_API void SetCurrentPosition(const OGP::Math::Vector2<std::size_t>& currentPosition) noexcept;
		OGP_API std::weak_ptr<OGP::Scripting::Rendering::SpriteRendererScript> GetSpriteRenderer() const noexcept;
		OGP_API std::weak_ptr<OGP::Scripting::Environment::GardenScript> GetGarden() const noexcept;
		OGP_API virtual void Spawn(const OGP::Entities::GardenEntityData& gardenEntityData, std::shared_ptr<OGP::Scripting::Environment::GardenScript> garden);
		OGP_API virtual bool Interact(const OGP::Math::Vector2<int>& relativeSourcePosition);

	private:

		OGP::Entities::GardenEntityData gardenEntityData;
		OGP::Math::Vector2<std::size_t> currentPosition;
		std::weak_ptr<OGP::Scripting::Rendering::SpriteRendererScript> spriteRenderer;
		std::weak_ptr<OGP::Scripting::Environment::GardenScript> garden;
	};
}
