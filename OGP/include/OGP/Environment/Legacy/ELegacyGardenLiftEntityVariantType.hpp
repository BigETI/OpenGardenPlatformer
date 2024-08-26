#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenLiftEntityVariantType : std::uint16_t {
		UpMovingLift = 0x0000,
		DownMovingLift = 0x0001,
		LeftMovingLift = 0x0002,
		RightMovingLift = 0x0003
	};
}
