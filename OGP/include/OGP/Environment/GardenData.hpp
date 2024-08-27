#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "../Cells/EGardenCellType.hpp"
#include "../Collections/ResizableGrid.hpp"
#include "../Entities/GardenEntityData.hpp"
#include "../Math/Vector2.hpp"

namespace OGP::Environment {
	struct GardenData {
		std::string name;
		std::string midiPath;
		std::size_t timeInGameSeconds;
		OGP::Collections::ResizableGrid<OGP::Cells::EGardenCellType> cells;
		std::vector<OGP::Entities::GardenEntityData> entities;
	};
}
