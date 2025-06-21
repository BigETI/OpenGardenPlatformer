#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Scripting/Cells/DeadlyCellScript.hpp>
#include <OGP/Scripting/Cells/SolidCellScript.hpp>

using namespace Klein::SceneManagement;

using namespace OGP::Scripting::Cells;

DeadlyCellScript::DeadlyCellScript(Node* node) : SolidCellScript(node) {
	// ...
}

bool DeadlyCellScript::IsDeadly() const noexcept {
	return true;
}
