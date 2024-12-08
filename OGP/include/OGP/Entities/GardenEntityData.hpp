#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include <Klein/Math/Vector2.hpp>
#include <Klein/Math/Bounds.hpp>

#include "EGardenEntityType.hpp"

namespace OGP::Entities {
	struct GardenEntityData {
		EGardenEntityType type;
		Klein::Math::Vector2<std::size_t> position;
		Klein::Math::Bounds<std::int64_t> bounds;
		std::string text;
	};
}
