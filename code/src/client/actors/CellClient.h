#pragma once

#include "../../common/Cell.h"
#include "../../client/actors/GameObjectClient.h"

class CellClient :
	virtual public Cell,
	virtual public GameObjectClient
{
};