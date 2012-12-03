#pragma once

#include <map>

#include "Cell.h"

using namespace std;

class ICellMap
{
public:
	virtual Cell* addCell(Cell* cell) = 0;
	virtual void removeCell(Cell* cell) = 0;
	virtual unsigned int size() const = 0;
};