#pragma once

#include <cstdint>

namespace OGP::Environment::Legacy {
	enum class ELegacyGardenEntityType : std::uint8_t {
		Player = 0x01,
		Marmot = 0x02,
		Worm = 0x03,
		Lift = 0x04,
		Key = 0x05,
		Fruit = 0x06,
		Vegetable = 0x07,
		Flower = 0x08,
		QuestionMark = 0x09
	};
}
