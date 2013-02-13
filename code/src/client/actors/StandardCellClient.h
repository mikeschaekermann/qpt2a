#pragma once
#include "CellClient.h"
#include "../../common/StandardCell.h"
#include "../actors/PlayerClient.h"

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
  StandardCell(position, angle)
 {
  setId(id);
 }

 void startAnimation();

 virtual void setPosition(Vec3f position) { CellClient::setPosition(position); }
 
private:
 virtual void setId(unsigned int id) { CellClient::setId(id); }
 virtual void setRotation(Vec3f rotation) { CellClient::setRotation(rotation); }
 virtual void setScale(Vec3f scale) { CellClient::setScale(scale); }
 virtual void setRadius(float radius) { CellClient::setRadius(radius); }
 virtual void drawAtTransformation() const;
};