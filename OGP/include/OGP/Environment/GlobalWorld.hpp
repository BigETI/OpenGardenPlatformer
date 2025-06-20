#pragma once

#include <chrono>

#include "../Exportables/Exportable.hxx"
#include "EGlobalWorldSpeed.hpp"

namespace OGP::Environment {

	class GlobalWorld {
	public:

		GlobalWorld() = delete;
		GlobalWorld(const GlobalWorld&) = delete;
		GlobalWorld(GlobalWorld&&) noexcept = delete;
		~GlobalWorld() noexcept = delete;

		OGP_API static EGlobalWorldSpeed GetGlobalWorldSpeed() noexcept;
		OGP_API static void SetGlobalWorldSpeed(EGlobalWorldSpeed globalWorldSpeed) noexcept;
		OGP_API static float GetTickCountPerSecond() noexcept;
		OGP_API static float GetSeconds(float tickCount) noexcept;
		OGP_API static std::chrono::high_resolution_clock::duration GetDuration(float tickCount) noexcept;
		OGP_API static float GetSpeed(float tickCountPerUnit) noexcept;

		GlobalWorld& operator =(const GlobalWorld&) = delete;
		GlobalWorld& operator =(GlobalWorld&&) noexcept = delete;

	private:

		static EGlobalWorldSpeed globalWorldSpeed;
		static float tickCountPerSecond;
	};
}
