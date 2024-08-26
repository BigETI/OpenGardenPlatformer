#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenVegetableEntityVariantType : std::uint16_t {
		Garlic = 0x0000,
		Mushroom = 0x0001,
		Spinach = 0x0002,
		Carrot = 0x0003
	};
}
