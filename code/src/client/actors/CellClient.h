#pragma once

#include "../../common/Cell.h"
#include "../../client/actors/GameObjectClient.h"
#include "../../common/ConfigurationDataHandler.h"
#include "../../common/network/messages/enum/CellType.h"

class PolypeptideClient;

class CellClient :
	virtual public Cell,
	virtual public GameObjectClient
{
public:
	CellClient():
		type(CellType::Invalid)
	{
		setOpacity(CONFIG_FLOAT("data.ingamefeedback.building.completeOpacity"));
	}

	virtual void update(float frameTime);

	virtual void setPosition(Vec3f position) { GameObject::setPosition(position); }
	virtual void drawHealthBar(float healthBarOpacity=1.0);
	virtual void setRadius(float radius) { GameObject::setRadius(radius); }
	
	virtual void draw() const;

	virtual void increaseGlobalTypeCounter() const {};
	virtual void decreaseGlobalTypeCounter() const {};

	virtual bool addPolypeptide(Polypeptide * polypeptide);

	virtual CellType::Type getType() const { return type; }

protected:
	float getHealthPercentage() const;
	float getBrightnessFromHealthPoints() const;
	float getOpacityFromHealthPoints() const;

	virtual void setRotation(Vec3f rotation) { GameObject::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { GameObject::setScale(scale); }

	CellType::Type type;
};