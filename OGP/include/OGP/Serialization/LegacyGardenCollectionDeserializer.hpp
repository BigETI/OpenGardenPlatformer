#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <istream>
#include <string>
#include <type_traits>

#include <Klein/Math/Bounds.hpp>
#include <Klein/Math/Vector2.hpp>

#include "../Environment/GardenCollectionData.hpp"
#include "../Exportables/Exportable.hxx"
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
		constexpr static bool TryReadingBounds(std::istream& inputStream, const Klein::Math::Vector2<TOutputComponent>& entityPosition, const Klein::Math::Vector2<TOutputComponent>& gardenSize, Klein::Math::Bounds<TOutputComponent>& result) noexcept {
			TInputComponent left;
			TInputComponent top;
			TInputComponent right;
			TInputComponent bottom;
			bool ret(
				TryReadingValue<TInputComponent>(inputStream, left) &&
				TryReadingValue<TInputComponent>(inputStream, top) &&
				TryReadingValue<TInputComponent>(inputStream, right) &&
				TryReadingValue<TInputComponent>(inputStream, bottom)
			);
			if (ret) {
				result = Klein::Math::Bounds<TOutputComponent>(
					std::clamp((top < static_cast<TOutputComponent>(0)) ? (static_cast<TOutputComponent>(gardenSize.y) - static_cast<TOutputComponent>(1)) : (static_cast<TOutputComponent>(entityPosition.y) + static_cast<TOutputComponent>(top)), static_cast<TOutputComponent>(0), static_cast<TOutputComponent>(gardenSize.y) - static_cast<TOutputComponent>(1)),
					std::clamp((bottom < static_cast<TOutputComponent>(0)) ? static_cast<TOutputComponent>(0) : (static_cast<TOutputComponent>(entityPosition.y) - static_cast<TOutputComponent>(bottom)), static_cast<TOutputComponent>(0), static_cast<TOutputComponent>(gardenSize.y) - static_cast<TOutputComponent>(1)),
					std::clamp((left < static_cast<TOutputComponent>(0)) ? static_cast<TOutputComponent>(0) : (static_cast<TOutputComponent>(entityPosition.x) - static_cast<TOutputComponent>(left)), static_cast<TOutputComponent>(0), static_cast<TOutputComponent>(gardenSize.x) - static_cast<TOutputComponent>(1)),
					std::clamp((right < static_cast<TOutputComponent>(0)) ? (static_cast<TOutputComponent>(gardenSize.x) - static_cast<TOutputComponent>(1)) : (static_cast<TOutputComponent>(entityPosition.x) + static_cast<TOutputComponent>(right)), static_cast<TOutputComponent>(0), static_cast<TOutputComponent>(gardenSize.x) - static_cast<TOutputComponent>(1))
				);
			}
			return ret;
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
