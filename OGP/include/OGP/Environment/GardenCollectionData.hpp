#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "GardenData.hpp"

namespace OGP::Environment {
	struct GardenCollectionData {
		std::string editPassword;
		std::string playPassword;
		std::size_t applyPlayPasswordUntilGardenIndex;
		std::string authorName;
		std::string comments;
		std::vector<GardenData> gardenCollection;
	};
}
