#pragma once

namespace OGP::Environment {

	enum class ESideFlags {

		None = 0x0,

		Top = 0x1,

		Bottom = 0x2,

		Left = 0x4,

		Right = 0x8,

		TopLeft = 0x10,

		TopRight = 0x20,

		BottomLeft = 0x40,

		BottomRight = 0x80,

		WithoutTop = Bottom | Left | Right | TopLeft | TopRight | BottomLeft | BottomRight,

		All = Top | Bottom | Left | Right | TopLeft | TopRight | BottomLeft | BottomRight
	};

	constexpr inline static ESideFlags operator &(ESideFlags left, ESideFlags right) {
		return static_cast<ESideFlags>(static_cast<int>(left) & static_cast<int>(right));
	}

	constexpr inline static ESideFlags operator |(ESideFlags left, ESideFlags right) {
		return static_cast<ESideFlags>(static_cast<int>(left) | static_cast<int>(right));
	}

	constexpr inline static ESideFlags& operator &=(ESideFlags& left, ESideFlags right) {
		return left = left & right;
	}

	constexpr inline static ESideFlags& operator |=(ESideFlags left, ESideFlags right) {
		return left = left | right;
	}
}
