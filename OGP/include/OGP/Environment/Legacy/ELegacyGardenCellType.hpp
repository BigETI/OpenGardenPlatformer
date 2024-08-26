#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenCellType : std::uint8_t {
		Air = 0x01,
		Ground = 0x02,
		Climbable = 0x03,
		Door = 0x04,
		Water = 0x05
	};
}
