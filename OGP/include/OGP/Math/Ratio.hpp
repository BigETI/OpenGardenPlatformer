#pragma once

#include <utility>

namespace OGP::Math {
	template <typename TComponent>
	struct Ratio {
		static_assert(
			std::is_same<char, TComponent>::value ||
			std::is_same<unsigned char, TComponent>::value ||
			std::is_same<short, TComponent>::value ||
			std::is_same<unsigned short, TComponent>::value ||
			std::is_same<int, TComponent>::value ||
			std::is_same<unsigned int, TComponent>::value ||
			std::is_same<long, TComponent>::value ||
			std::is_same<unsigned long, TComponent>::value ||
			std::is_same<long long, TComponent>::value ||
			std::is_same<unsigned long long, TComponent>::value ||
			std::is_same<float, TComponent>::value ||
			std::is_same<double, TComponent>::value ||
			std::is_same<long double, TComponent>::value,
			"The specified ratio component type must be of a numeric type."
		);

		TComponent numerator;
		TComponent denominator;

		Ratio() : numerator(static_cast<TComponent>(0)), denominator(static_cast<TComponent>(0)) {
			// ...
		}
		
		Ratio(TComponent value) : numerator(value), denominator(static_cast<TComponent>(1)) {
			// ...
		}
		
		Ratio(TComponent numerator, TComponent denominator) : numerator(numerator), denominator(denominator) {
			// ...
		}

		Ratio(const Ratio<TComponent>& ratio) : numerator(ratio.numerator), denominator(ratio.denominator) {
			// ...
		}
		
		Ratio(Ratio<TComponent>&& ratio) : numerator(std::move(ratio.numerator)), denominator(std::move(ratio.denominator)) {
			// ...
		}

		template <typename TResult>
		inline TResult GetRatio() {
			static_assert(
				std::is_same<float, TResult>::value ||
				std::is_same<double, TResult>::value ||
				std::is_same<long double, TResult>::value,
				"The specified result type must be of a numeric type."
			);
			return (denominator > 0U) ? (static_cast<TResult>(numerator) / static_cast<TResult>(denominator)) : static_cast<TResult>(0);
		}

		Ratio<TComponent>& operator =(TComponent value) {
			numerator = value;
			denominator = static_cast<TComponent>(1);
			return *this;
		}
		
		Ratio<TComponent>& operator =(const Ratio<TComponent>& ratio) {
			numerator = ratio.numerator;
			denominator = ratio.denominator;
			return *this;
		}

		Ratio<TComponent>& operator =(Ratio<TComponent>&& ratio) {
			numerator = std::move(ratio.numerator);
			denominator = std::move(ratio.denominator);
			return *this;
		}
	};
}
