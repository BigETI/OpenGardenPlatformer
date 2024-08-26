#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/SolidCellScript.hpp>

using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Cells;

SolidCellScript::SolidCellScript(Node* node) : CellScript(node) {
	// ...
}

bool SolidCellScript::IsSolid() const noexcept {
	return true;
}
