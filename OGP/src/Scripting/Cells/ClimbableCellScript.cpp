#include <OGP/Environment/EGardenCellType.hpp>
#include <OGP/SceneManagement/Node.hpp>
#include <OGP/Scripting/Cells/CellScript.hpp>
#include <OGP/Scripting/Cells/ClimbableCellScript.hpp>

using namespace OGP::Environment;
using namespace OGP::SceneManagement;
using namespace OGP::Scripting::Cells;
using namespace OGP::Scripting::Environment;

ClimbableCellScript::ClimbableCellScript(Node* node) : CellScript(node) {
	// ...
}

bool ClimbableCellScript::IsClimbable() const noexcept {
	return true;
}

bool ClimbableCellScript::IsClimbingUpAllowed() const noexcept {
	return IsClimbable() && (GetGardenCellType() != EGardenCellType::Rope);
}
