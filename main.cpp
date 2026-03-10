#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>

#include "file_records/filesystem_node.hpp"
#include "traversals/traversals_content_roots.hpp"
#include "traversals/traversals_string_tree.hpp"
#include "misc/get_content_from_files.hpp"
#include "misc/get_root_name.hpp"

using namespace std;
using namespace file_records;

static vector<string> dontIncludeFolderNests = { ".git", "build" };
static map<string, vector<string>*> flags;

void buildTree(FilesystemNode& rootNode) {
	const vector<string> EXCLUDE_DIRS = flags.count("exclude-dir") > 0
		? *flags.at("exclude-dir") : vector<string>();

	for (filesystem::directory_entry entry : filesystem::directory_iterator(rootNode.getPath())) {
		FilesystemNode entryNode(&entry);
	
		if (entry.is_directory()) {
			bool isADisincludedFolderNest = false;
	
			for (const auto& disincludeFolderNest : EXCLUDE_DIRS)
				if (entryNode.getName() == disincludeFolderNest) {
					isADisincludedFolderNest = true;
					break;
				}
	
			if(!isADisincludedFolderNest)
				buildTree(entryNode);
		}
	
		rootNode.addChild(entryNode);
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		//populate static map flags 'flags' with all flags and tags
		vector<string>* currentTags = nullptr;
		string currentFlag = "";
		const int FLAGS_INDEX = 1;
		for (int i = FLAGS_INDEX; i < argc; i++) {
			string token = argv[i];
			if(token == "\\")
				continue;

			//process for --flags
			if (token.rfind("--", 0) == 0)
			{
				if (currentTags != nullptr) {
					flags.insert(pair(currentFlag, currentTags));
					currentTags = nullptr;
				}

				currentFlag = token.substr(2); // remove "--"
				currentTags = new vector<string>();
			}
			else
				currentTags->push_back(token);
		}
		flags.insert(pair(currentFlag, currentTags));

		//ensure file extensions have a dot char at the beginning
		vector<string>& INCLUDE_EXTENSIONS = flags.count("include-ext") > 0
			? *flags.at("include-ext") : vector<string>();
		if (INCLUDE_EXTENSIONS.size() > 0)
			for (auto& tag : INCLUDE_EXTENSIONS) {
				if (tag[0] != '.')
					tag.insert(tag.begin(), '.');
			}
		
		/*
		//print all flags and tags
		for (const auto& pair : flags) {
			cout << "--" << pair.first;
			for (const auto& tag : *(flags.at(pair.first)))
				cout << ' ' << tag;
			cout << endl;
		}
		*/

		if (flags.count("root") > 0 && flags.at("root")->size() == 1) {
			//then we have a root path to work with
			filesystem::path rootPath(flags.at("root")->at(0));
			filesystem::directory_entry rootEntry(rootPath);

			//build the tree
			FilesystemNode rootNode(&rootEntry);
			buildTree(rootNode);

			//list the tree
			string tree = rootNode.getName() + '\n';
			traverseStringTree(tree, rootNode);
			cout << "> PROJECT STRUCTURE:" << endl << tree << endl;

			if (argc > 2) {
				//then we have extensions dedicated to content extraction
				vector<string> contentExtensions;
				for (int i = 2; i < argc; i++)
					contentExtensions.push_back(argv[i]);

				cout << "> CONTENT EXTENSIONS:" << endl;
				for(string contentExtension : INCLUDE_EXTENSIONS)
					cout << '-' << contentExtension << endl;
				cout << endl;

				//list all roots with extensions that are dedicated to content extraction
				vector<string> contentRoots;
				traverseContentRootsByExtension(contentRoots, rootNode, contentExtensions);

				cout << "> CONTENT ROOTS:" << endl;
				for(string contentRoot : contentRoots)
					cout << ">> " << contentRoot << endl;
				vector<string> contents = getContentFromFiles(contentRoots);

				/*
				cout << "> CONTENTS:" << endl;
				for (int i = 0; i < contentRoots.size(); i++) {
					cout << ">> " << contentRoots[i] << endl;
					cout << contents[i] << endl << endl;
				}
				*/
			}
		}
	}

	return 0;
}