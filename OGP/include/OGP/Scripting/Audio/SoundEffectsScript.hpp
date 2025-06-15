#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Audio/AudioPlayerScript.hpp>
#include <Klein/Scripting/Script.hpp>

namespace OGP::Scripting::Audio {
	class SoundEffectsScript : public Klein::Scripting::Script {
	public:

		SoundEffectsScript(Klein::SceneManagement::Node* node);
		virtual ~SoundEffectsScript() noexcept override;

		static SoundEffectsScript* GetGlobalSoundEffects() noexcept;

		float GetVolume() const noexcept;
		void SetVolume(float volume) noexcept;
		void PreloadSoundEffectWhenAvailable(const std::string& soundEffectID);
		bool PlaySoundEffect(const std::string& soundEffectID) noexcept;
		bool PlaySoundEffect(const std::string& soundEffectID, float volume) noexcept;

	private:

		static SoundEffectsScript* globalSoundEffects;

		float volume;
		std::unordered_map<std::string, std::vector<std::shared_ptr<Klein::Scripting::Audio::AudioPlayerScript>>> audioPlayers;
	};
}
