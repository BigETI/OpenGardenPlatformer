#pragma once

#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Script.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>

namespace OGPGame::Scripting {
	class PlayGardenCollectionScript : public Klein::Scripting::Script {
	public:

		PlayGardenCollectionScript(Klein::SceneManagement::Node* node);

		void PlayGardenCollection(const Klein::ResourceManagement::ResourceID& gardenCollectionResourceID) const;
	};
}
