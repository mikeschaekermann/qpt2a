#pragma once
#include "CellClient.h"
#include "../../common/StandardCell.h"
#include "../actors/PlayerClient.h"
#include <list>

class StandardCellClient :
 virtual public StandardCell,
 virtual public CellClient
{
public:
 StandardCellClient(unsigned int id, Vec3f position, float angle, PlayerClient * owner) :
  GameObject(),
  Cell(position, CONFIG_FLOAT1("data.cell.standardcell.radius"), angle, CONFIG_FLOAT1("data.cell.standardcell.healthpoints"), owner),
  GameObjectClient(),
  CellClient(),
  StandardCell(position, angle),
  spikesScale(1.0)
 {
	setId(id);
	
	auto distanceDropOffDegree = CONFIG_FLOAT2("data.cell.standardcell.distanceDropOffDegree", 70);
	auto attackRadius = CONFIG_FLOAT2("data.cell.standardcell.attackRadius", 20);
	auto numOfSpikesOnEachSide = CONFIG_INT2("data.cell.standardcell.spikes.numberOnEachSide", 10);
	int directions[2] = {-1, 1};

	spikesAngleRadiusList.push_back(make_pair(0, radius + attackRadius));

	for (unsigned int i = 0; i < 2; ++i)
	{
		for (unsigned s = 1; s <= numOfSpikesOnEachSide; ++s)
		{
			auto spikeAngle = (float(s) / float(numOfSpikesOnEachSide)) * distanceDropOffDegree;
			auto spikeRadius = radius + attackRadius * (1 - spikeAngle / distanceDropOffDegree);
			spikeAngle *= directions[i];
			
			spikesAngleRadiusList.push_back(make_pair(spikeAngle, spikeRadius));
		}
	}
 }

 void startAttackAnimation();

 virtual void setPosition(Vec3f position) { CellClient::setPosition(position); }
 virtual void update(float frameTime);

private:
 virtual void setId(unsigned int id) { CellClient::setId(id); }
 virtual void setRotation(Vec3f rotation) { CellClient::setRotation(rotation); }
 virtual void setScale(Vec3f scale) { CellClient::setScale(scale); }
 virtual void setRadius(float radius) { CellClient::setRadius(radius); }
 virtual void drawAtTransformation() const;
 virtual bool spikesCanBeSeen() const;

 list<pair<float, float> > spikesAngleRadiusList;
 float spikesScale;
};