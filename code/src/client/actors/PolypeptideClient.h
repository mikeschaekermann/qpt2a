#pragma once

#include "../../common/Polypeptide.h"
#include "../../client/actors/GameObjectClient.h"
#include "../../common/ConfigurationDataHandler.h"

class PolypeptideClient :
	virtual public Polypeptide,
	virtual public GameObjectClient
{
public:
	PolypeptideClient() { }

	virtual void drawAtTransformation();

	virtual void setPosition(Vec3f position) { GameObject::setPosition(position); }
	virtual void setRadius(float radius) { GameObject::setRadius(radius); }
	
protected:
	virtual void setRotation(Vec3f rotation) { GameObject::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { GameObject::setScale(scale); }
};