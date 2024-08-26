#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenFlowerEntityVariantType : std::uint16_t {
		Sunflower = 0x0000,
		Tulip = 0x0004,
		YellowDaisy = 0x0005,
		Rose = 0x0006
	};
}
