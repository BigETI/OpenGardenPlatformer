#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "../../Environment/GardenCollectionData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Math/Vector2.hpp"
//#include "../../SceneManagement/Node.hpp"
#include "../../Serialization/IGardenCollectionDeserializer.hpp"
#include "../Script.hpp"
#include "GardenScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenCollectionScript : public OGP::Scripting::Script {
	public:

		OGP_API GardenCollectionScript(OGP::SceneManagement::Node* node);

		OGP_API bool LoadGardenCollectionFile(const std::filesystem::path& gardenCollectionFilePath);
		OGP_API void UnloadGarden();
		OGP_API void ReloadGarden();
		OGP_API bool SelectGardenIndex(std::size_t gardenIndex);
		OGP_API bool SelectGardenIndex(std::size_t gardenIndex, bool isForceLoading);

	protected:

		OGP_API virtual void OnGameTick(OGP::Game& game, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		std::vector<std::shared_ptr<OGP::Serialization::IGardenCollectionDeserializer>> gardenCollectionDeserializers;
		OGP::Environment::GardenCollectionData gardenCollectionData;
		std::shared_ptr<GardenScript> garden;
		std::size_t selectedGardenIndex;
		bool isSelectingGarden;
		std::size_t toSelectGardenIndex;
	};
}
