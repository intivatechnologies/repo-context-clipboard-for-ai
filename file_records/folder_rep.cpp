#include "folder_rep.hpp"
#include "../misc/get_root_name.hpp"

#include <filesystem>

using namespace file_records;
using namespace std::filesystem;

FolderRep* FolderRep::installFolderAtRoot(string root) {
	vector<FileRecord*> rootChildren;

	for (const auto& entry : directory_iterator(root)) {
		if (entry.is_regular_file())
			rootChildren.push_back(new FileRep(entry.path().filename().string(),
				entry.path().extension().string()));
		else
			rootChildren.push_back(installFolderAtRoot(entry.path().string()));
	}

	return new FolderRep(getRootName(root), rootChildren);
}