#pragma once

namespace OGP::Entities {
	enum class ELiftMovementState {
		Up,
		Down,
		Left,
		Right,
		ParkingFromMovingUp,
		ParkingFromMovingDown,
		ParkingFromMovingLeft,
		ParkingFromMovingRight
	};
}
