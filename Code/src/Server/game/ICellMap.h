#pragma once

#include <map>

#include "Cell.h"

using namespace std;

class ICellMap
{
public:
	virtual bool addCell(Cell* cell) = 0;
	virtual unsigned int size() const = 0;
};