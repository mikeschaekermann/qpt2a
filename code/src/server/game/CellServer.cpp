#include "CellServer.h"

CellServer::CellServer(Type type, Vec3f position, float angle, Player * owner) :
	Cell(position, angle, owner),
	type(type)
{
	switch(type)
	{
		case STEMCELL:
			this->radius = CONFIG_FLOAT("data.cell.stemcell.radius");
			this->healthPoints = CONFIG_FLOAT("data.cell.stemcell.healthpoints");
			break;
		case STANDARDCELL:
			this->radius = CONFIG_FLOAT("data.cell.standardcell.radius");
			this->healthPoints = CONFIG_FLOAT("data.cell.standardcell.healthpoints");
			break;
		case BONECELL:
			this->radius = CONFIG_FLOAT("data.cell.bonecell.radius");
			this->healthPoints = CONFIG_FLOAT("data.cell.bonecell.healthpoints");
			break;
	}
}

void CellServer::getNextCellPositionByAngle(float angle, float nextCellRadius, Vec3f & outPosition) const
{
	outPosition = position;
	outPosition.x += cosf(angle) * (0.1f + nextCellRadius + this->radius);
	outPosition.y += sinf(angle) * (0.1f + nextCellRadius + this->radius);
}

CellServer::Type CellServer::getType() const { return type; }

vector<StaticModificatorServer *> CellServer::getStaticModificator() const { return staticModificators; }

void CellServer::addStaticModificator(StaticModificatorServer * staticModificator)
{
	staticModificators.push_back(staticModificator);
}

void CellServer::setId(unsigned int id) { GameObjectServer::setId(id); }
void CellServer::setPosition(Vec3f position) { Cell::setPosition(position); }
void CellServer::setRotation(Vec3f rotation) { Cell::setRotation(rotation); }
void CellServer::setScale(Vec3f scale) { Cell::setScale(scale); }
void CellServer::setRadius(float radius) { Cell::setRadius(radius); }
