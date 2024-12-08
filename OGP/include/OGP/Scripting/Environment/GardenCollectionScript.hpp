#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include <Klein/Engine.hpp>
#include <Klein/Math/Vector2.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Environment/GardenCollectionData.hpp"
#include "../../Exportables/Exportable.hxx"
#include "../../Serialization/IGardenCollectionDeserializer.hpp"
#include "GardenScript.hpp"

namespace OGP::Scripting::Environment {
	class GardenCollectionScript : public Klein::Scripting::Script {
	public:

		OGP_API GardenCollectionScript(Klein::SceneManagement::Node* node);

		OGP_API bool LoadGardenCollectionFile(const std::filesystem::path& gardenCollectionFilePath);
		OGP_API void UnloadGarden();
		OGP_API void ReloadGarden();
		OGP_API bool SelectGardenIndex(std::size_t gardenIndex);
		OGP_API bool SelectGardenIndex(std::size_t gardenIndex, bool isForceLoading);

	protected:

		OGP_API virtual void OnGameTick(Klein::Engine& engine, std::chrono::high_resolution_clock::duration deltaTime) override;

	private:

		std::vector<std::shared_ptr<OGP::Serialization::IGardenCollectionDeserializer>> gardenCollectionDeserializers;
		OGP::Environment::GardenCollectionData gardenCollectionData;
		std::shared_ptr<GardenScript> garden;
		std::size_t selectedGardenIndex;
		bool isSelectingGarden;
		std::size_t toSelectGardenIndex;
	};
}
