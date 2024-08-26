#pragma once

#include <array>
#include <cstddef>
#include <unordered_map>
#include <vector>

#include "../../Exportables/Exportable.hxx"
#include "../../Math/Vector2.hpp"
#include "../IInputHandler.hpp"
#include "../InputEvent.hpp"
#include "RaylibGamepadInputEvents.hpp"

namespace OGP::InputSystem::Raylib {
	class RaylibInputHandler : public OGP::InputSystem::IInputHandler {
	public:

		static constexpr const std::size_t MaximalMouseButtonCount = 7U;
		static constexpr const std::size_t MaximalGamepadCount = 16U;

		OGP_API RaylibInputHandler();

		RaylibInputHandler(const RaylibInputHandler&) = delete;
		RaylibInputHandler(RaylibInputHandler&&) = delete;

		OGP_API std::vector<OGP::InputSystem::InputEvent>& FetchInputEvents(std::vector<InputEvent>& result) override;
		
		RaylibInputHandler& operator =(const RaylibInputHandler&) = delete;
		RaylibInputHandler& operator =(RaylibInputHandler&&) = delete;

	private:

		std::unordered_map<int, OGP::InputSystem::InputEvent> keyboardInputEvents;
		std::array<OGP::InputSystem::InputEvent, MaximalMouseButtonCount> mouseButtonInputEvents;
		OGP::InputSystem::InputEvent mouseDeltaInputEvent;
		OGP::InputSystem::InputEvent mousePositionInputEvent;
		OGP::InputSystem::InputEvent mouseWheelMovementInputEvent;
		std::array<RaylibGamepadInputEvents, MaximalGamepadCount> gamepadInputEvents;

		OGP::InputSystem::InputEvent& GetCachedKeyboardInputEvent(int keyCode);
	};
}
