#include <Klein/SceneManagement/Node.hpp>

#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/SolidCellScript.hpp>

using namespace Klein::SceneManagement;

using namespace OGP::Scripting::Cells;

SolidCellScript::SolidCellScript(Node* node) : CellScript(node) {
	// ...
}

bool SolidCellScript::IsSolid() const noexcept {
	return true;
}
