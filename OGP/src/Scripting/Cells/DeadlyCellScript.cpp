#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/DeadlyCellScript.hpp>

using namespace Klein::SceneManagement;

using namespace OGP::Scripting::Cells;

DeadlyCellScript::DeadlyCellScript(Node* node) : CellScript(node) {
	// ...
}

bool DeadlyCellScript::IsDeadly() const noexcept {
	return true;
}
