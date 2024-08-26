#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenDoorCellTypeVariant : std::uint16_t {
		RedDoor                         = 0x0000,
		YellowDoor						= 0x0001,
		GreenDoor						= 0x0002,
		AutomaticallyClosingRedDoor	    = 0x0006,
		AutomaticallyClosingYellowDoor  = 0x0007,
		AutomaticallyClosingGreenDoor	= 0x0008
	};
}
