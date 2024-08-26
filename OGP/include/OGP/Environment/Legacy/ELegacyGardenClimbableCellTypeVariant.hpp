#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenClimbableCellTypeVariant : std::uint16_t {
		Ladder = 0x0000,
		LadderShowingUpWhenFinished = 0x0001,
		Rope = 0x0003,
		LadderVanishingWhenFinished = 0x0004
	};
}
