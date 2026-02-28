#pragma once

#include <filesystem>
#include <vector>
#include <string>

using namespace std;

namespace fs = std::filesystem;

class Directory {
	static string getAppendage(string p) { return p + "\\"; };

	const vector<string> fileNames, folderNames;
	const fs::path root;

public:
	static const Directory scanDirectoryForNames(const fs::path& rootSubdirectoryPath, const fs::path& mainRoot);

	const vector<string>& getFileNames() const { return fileNames; };
	const vector<string>& getFolderNames() const { return folderNames; };
	const fs::path& getRoot() const { return root; };

	Directory(vector<string> fileNames, vector<string> folderNames, fs::path root)
		: fileNames(move(fileNames)), folderNames(move(folderNames)), root(move(root)) {}
};
