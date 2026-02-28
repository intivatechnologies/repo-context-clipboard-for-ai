#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <stack>

#include "file_messenger/file_locator.hpp"

using namespace std;

namespace fs = filesystem; //Has to be using C++ 17 ISO or greater

vector<FileLocator> scanDirectory(const std::filesystem::path& root) {
	vector<FileLocator> fileLocators;
	for (const auto& entry : fs::directory_iterator(root)) {
		string print;
		if (entry.is_regular_file()) {
			FileLocator f(entry.path().string());
			fileLocators.push_back(f);
			print = f.getName();
		}
		else
			print = entry.path().string();
	}
	return fileLocators;
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		stack<vector<FileLocator>> s;
		vector<FileLocator> v = scanDirectory(argv[1]);
		s.push(v);

		while (!s.empty()) {
			vector<FileLocator> vTop = s.top();
			s.pop();

			for (const auto& f : vTop) {
				cout << f.getName() << endl;
			}
		}

		return 0;
	}

	cout << "An input directory could not be found." << endl;
	return -1;
}