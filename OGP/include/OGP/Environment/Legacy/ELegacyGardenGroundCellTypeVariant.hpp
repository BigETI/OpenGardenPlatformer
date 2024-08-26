#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenGroundCellTypeVariant : std::uint16_t {
		Default = 0x0000,
		Trap = 0x0001,
		HardGround = 0x0002,
		GrassGround = 0x000D,
		GrassTrap = 0x000E,
		HardGrassGround = 0x000F,
		MoleHill = 0x001A
	};
}
