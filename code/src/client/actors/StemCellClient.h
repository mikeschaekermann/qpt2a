#pragma once
#include "CellClient.h"
#include "../../common/StemCell.h"

class StemCellClient :
	virtual public StemCell,
	virtual public CellClient
{
public:
	StemCellClient(unsigned int id, Vec3f position, float angle) :
		GameObject(),
		Cell(position, CONFIG_FLOAT1("data.cell.stemcell.radius"), angle, CONFIG_FLOAT1("data.cell.stemcell.healthpoints")),
		GameObjectClient(),
		CellClient(),
		StemCell(position, angle)
	{
		setId(id);
	}

private:
	virtual void setId(unsigned int id) { CellClient::setId(id); }
	virtual void setPosition(Vec3f position) { CellClient::setPosition(position); }
	virtual void setRotation(Vec3f rotation) { CellClient::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { CellClient::setScale(scale); }
	virtual void setRadius(float radius) { CellClient::setRadius(radius); }
	virtual void drawAtTransformation() const;
};