#pragma once

#include <vector>

#include "InputEvent.hpp"

namespace OGP::InputSystem {
	class IInputHandler {
	public:

		virtual ~IInputHandler() noexcept {
			// ...
		}

		virtual std::vector<InputEvent>& FetchInputEvents(std::vector<InputEvent>& result) = 0;
	};
}
