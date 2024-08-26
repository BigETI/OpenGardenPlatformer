#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "../../Exportables/Exportable.hxx"
#include "../InputEvent.hpp"

namespace OGP::InputSystem::Raylib {
	class RaylibGamepadInputEvents {
	public:

		static constexpr const std::size_t MaximalGamepadButtonCount = 18U;

		OGP_API RaylibGamepadInputEvents();
		OGP_API RaylibGamepadInputEvents(std::size_t gamepadIndex);

		OGP_API std::size_t GetGamepadIndex() const noexcept;
		OGP_API bool WasGamepadConnected() const noexcept;
		OGP_API OGP::InputSystem::InputEvent SetGamepadConnectedState(bool isConnected);
		OGP_API bool WasButtonDown(std::size_t buttonIndex) const;
		OGP_API OGP::InputSystem::InputEvent SetButtonDownState(std::size_t buttonIndex, bool isButtonDown);
		OGP_API std::size_t GetUsedAxisCount() const noexcept;
		OGP_API float GetOldAxisValue(std::size_t axisIndex) const noexcept;
		OGP_API OGP::InputSystem::InputEvent SetAxisValue(std::size_t axisIndex, float axisValue);

	private:

		std::size_t gamepadIndex;
		OGP::InputSystem::InputEvent isConnectedInputEvent;
		std::array<InputEvent, MaximalGamepadButtonCount> buttonInputEvents;
		std::vector<InputEvent> axisInputEvents;
	};
}
