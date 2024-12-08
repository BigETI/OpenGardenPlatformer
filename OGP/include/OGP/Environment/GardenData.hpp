#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include <Klein/Collections/ResizableGrid.hpp>

#include "../Cells/EGardenCellType.hpp"
#include "../Entities/GardenEntityData.hpp"

namespace OGP::Environment {
	struct GardenData {
		std::string name;
		std::string midiPath;
		std::size_t timeInGameSeconds;
		Klein::Collections::ResizableGrid<OGP::Cells::EGardenCellType> cells;
		std::vector<OGP::Entities::GardenEntityData> entities;
	};
}
