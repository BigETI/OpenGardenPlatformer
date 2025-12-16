#include <cassert>
#include <chrono>

#include <OGP/Environment/EGlobalWorldSpeed.hpp>
#include <OGP/Environment/GlobalWorld.hpp>

using namespace std;
using namespace std::chrono;

using namespace OGP::Environment;

constexpr static inline float GetTickCountPerSecond(EGlobalWorldSpeed globalWorldSpeed) {
	switch (globalWorldSpeed) {
	case EGlobalWorldSpeed::Slowest:
		return 2.0f;
	case EGlobalWorldSpeed::VerySlow:
		return 4.0f;
	case EGlobalWorldSpeed::Slower:
		return 8.0f;
	case EGlobalWorldSpeed::Slow:
		return 16.0f;
	case EGlobalWorldSpeed::Normal:
		return 32.0f;
	case EGlobalWorldSpeed::Fast:
		return 64.0f;
	case EGlobalWorldSpeed::Faster:
		return 128.0f;
	case EGlobalWorldSpeed::VeryFast:
		return 256.0f;
	case EGlobalWorldSpeed::Fastest:
		return 512.0f;
	}
	assert(false);
	return 0.0f;
}

EGlobalWorldSpeed GlobalWorld::globalWorldSpeed(EGlobalWorldSpeed::Normal);
float GlobalWorld::tickCountPerSecond(::GetTickCountPerSecond(globalWorldSpeed));

EGlobalWorldSpeed GlobalWorld::GetGlobalWorldSpeed() noexcept {
	return globalWorldSpeed;
}

void GlobalWorld::SetGlobalWorldSpeed(EGlobalWorldSpeed globalWorldSpeed) noexcept {
	GlobalWorld::globalWorldSpeed = globalWorldSpeed;
	tickCountPerSecond = ::GetTickCountPerSecond(globalWorldSpeed);
}

float GlobalWorld::GetTickCountPerSecond() noexcept {
	return tickCountPerSecond;
}

float GlobalWorld::GetSeconds(float tickCount) noexcept {
	return tickCount / tickCountPerSecond;
}

high_resolution_clock::duration GlobalWorld::GetDuration(float tickCount) noexcept {
	return duration_cast<high_resolution_clock::duration>(duration<float>(GetSeconds(tickCount)));
}

float GlobalWorld::GetSpeed(float tickCountPerUnit) noexcept {
	return tickCountPerSecond / tickCountPerUnit;
}
