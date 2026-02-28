#pragma once
#ifndef FILE_LOCATOR_HPP
#define FILE_LOCATOR_HPP

#include <string>

using namespace std;

class FileLocator {
	string name, root = "";

public:
	const string& getName() const { return name; }
	const string& getRoot() const { return root; }

	void setName(const string& name) { this->name = name; }
	void setRoot(const string& root) { this->root = root; }

	FileLocator(string fpath) {
		name = fpath.substr(fpath.find_last_of('\\') + 1, fpath.length() - 1);
	}
};

#endif