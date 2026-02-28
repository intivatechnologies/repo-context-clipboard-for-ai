#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

#include "file_messenger/directory.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc > 1) {
		stack<Directory> s;
		Directory d = Directory::scanDirectoryForNames("", argv[1]);

		s.push(d);

		while (!s.empty()) {
			Directory vTop = s.top();
			s.pop();

			cout << "files:" << endl;
			for (const auto& f : vTop.getFileNames())
				cout << f << endl;
			cout << "folders:" << endl;
			for (const auto& f : vTop.getFolderNames())
				cout << f << endl;
			cout << "mainRoot: " << vTop.getRoot() << endl;
		}

		return 0;
	}

	cout << "An input directory could not be found." << endl;
	return -1;
}