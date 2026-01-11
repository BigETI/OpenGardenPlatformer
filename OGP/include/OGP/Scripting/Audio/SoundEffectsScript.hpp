#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/Audio/AudioPlayerScript.hpp>
#include <Klein/Scripting/Script.hpp>

#include "../../Exportables/Exportable.hxx"

namespace OGP::Scripting::Audio {
	class SoundEffectsScript : public Klein::Scripting::Script {
	public:

		OGP_API SoundEffectsScript(Klein::SceneManagement::Node* node);
		OGP_API virtual ~SoundEffectsScript() noexcept override;

		OGP_API static SoundEffectsScript* GetGlobalSoundEffects() noexcept;

		OGP_API float GetVolume() const noexcept;
		OGP_API void SetVolume(float volume) noexcept;
		OGP_API void PreloadSoundEffectWhenAvailable(const std::string& soundEffectID);
		OGP_API bool PlaySoundEffect(const std::string& soundEffectID) noexcept;
		OGP_API bool PlaySoundEffect(const std::string& soundEffectID, float volume) noexcept;

	private:

		static SoundEffectsScript* globalSoundEffects;

		float volume;
		std::unordered_map<std::string, std::vector<std::shared_ptr<Klein::Scripting::Audio::AudioPlayerScript>>> audioPlayers;
	};
}
