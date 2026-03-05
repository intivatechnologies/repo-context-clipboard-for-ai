#pragma once
#ifndef FOLDER_REP_HPP
#define FOLDER_REP_HPP

#include <string>
#include <vector>

using namespace std;

namespace file_records {
	class FileRecord {
		const string name;

	public:
		FileRecord(const string name) : name(name) {}
		virtual ~FileRecord() = default; // Make destructor virtual for polymorphism

		const string getName() const {
			return name;
		}
	};

	class FileRep : public FileRecord {
	protected:
		const string extension;

	public:
		FileRep(const string name, const string extension) : FileRecord(name), extension(extension) {}

		const string getExtension() const {
			return extension;
		}
	};

	class FolderRep : public FileRecord {
		const vector<FileRecord*> children;

	public:
		static FolderRep* installFolderAtRoot(string root);

		FolderRep(const string name, const vector<FileRecord*> children)
			: FileRecord(name), children(children) {}

		const vector<FileRecord*>& getChildren() const {
			return children;
		}
	};
}

#endif