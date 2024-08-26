#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenAirCellTypeVariant : std::uint16_t {
		Nothing = 0x0000,
		Fence = 0x0001,
		Bush = 0x0002,
		LeftCloud = 0x0003,
		RightCloud = 0x0004,
		Sun = 0x0005,
		LeftArrow = 0x0006,
		RightArrow = 0x0007,
		UpArrow = 0x0008,
		DownArrow = 0x0009
	};
}
