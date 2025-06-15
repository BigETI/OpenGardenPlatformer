#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <queue>

#include <Klein/Audio/IAudioClip.hpp>
#include <Klein/Engine.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Audio/MusicIDTransitionTime.hpp"
#include "../../Audio/EMusicPlayerState.hpp"
#include "../../Exportables/Exportable.hxx"

namespace OGP::Scripting::Audio {
	class MusicPlayerScript : public Klein::Scripting::Script {
	public:

		MusicPlayerScript(Klein::SceneManagement::Node* node);
		virtual ~MusicPlayerScript() noexcept override;

		OGP_API static MusicPlayerScript* GetGlobalMusicPlayer() noexcept;

		OGP_API bool PreloadMusic(const std::string& musicID) noexcept;
		OGP_API bool EnqueueMusicToPlay(const std::string& musicID, const std::chrono::high_resolution_clock::duration& transitionTime = std::chrono::high_resolution_clock::duration::zero()) noexcept;
		OGP_API bool Stop(const std::chrono::high_resolution_clock::duration& fadeOutTime = std::chrono::high_resolution_clock::duration::zero());

	protected:

		OGP_API virtual void OnEnable(Klein::Engine& engine) override;
		OGP_API virtual void OnDisable(Klein::Engine& engine) override;
		OGP_API virtual void OnGameTick(Klein::Engine& engine, const std::chrono::high_resolution_clock::duration& deltaTime) override;

	private:

		static MusicPlayerScript* globalMusicPlayer;

		Klein::Engine* engine;
		OGP::Audio::EMusicPlayerState musicPlayerState;
		std::unordered_map<std::string, std::shared_ptr<Klein::Audio::IAudioClip>> preloadedAudioClips;
		std::weak_ptr<Klein::Audio::IAudioClip> playingAudioClip;
		std::weak_ptr<Klein::Audio::IAudioClip> nextAudioClip;
		std::queue<OGP::Audio::MusicIDTransitionTime> toBePlayedMusicIDs;
		std::chrono::high_resolution_clock::duration currentTransitionTime;
		std::chrono::high_resolution_clock::duration elapsedTransitionTime;

		void ResetMusicPlayerState() noexcept;
	};
}
