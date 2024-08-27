#pragma once

#include <array>
#include <cstddef>
#include <istream>
#include <string>
#include <type_traits>

#include "../Environment/GardenCollectionData.hpp"
#include "../Exportables/Exportable.hxx"
#include "../Math/Bounds.hpp"
#include "IGardenCollectionDeserializer.hpp"

namespace OGP::Serialization {
	class LegacyGardenCollectionDeserializer : public IGardenCollectionDeserializer {
	public:

		OGP_API LegacyGardenCollectionDeserializer();

		LegacyGardenCollectionDeserializer(const LegacyGardenCollectionDeserializer&) = delete;
		LegacyGardenCollectionDeserializer(LegacyGardenCollectionDeserializer&&) noexcept = delete;

		OGP_API virtual bool TryDeserializingStream(
			std::istream& inputStream,
			OGP::Environment::GardenCollectionData& result
		) const override;

		LegacyGardenCollectionDeserializer& operator =(const LegacyGardenCollectionDeserializer&) = delete;
		LegacyGardenCollectionDeserializer& operator =(LegacyGardenCollectionDeserializer&&) noexcept = delete;

	private:

		inline static bool IsSystemBigEndian() noexcept {
			constexpr const std::uint16_t endianess_information(0xFF00);
			return *reinterpret_cast<const std::uint8_t*>(&endianess_information);
		}

		template <typename TValue>
		constexpr static TValue& FixByteOrder(TValue& value) noexcept {
			if ((sizeof(TValue) > static_cast<std::size_t>(1U)) && IsSystemBigEndian()) {
				TValue temp(value);
				std::uint8_t* bytes(reinterpret_cast<std::uint8_t*>(&value));
				std::uint8_t* temp_bytes(reinterpret_cast<std::uint8_t*>(&temp));
				for (std::size_t index(static_cast<std::size_t>(0U)); index != sizeof(TValue); index++) {
					bytes[index] = temp_bytes[sizeof(TValue) - static_cast<std::size_t>(1U) - index];
				}
			}
			return value;
		}

		template <typename TInputValue, typename TOutputValue = TInputValue>
		constexpr static bool TryReadingValue(std::istream& inputStream, TOutputValue& result) noexcept {
			bool ret;
			if (std::is_same<TInputValue, TOutputValue>::value) {
				ret = inputStream.read(reinterpret_cast<char*>(&result), sizeof(TOutputValue)).good();
				if (ret) {
					FixByteOrder(result);
				}
			}
			else {
				TInputValue temp;
				ret = inputStream.read(reinterpret_cast<char*>(&temp), sizeof(TInputValue)).good();
				if (ret) {
					FixByteOrder(temp);
					result = static_cast<TOutputValue>(temp);
				}
			}
			return ret;
		}
		
		template <typename TInputValue, typename TOutputValue = TInputValue>
		constexpr inline static bool TryReadingValueAndFixingYPosition(std::istream& inputStream, std::size_t gardenHeight, TOutputValue& result) noexcept {
			bool ret(TryReadingValue<TInputValue, TOutputValue>(inputStream, result));
			if (ret) {
				result = gardenHeight - result - static_cast<TOutputValue>(1);
			}
			return ret;
		}

		template <typename TInputComponent, typename TOutputComponent = TInputComponent>
		constexpr static bool TryReadingBounds(std::istream& inputStream, OGP::Math::Bounds<TOutputComponent>& result) noexcept {
			return
				TryReadingValue<TInputComponent, TOutputComponent>(inputStream, result.left) &&
				TryReadingValue<TInputComponent, TOutputComponent>(inputStream, result.top) &&
				TryReadingValue<TInputComponent, TOutputComponent>(inputStream, result.right) &&
				TryReadingValue<TInputComponent, TOutputComponent>(inputStream, result.bottom);
		}

		template <typename TArrayElement, size_t ArraySize>
		constexpr inline static bool TryReadingArray(std::istream& inputStream, std::array<TArrayElement, ArraySize>& result) noexcept {
			return inputStream.read(reinterpret_cast<char*>(result.data()), result.size() * sizeof(TArrayElement)).good();
		}

		static bool TryReadingRawString(std::istream& inputStream, std::size_t stringLength, std::string& result) noexcept;
		static bool TryReadingDGFString(std::istream& inputStream, std::string& result) noexcept;
		static bool TryReadingAndDecryptingDGFString(std::istream& inputStream, std::string& result) noexcept;

	};
}
