#pragma once
#include "CellClient.h"
#include "../../common/BoneCell.h"
#include "../actors/PlayerClient.h"
#include <list>

class BoneCellClient :
 virtual public BoneCell,
 virtual public CellClient
{
public:
 BoneCellClient(unsigned int id, Vec3f position, float angle, PlayerClient * owner) :
  GameObject(),
  Cell(position, CONFIG_FLOAT1("data.cell.bonecell.radius"), angle, CONFIG_FLOAT1("data.cell.bonecell.healthpoints"), owner),
  GameObjectClient(),
  CellClient(),
  BoneCell(position, angle)
 {
	setId(id);
 }

 virtual void update(float frameTime) { CellClient::update(frameTime); }
 virtual void setPosition(Vec3f position) { CellClient::setPosition(position); }

private:
 virtual void setId(unsigned int id) { CellClient::setId(id); }
 virtual void setRotation(Vec3f rotation) { CellClient::setRotation(rotation); }
 virtual void setScale(Vec3f scale) { CellClient::setScale(scale); }
 virtual void setRadius(float radius) { CellClient::setRadius(radius); }
 virtual void drawAtTransformation() const;
};