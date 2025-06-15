#include <chrono>
#include <memory>
#include <string>

#include <Klein/Audio/IAudioClip.hpp>
#include <Klein/Engine.hpp>
#include <Klein/Math/Easing.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Audio/MusicIDTransitionTime.hpp>
#include <OGP/Audio/EMusicPlayerState.hpp>
#include <OGP/Scripting/Audio/MusicPlayerScript.hpp>

using namespace std;
using namespace std::chrono;

using namespace Klein;
using namespace Klein::Audio;
using namespace Klein::Math;
using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;

using namespace OGP::Audio;
using namespace OGP::Scripting::Audio;

MusicPlayerScript* MusicPlayerScript::globalMusicPlayer(nullptr);

MusicPlayerScript::MusicPlayerScript(Node* node) :
	Script(node),
	engine(nullptr),
	musicPlayerState(EMusicPlayerState::Idle),
	currentTransitionTime(high_resolution_clock::duration::zero()),
	elapsedTransitionTime(high_resolution_clock::duration::zero()) {
	if (!globalMusicPlayer) {
		globalMusicPlayer = this;
	}
}

MusicPlayerScript::~MusicPlayerScript() noexcept {
	for (const auto& preloaded_audio_clip : preloadedAudioClips) {
		preloaded_audio_clip.second->Stop();
	}
	preloadedAudioClips.clear();
	if (globalMusicPlayer == this) {
		globalMusicPlayer = nullptr;
	}
}

MusicPlayerScript* MusicPlayerScript::GetGlobalMusicPlayer() noexcept {
	return globalMusicPlayer;
}

bool MusicPlayerScript::PreloadMusic(const string& musicID) noexcept {
	if ((preloadedAudioClips.find(musicID) != preloadedAudioClips.end())) {
		return true;
	}
	if (!engine) {
		return false;
	}
	shared_ptr<IAudioDevice> default_audio_device(engine->GetDefaultAudioDevice());
	if (!default_audio_device) {
		return false;
	}
	ResourceID resourceID("Music/" + musicID);
	shared_ptr<IAudioClip> audio_clip(default_audio_device->LoadAudioClip(resourceID, true));
	if (!audio_clip) {
		return false;
	}
	preloadedAudioClips.insert_or_assign(musicID, audio_clip);
	return true;
}

bool MusicPlayerScript::EnqueueMusicToPlay(const string& musicID, const high_resolution_clock::duration& transitionTime) noexcept {
	bool ret(PreloadMusic(musicID));
	if (ret) {
		toBePlayedMusicIDs.push(MusicIDTransitionTime{ .musicID = musicID, .transitionTime = transitionTime });
	}
	return ret;
}

bool MusicPlayerScript::Stop(const high_resolution_clock::duration& fadeOutTime) {
	bool ret(false);
	if (shared_ptr<IAudioClip> playing_audio_clip = playingAudioClip.lock()) {
		if (fadeOutTime > high_resolution_clock::duration::zero()) {
			this->currentTransitionTime = fadeOutTime;
			this->elapsedTransitionTime = high_resolution_clock::duration::zero();
			this->musicPlayerState = EMusicPlayerState::EndPlaying;
		}
		else {
			playing_audio_clip->Stop();
			ResetMusicPlayerState();
		}
		while (!toBePlayedMusicIDs.empty()) {
			toBePlayedMusicIDs.pop();
		}
		ret = true;
	}
	return ret;
}

void MusicPlayerScript::OnEnable(Engine& engine) {
	this->engine = &engine;
}

void MusicPlayerScript::OnDisable(Klein::Engine& engine) {
	this->engine = nullptr;
	for (const auto& preloaded_audio_clip : preloadedAudioClips) {
		preloaded_audio_clip.second->Stop();
	}
	preloadedAudioClips.clear();
}

