#pragma once
#include "CellClient.h"
#include "../../common/StemCell.h"
#include "../actors/PlayerClient.h"

class StemCellClient :
 virtual public StemCell,
 virtual public CellClient
{
public:
 StemCellClient(unsigned int id, Vec3f position, float angle, PlayerClient * owner) :
  Cell(position, CONFIG_FLOAT("data.cell.stemcell.radius"), angle, CONFIG_FLOAT("data.cell.stemcell.healthpoints"), owner),
  CellClient(),
  StemCell(position, angle)
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