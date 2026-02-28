#include "directory.hpp"

const Directory Directory::scanDirectoryForNames(const fs::path& rootSubdirectoryPath, const fs::path& root) {
	string appendage = getAppendage(root.string());
	vector<string> fileNames, folderNames;

	for (const auto& entry : fs::directory_iterator(root.string())) {
		string eps = entry.path().string();
		string name = eps.substr(appendage.size(), eps.size() - 1);

		if (entry.is_regular_file())
			fileNames.push_back(name);
		else
			folderNames.push_back(name);
	}

	return Directory(fileNames, folderNames, root);
}