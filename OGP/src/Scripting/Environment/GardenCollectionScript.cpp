#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <OGP/Environment/EGardenCellType.hpp>
#include <OGP/Math/Vector2.hpp>
#include <OGP/Scripting/Environment/GardenCollectionScript.hpp>
#include <OGP/Serialization/LegacyGardenCollectionDeserializer.hpp>

using namespace OGP::Environment;
using namespace OGP::Math;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting;
using namespace OGP::Scripting::Environment;
using namespace OGP::Serialization;
using namespace std;
using namespace std::chrono;
using namespace std::filesystem;

GardenCollectionScript::GardenCollectionScript(Node* node) :
	Script(node),
	gardenCollectionDeserializers(
		{
			make_shared<LegacyGardenCollectionDeserializer>()
		}),
	selectedGardenIndex(static_cast<size_t>(0U)),
	isSelectingGarden(false),
	toSelectGardenIndex(static_cast<size_t>(0U)) {
	// ...
}

bool GardenCollectionScript::LoadGardenCollectionFile(const path& gardenCollectionFilePath) {
	bool ret(false);
	for (const auto& garden_collection_deserializer : gardenCollectionDeserializers) {
		ifstream garden_collection_file_stream(gardenCollectionFilePath, ios::binary);
		if (garden_collection_file_stream.is_open() && garden_collection_deserializer->TryDeserializingStream(garden_collection_file_stream, gardenCollectionData)) {
			ret = true;
			break;
		}
	}
	if (ret) {
		SelectGardenIndex(static_cast<size_t>(0U), true);
	}
	return ret;
}

void GardenCollectionScript::UnloadGarden() {
	garden.reset();
	GetNode().RemoveAllChildren();
}

void GardenCollectionScript::ReloadGarden() {
	SelectGardenIndex(selectedGardenIndex, true);
}

bool GardenCollectionScript::SelectGardenIndex(size_t gardenIndex) {
	return SelectGardenIndex(gardenIndex, false);
}

bool GardenCollectionScript::SelectGardenIndex(size_t gardenIndex, bool isForceLoading) {
	bool ret((gardenIndex < gardenCollectionData.gardenCollection.size()) && (isForceLoading || !garden || (selectedGardenIndex != gardenIndex)));
	if (ret) {
		toSelectGardenIndex = gardenIndex;
		isSelectingGarden = true;
	}
	return ret;
}

void GardenCollectionScript::OnGameTick(Game& game, high_resolution_clock::duration deltaTime) {
	if (isSelectingGarden) {
		isSelectingGarden = false;
		selectedGardenIndex = toSelectGardenIndex;
		UnloadGarden();
		string garden_name("Garden_");
		garden_name += to_string(selectedGardenIndex);
		garden = GetNode().CreateNewChild(std::move(garden_name))->AddScript<GardenScript>();
		garden->OnCompleted += [&]() {
			// TODO: React on garden completion

			// TODO: Test
			SelectGardenIndex(selectedGardenIndex + static_cast<size_t>(1));
		};
		garden->OnFailed += [&]() {
			ReloadGarden();
		};
		garden->LoadGardenFromGardenData(gardenCollectionData.gardenCollection.at(selectedGardenIndex));
	}
}
