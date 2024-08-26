#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenWormEntityVariantType : std::uint16_t {
		RightMovingWorm = 0x0000,
		LeftMovingWorm = 0x0010
	};
}
