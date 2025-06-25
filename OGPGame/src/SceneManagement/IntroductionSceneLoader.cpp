#include <Klein/SceneManagement/Node.hpp>

#include <OGPGame/SceneManagement/IntroductionSceneLoader.hpp>
#include <OGPGame/Scripting/IntroductionScript.hpp>

using namespace Klein::SceneManagement;

using namespace OGPGame::SceneManagement;
using namespace OGPGame::Scripting;

void IntroductionSceneLoader::Load(Node& rootNode) {
	rootNode.CreateNewChild("Introduction")->AddScript<IntroductionScript>();
}
