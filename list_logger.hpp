#pragma once
#ifndef LIST_LOGGER
#define LIST_LOGGER

#include <vector>
#include <string>

using namespace std;

/* Enters in list string-only-data in the order it was added. */
class ListLogger {
	vector<string> names;

public:
	vector<string>& getNames() { return names; };
};

#endif