#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "../Collections/ResizableGrid.hpp"
#include "../Math/Vector2.hpp"
#include "EGardenCellType.hpp"
#include "GardenEntityData.hpp"

namespace OGP::Environment {
	struct GardenData {
		std::string name;
		std::string midiPath;
		std::size_t timeInGameSeconds;
		OGP::Collections::ResizableGrid<EGardenCellType> cells;
		std::vector<GardenEntityData> entities;
	};
}
