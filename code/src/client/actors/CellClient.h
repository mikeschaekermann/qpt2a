#pragma once

#include "../../common/Cell.h"
#include "../../client/actors/GameObjectClient.h"
#include "../../common/ConfigurationDataHandler.h"

class CellClient :
	virtual public Cell,
	virtual public GameObjectClient
{
public:
	CellClient()
	{
		setOpacity(CONFIG_FLOAT("data.ingamefeedback.building.completeOpacity"));
	}

	virtual void update(float frameTime);

	virtual void setPosition(Vec3f position) { GameObject::setPosition(position); }
	virtual void drawHealthBar(float healthBarOpacity=1.0);
	virtual void setRadius(float radius) { GameObject::setRadius(radius); }

	virtual void drawAtTransformation() const;
	
protected:
	float getHealthPercentage() const;
	float getBrightnessFromHealthPoints() const;
	float getOpacityFromHealthPoints() const;

	virtual void setRotation(Vec3f rotation) { GameObject::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { GameObject::setScale(scale); }
};