#pragma once
#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>

using namespace std;

class Location {
	string name;

public:
	const string& getName() const { return name; }

	void setName(const string& name) { this->name = name; }

	Location(string fpath) {
		name = fpath.substr(fpath.find_last_of('\\') + 1, fpath.length() - 1);
	}
};

#endif