#include <filesystem>

#include <OGP/ResourceManagement/FileSystem.hpp>
#include <OGP/ResourceManagement/ResourceID.hpp>

using namespace OGP::ResourceManagement;
using namespace std;
using namespace std::filesystem;

path& FileSystem::GetFilePathFromResourceID(const ResourceID& resourceID, path& result) {
	return result = std::filesystem::current_path() / "Resources" / resourceID.GetString();
}
