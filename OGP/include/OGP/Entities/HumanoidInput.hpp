#pragma once

#include "../Exportables/Exportable.hxx"

namespace OGP::Entities {
	struct HumanoidInput {
		bool isWalkingLeft;
		bool isWalkingRight;
		bool isWalkingUp;
		bool isWalkingDown;
		bool isDiggingLeft;
		bool isDiggingRight;

		OGP_API HumanoidInput();
	};
}
