#include <OGP/Entities/HumanoidInput.hpp>

using namespace OGP::Entities;

HumanoidInput::HumanoidInput() :
	isWalkingLeft(false),
	isWalkingRight(false),
	isWalkingUp(false),
	isWalkingDown(false),
	isDiggingLeft(false),
	isDiggingRight(false) {
	// ...
}

HumanoidInput::HumanoidInput(
	bool isWalkingLeft,
	bool isWalkingRight,
	bool isWalkingUp,
	bool isWalkingDown,
	bool isDiggingLeft,
	bool isDiggingRight
) :
	isWalkingLeft(isWalkingLeft),
	isWalkingRight(isWalkingRight),
	isWalkingUp(isWalkingUp),
	isWalkingDown(isWalkingDown),
	isDiggingLeft(isDiggingLeft),
	isDiggingRight(isDiggingRight) {
	// ...
};

HumanoidInput HumanoidInput::GetMergedWith(const HumanoidInput& humanoidInput) const noexcept {
	return HumanoidInput(
		isWalkingLeft || humanoidInput.isWalkingLeft,
		isWalkingRight || humanoidInput.isWalkingRight,
		isWalkingUp || humanoidInput.isWalkingUp,
		isWalkingDown || humanoidInput.isWalkingDown,
		isDiggingLeft || humanoidInput.isDiggingLeft,
		isDiggingRight || humanoidInput.isDiggingRight
	);
}