void MusicPlayerScript::OnGameTick(Engine& engine, const high_resolution_clock::duration& deltaTime) {
	switch (musicPlayerState) {
	case EMusicPlayerState::Idle:
		if (!toBePlayedMusicIDs.empty()) {
			MusicIDTransitionTime music_id_transition_time(toBePlayedMusicIDs.front());
			toBePlayedMusicIDs.pop();
			if (PreloadMusic(music_id_transition_time.musicID)) {
				shared_ptr<IAudioClip> playing_audio_clip(preloadedAudioClips.find(music_id_transition_time.musicID)->second);
				playing_audio_clip->Play(0.0f);
				playingAudioClip = playing_audio_clip;
				currentTransitionTime = music_id_transition_time.transitionTime;
				elapsedTransitionTime = high_resolution_clock::duration::zero();
				musicPlayerState = EMusicPlayerState::BeginPlaying;
			}
		}
		break;
	case EMusicPlayerState::BeginPlaying:
		if (shared_ptr<IAudioClip> playing_audio_clip = playingAudioClip.lock()) {
			elapsedTransitionTime = min(elapsedTransitionTime + deltaTime, currentTransitionTime);
			float transition(duration<float>(elapsedTransitionTime).count() / duration<float>(currentTransitionTime).count());
			playing_audio_clip->SetVolume(Easing::EaseOut(transition));
			if (elapsedTransitionTime >= currentTransitionTime) {
				currentTransitionTime = high_resolution_clock::duration::zero();
				elapsedTransitionTime = high_resolution_clock::duration::zero();
				musicPlayerState = EMusicPlayerState::Playing;
			}
			playing_audio_clip->Update();
		}
		else {
			ResetMusicPlayerState();
		}
		break;
	case EMusicPlayerState::Playing:
		if (shared_ptr<IAudioClip> playing_audio_clip = playingAudioClip.lock()) {
			if (!toBePlayedMusicIDs.empty()) {
				MusicIDTransitionTime music_id_transition_time(toBePlayedMusicIDs.front());
				toBePlayedMusicIDs.pop();
				if (PreloadMusic(music_id_transition_time.musicID)) {
					shared_ptr<IAudioClip> next_audio_clip(preloadedAudioClips.find(music_id_transition_time.musicID)->second);
					if (next_audio_clip != playing_audio_clip) {
						next_audio_clip->Play(0.0f);
						nextAudioClip = next_audio_clip;
						currentTransitionTime = music_id_transition_time.transitionTime;
						elapsedTransitionTime = high_resolution_clock::duration::zero();
						musicPlayerState = EMusicPlayerState::ChangingMusic;
						next_audio_clip->Update();
					}
				}
			}
			playing_audio_clip->Update();
		}
		else {
			ResetMusicPlayerState();
		}
		break;
	case EMusicPlayerState::ChangingMusic:
		if (shared_ptr<IAudioClip> playing_audio_clip = playingAudioClip.lock()) {
			if (shared_ptr<IAudioClip> next_audio_clip = nextAudioClip.lock()) {
				elapsedTransitionTime = min(elapsedTransitionTime + deltaTime, currentTransitionTime);
				float transition(duration<float>(elapsedTransitionTime).count() / duration<float>(currentTransitionTime).count());
				playing_audio_clip->SetVolume(Easing::EaseIn(1.0f - transition));
				next_audio_clip->SetVolume(Easing::EaseOut(transition));
				if (elapsedTransitionTime >= currentTransitionTime) {
					currentTransitionTime = high_resolution_clock::duration::zero();
					elapsedTransitionTime = high_resolution_clock::duration::zero();
					musicPlayerState = EMusicPlayerState::Playing;
					playing_audio_clip->Stop();
					playingAudioClip = nextAudioClip;
					nextAudioClip.reset();
				}
				playing_audio_clip->Update();
				next_audio_clip->Update();
			}
			else {
				ResetMusicPlayerState();
			}
		}
		else {
			ResetMusicPlayerState();
		}
		break;
	case EMusicPlayerState::EndPlaying:
		if (shared_ptr<IAudioClip> playing_audio_clip = playingAudioClip.lock()) {
			elapsedTransitionTime = min(elapsedTransitionTime + deltaTime, currentTransitionTime);
			float volume(duration<float>(elapsedTransitionTime) / duration<float>(currentTransitionTime));
			playing_audio_clip->SetVolume(Easing::EaseIn(1.0f - volume));
			if (elapsedTransitionTime >= currentTransitionTime) {
				playing_audio_clip->Stop();
				currentTransitionTime = high_resolution_clock::duration::zero();
				elapsedTransitionTime = high_resolution_clock::duration::zero();
				musicPlayerState = EMusicPlayerState::Idle;
			}
			playing_audio_clip->Update();
		}
		else {
			ResetMusicPlayerState();
		}
		break;
	}
}

void MusicPlayerScript::ResetMusicPlayerState() noexcept {
	currentTransitionTime = high_resolution_clock::duration::zero();
	elapsedTransitionTime = high_resolution_clock::duration::zero();
	musicPlayerState = EMusicPlayerState::Idle;
}
