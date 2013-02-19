#pragma once

#include "../common/Config.h"

#include <string>
#include <vector>
#include <map>

#include "Cell.h"

using namespace std;

class Player
{
public:
	Player(unsigned int id, string name) :
		id(id),
		name(name),
		stemCell(nullptr)
	{
	}

	unsigned int getId() const { return id; }
	const string& getName() const { return name; }
	virtual Cell & getStemCell() { return *stemCell; }
	void setId(unsigned int newId) { id = newId; }
	virtual void setStemCell(Cell * stemCell) { this->stemCell = stemCell; }


protected:
	unsigned int id;
	string name;
	Cell * stemCell;
};