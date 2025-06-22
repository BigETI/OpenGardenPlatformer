#include <Klein/SceneManagement/Node.hpp>

#include <OGP/SceneManagement/BaseSceneLoader.hpp>
#include <OGP/Scripting/Audio/MusicPlayerScript.hpp>
#include <OGP/Scripting/Audio/SoundEffectsScript.hpp>

using namespace Klein::SceneManagement;

using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Audio;

void BaseSceneLoader::Load(Node& rootNode) {
	rootNode.CreateNewChild("MusicPlayer")->AddScript<MusicPlayerScript>();
	rootNode.CreateNewChild("SoundEffects")->AddScript<SoundEffectsScript>()->SetVolume(0.25f);
}
