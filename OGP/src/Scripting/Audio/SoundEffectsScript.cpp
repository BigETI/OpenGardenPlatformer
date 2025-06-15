#include <memory>
#include <string>
#include <vector>

#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Audio/AudioPlayerScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include <OGP/Scripting/Audio/SoundEffectsScript.hpp>

using namespace std;

using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting;
using namespace Klein::Scripting::Audio;

using namespace OGP::Scripting::Audio;

SoundEffectsScript* SoundEffectsScript::globalSoundEffects(nullptr);

SoundEffectsScript::SoundEffectsScript(Node* node) : Script(node), volume(1.0f) {
	if (!globalSoundEffects) {
		globalSoundEffects = this;
	}
}

SoundEffectsScript::~SoundEffectsScript() noexcept {
	if (globalSoundEffects == this) {
		globalSoundEffects = nullptr;
	}
}

SoundEffectsScript* SoundEffectsScript::GetGlobalSoundEffects() noexcept {
	return globalSoundEffects;
}

float SoundEffectsScript::GetVolume() const noexcept {
	return volume;
}

void SoundEffectsScript::SetVolume(float volume) noexcept {
	this->volume = volume;
}

void SoundEffectsScript::PreloadSoundEffectWhenAvailable(const string& soundEffectID) {
	if (audioPlayers.find(soundEffectID) == audioPlayers.end()) {
		shared_ptr<AudioPlayerScript> audio_player(GetNode().CreateNewChild(soundEffectID)->EnsureScript<AudioPlayerScript>());
		audio_player->SetResourceID(ResourceID("SoundEffects/" + soundEffectID + ".wav"), false, false);
		audio_player->PreloadWhenAvailable();
		vector<shared_ptr<AudioPlayerScript>> audio_players;
		audio_players.push_back(audio_player);
		audioPlayers.insert_or_assign(soundEffectID, audio_players);
	}
}

bool SoundEffectsScript::PlaySoundEffect(const string& soundEffectID) noexcept {
	return PlaySoundEffect(soundEffectID, volume);
}

bool SoundEffectsScript::PlaySoundEffect(const string& soundEffectID, float volume) noexcept {
	PreloadSoundEffectWhenAvailable(soundEffectID);
	const auto& it(audioPlayers.find(soundEffectID));
	if (it == audioPlayers.end()) {
		return false;
	}
	vector<shared_ptr<AudioPlayerScript>>& audio_players(audioPlayers.find(soundEffectID)->second);
	bool is_playing(false);
	for (shared_ptr<AudioPlayerScript> audio_player : audio_players) {
		if (audio_player) {
			if (audio_player->IsPlaying()) {
				continue;
			}
			audio_player->PlayWhenAvailable(volume);
			is_playing = true;
			break;
		}
	}
	if (!is_playing) {
		shared_ptr<AudioPlayerScript> audio_player(GetNode().CreateNewChild(soundEffectID)->EnsureScript<AudioPlayerScript>());
		audio_player->SetResourceID(ResourceID("SoundEffects/" + soundEffectID + ".wav"), false, false);
		audio_player->PlayWhenAvailable(volume);
		audio_players.push_back(audio_player);
	}
	return true;
}
