#pragma once

#include <chrono>
#include <string>

namespace OGP::Audio {
	struct MusicIDTransitionTime {
		std::string musicID;
		std::chrono::high_resolution_clock::duration transitionTime;
	};
}
