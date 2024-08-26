#pragma once

#include <istream>

#include "../Environment/GardenCollectionData.hpp"

namespace OGP::Serialization {
	class IGardenCollectionDeserializer {
	public:

		virtual ~IGardenCollectionDeserializer() noexcept {
			// ...
		}

		virtual bool TryDeserializingStream(std::istream& inputStream, OGP::Environment::GardenCollectionData& result) const = 0;
	};
}
