#pragma once

#include "../common/Config.h"

#include <string>
#include <vector>
#include <map>

#include "StemCell.h"

using namespace std;

class Player
{
public:
	Player(unsigned int id, string name) :
		id(id),
		name(name)
	{
	}

	unsigned int getId() const { return id; }
	const string& getName() const { return name; }
	void setId(unsigned int newId) { id = newId; }

private:
	unsigned int id;
	string name;
};