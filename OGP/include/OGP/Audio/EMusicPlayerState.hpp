#pragma once

namespace OGP::Audio {
	enum class EMusicPlayerState {
		Idle,
		BeginPlaying,
		Playing,
		ChangingMusic,
		EndPlaying
	};
}
