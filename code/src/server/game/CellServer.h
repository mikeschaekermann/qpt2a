#pragma once

#include <vector>

#include "../../common/Cell.h"
#include "GameObjectServer.h"

class CellServer :
	virtual public Cell,
	virtual public GameObjectServer
{
};