#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenFruitEntityVariantType : std::uint16_t {
		Apple = 0x0000,
		Lemon = 0x0005,
		Cherry = 0x0006,
		Pineapple = 0x0007
	};
}
