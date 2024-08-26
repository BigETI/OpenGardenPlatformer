#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/TopDeadlyCellScript.hpp>

using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Cells;

TopDeadlyCellScript::TopDeadlyCellScript(Node* node) : CellScript(node) {
	// ...
}

bool TopDeadlyCellScript::IsTopDeadly() const noexcept {
	return true;
}
