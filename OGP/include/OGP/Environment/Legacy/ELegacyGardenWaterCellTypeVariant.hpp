#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenWaterCellTypeVariant : std::uint16_t {
		Full = 0x0000,
		Surface = 0x0001
	};
}
