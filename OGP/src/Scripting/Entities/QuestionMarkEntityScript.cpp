#include <OGP/Math/Vector2.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Entities/EntityScript.hpp>
#include <OGP/Scripting/Entities/QuestionMarkEntityScript.hpp>

using namespace OGP::Math;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Entities;

QuestionMarkEntityScript::QuestionMarkEntityScript(Node* node) : EntityScript(node) {
	// ...
}

bool QuestionMarkEntityScript::Interact(const Vector2<int>& relativeSourcePosition) {

	// TODO: Implement question mark interaction
	
	return false;
}
