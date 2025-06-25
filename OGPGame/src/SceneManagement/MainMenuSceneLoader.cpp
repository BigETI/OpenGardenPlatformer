#include <chrono>
#include <memory>

#include <Klein/Math/Bounds.hpp>
#include <Klein/ResourceManagement/ResourceID.hpp>
#include <Klein/SceneManagement/Node.hpp>
#include <Klein/Scripting/UI/ButtonScript.hpp>
#include <Klein/Scripting/UI/CanvasScript.hpp>
#include <Klein/Scripting/UI/TextScript.hpp>

#include <OGP/Scripting/Audio/MusicPlayerScript.hpp>
#include <OGP/Scripting/Audio/SoundEffectsScript.hpp>

#include <OGPGame/Game.hpp>
#include <OGPGame/SceneManagement/MainMenuSceneLoader.hpp>
#include <OGPGame/Scripting/PlayGardenCollectionScript.hpp>

using namespace std;
using namespace std::literals::chrono_literals;

using namespace Klein::Math;
using namespace Klein::ResourceManagement;
using namespace Klein::SceneManagement;
using namespace Klein::Scripting::UI;

using namespace OGP::Scripting::Audio;

using namespace OGPGame;
using namespace OGPGame::SceneManagement;
using namespace OGPGame::Scripting;

static void CreateNewPlayGardenButton(const ResourceID& gardenCollectionResourceID, const ResourceID& imageResourceID, const std::string& label, Node& canvasNode, const Vector2<float>& position) {
	shared_ptr<Node> button_node(canvasNode.CreateNewChild());
	shared_ptr<ButtonScript> button(button_node->AddScript<ButtonScript>());
	shared_ptr<TextScript> text(button_node->CreateNewChild()->AddScript<TextScript>());
	weak_ptr<PlayGardenCollectionScript> play_garden_collection(button_node->AddScript<PlayGardenCollectionScript>());
	button->OnClicked += [play_garden_collection, gardenCollectionResourceID]() {
		if (shared_ptr<PlayGardenCollectionScript> current_play_garden_collection = play_garden_collection.lock()) {
			current_play_garden_collection->PlayGardenCollection(gardenCollectionResourceID);
			if (SoundEffectsScript* sound_effects = SoundEffectsScript::GetGlobalSoundEffects()) {
				sound_effects->PlaySoundEffect("NextLevel");
			}
		}
	};
	button->SetPosition(position);
	button->SetAnchor(Bounds<float>(0.5f, 0.5f, 0.5f, 0.5f));
	button->SetBorder(Border<float>(54.0f, 54.0f, 96.0f, 96.0f));
	button->SetImageResourceID(imageResourceID);
	text->SetAnchor(Bounds<float>(0.0f, 0.0f, 0.0f, 0.0f));
	text->SetText(label);
}

void MainMenuSceneLoader::Load(Node& rootNode) {
	shared_ptr<Node> canvas_node(rootNode.CreateNewChild());
	shared_ptr<CanvasScript> canvas(canvas_node->AddScript<CanvasScript>());
	canvas->SetRenderingContextIndex(static_cast<size_t>(1));
	CreateNewPlayGardenButton(ResourceID("GardenCollections/DAISYG.DGF"), ResourceID("Textures/Introduction/DaisyG.png"), "DaisyG", *canvas_node, Vector2<float>(0.0f, 108.0f + 32.0f));
	CreateNewPlayGardenButton(ResourceID("GardenCollections/DAISYG2.DGF"), ResourceID("Textures/Introduction/DaysiG2.png"), "DaisyG2", *canvas_node, Vector2<float>(0.0f, 0.0f));
	CreateNewPlayGardenButton(ResourceID("GardenCollections/DGARDEN.DGF"), ResourceID("Textures/Introduction/DGarden.png"), "DGarden", *canvas_node, Vector2<float>(0.0f, -108.0f - 32.0f));
	if (MusicPlayerScript* music_player = MusicPlayerScript::GetGlobalMusicPlayer()) {
		music_player->EnqueueMusicToPlay("DGARDEN2.mp3", 1s);
	}
}
