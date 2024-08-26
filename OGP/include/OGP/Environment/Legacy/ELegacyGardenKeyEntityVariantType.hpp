#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenKeyEntityVariantType : std::uint16_t {
		RedKey = 0x0000,
		YellowKey = 0x0001,
		GreenKey = 0x0002
	};
}
