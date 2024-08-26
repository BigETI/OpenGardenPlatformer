#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "../Math/Vector2.hpp"
#include "../Math/Bounds.hpp"
#include "EGardenEntityType.hpp"

namespace OGP::Environment {
	struct GardenEntityData {
		EGardenEntityType type;
		OGP::Math::Vector2<std::size_t> position;
		OGP::Math::Bounds<std::int64_t> bounds;
		std::string text;
	};
}
