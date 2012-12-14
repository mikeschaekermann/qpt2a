#pragma once
#include "CellClient.h"
#include "../../common/StemCell.h"

class StemCellClient :
	virtual public StemCell,
	virtual public CellClient
{};