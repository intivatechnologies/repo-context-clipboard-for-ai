#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <filesystem>
#include <algorithm>

using namespace std;

enum PlaceOperation {
	TABULATE,
	DETABULATE,
	CONTINUE
};

/* A line placed */
class PlacedLine {
	const string line;
	const PlaceOperation placeOperation;

public:
	const string& getLine() const { return line; };
	const PlaceOperation& getPlaceOperation() const { return placeOperation; };
	explicit PlacedLine(string line, PlaceOperation placeOperation = PlaceOperation::CONTINUE)
		: line(line), placeOperation(placeOperation) {}
};

string getRootName(filesystem::path path) {
	return path.string().substr(path.string().find_last_of("\\") + 1, path.string().size() - 1);
}

static const vector<string> ignoreNames({ ".git", "build" });

vector<PlacedLine> getPlacedLines(string root) {
	PlaceOperation nextOperation = PlaceOperation::TABULATE;
	string rootName = getRootName(root);
	vector<PlacedLine> lines;

	//examines each name in the directory
	for (const auto& entry : filesystem::directory_iterator(root)) {
		//extract the file/folder name
		string eps = entry.path().string(), name = eps.substr(root.size() + 1, eps.size() - 1);

		if (entry.is_regular_file())
			lines.push_back(PlacedLine(name + ",", nextOperation));
		else {
			auto ignoreNameCheck = find(ignoreNames.begin(), ignoreNames.end(), name);

			if (ignoreNameCheck != ignoreNames.end())
				lines.push_back(PlacedLine(name + ": ... ,", nextOperation));
			else {
				lines.push_back(PlacedLine(name + ": {", nextOperation));

				vector<PlacedLine> nestedLines = getPlacedLines(root + "\\" + name);
				for (const auto& nestedEntry : nestedLines)
					lines.push_back(nestedEntry);

				lines.push_back(PlacedLine("}", nestedLines.size() > 0
					? PlaceOperation::DETABULATE : PlaceOperation::CONTINUE));
			}
		}

		if (nextOperation != PlaceOperation::CONTINUE)
			nextOperation = PlaceOperation::CONTINUE;
	}

	return lines;
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		vector<PlacedLine> lines({ PlacedLine(getRootName(argv[1]) + ": {")});
		vector<PlacedLine> nestedLines = getPlacedLines(argv[1]);

		for (const auto& nestedEntry : nestedLines)
			lines.push_back(nestedEntry);
		lines.push_back(PlacedLine("}", PlaceOperation::DETABULATE));

		string indentation = "";
		int tabsSoFar = 0;

		for (PlacedLine pl : lines) {
			switch (pl.getPlaceOperation()) {
			case PlaceOperation::TABULATE:
				indentation += "\t";
				++tabsSoFar;
				break;
			case PlaceOperation::DETABULATE:
				--tabsSoFar;
				indentation = "";
				for (int i = 0; i < tabsSoFar; i++)
					indentation += "\t";
				break;
			}

			cout << indentation << pl.getLine() << endl;
		}

		return 0;
	}

	cout << "An input directory could not be found." << endl;
	return -1;
}