#pragma once

#include <vector>

#include "../../common/Cell.h"
#include "../../common/ConfigurationDataHandler.h"

#include "GameObjectServer.h"

class StaticModificatorServer;

class CellServer :
	virtual public Cell,
	virtual public GameObjectServer
{
public:
	enum Type
	{
		STEMCELL,
		STANDARDCELL,
		BONECELL
	};

	CellServer(Type type, Vec3f position, float angle, Player * owner);

	void getNextCellPositionByAngle(float angle, float nextCellRadius, Vec3f & outPosition) const;

	Type getType() const;

	vector<StaticModificatorServer *> getStaticModificator() const;

	void addStaticModificator(StaticModificatorServer * staticModificator);

private:
	virtual void setId(unsigned int id);
	virtual void setPosition(Vec3f position);
	virtual void setRotation(Vec3f rotation);
	virtual void setScale(Vec3f scale);
	virtual void setRadius(float radius);

	vector<StaticModificatorServer *> staticModificators;

	Type type;
};