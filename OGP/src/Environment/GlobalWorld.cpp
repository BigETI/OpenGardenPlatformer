#include <chrono>

#include <OGP/Environment/EGlobalWorldSpeed.hpp>
#include <OGP/Environment/GlobalWorld.hpp>

using namespace std::chrono;

using namespace OGP::Environment;

constexpr static inline float GetTickCountPerSecond(EGlobalWorldSpeed globalWorldSpeed) {
	float ret;
	switch (globalWorldSpeed) {
	case EGlobalWorldSpeed::Slowest:
		ret = 2.0f;
		break;
	case EGlobalWorldSpeed::VerySlow:
		ret = 4.0f;
		break;
	case EGlobalWorldSpeed::Slower:
		ret = 8.0f;
		break;
	case EGlobalWorldSpeed::Slow:
		ret = 16.0f;
		break;
	case EGlobalWorldSpeed::Normal:
		ret = 32.0f;
		break;
	case EGlobalWorldSpeed::Fast:
		ret = 64.0f;
		break;
	case EGlobalWorldSpeed::Faster:
		ret = 128.0f;
		break;
	case EGlobalWorldSpeed::VeryFast:
		ret = 256.0f;
		break;
	case EGlobalWorldSpeed::Fastest:
		ret = 512.0f;
		break;
	}
	return ret;
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
