#pragma once

#include "../../common/Polypeptide.h"
#include "../../client/actors/GameObjectClient.h"
#include "../../common/ConfigurationDataHandler.h"

class PolypeptideClient :
	virtual public Polypeptide,
	virtual public GameObjectClient
{
public:
	PolypeptideClient(Vec3f rotationPoint) :
		speed(CONFIG_FLOAT1("data.polypeptide.speed")),
		forward(Vec3f(1, 0, 0)),
		rotationPoint(rotationPoint),
		isOnIdleCircle(false),
		idleRadius(15)
	{
		scale = Vec3f(
			CONFIG_FLOAT1("data.polypeptide.scale"),
			CONFIG_FLOAT1("data.polypeptide.scale"),
			CONFIG_FLOAT1("data.polypeptide.scale")
		);
	}

	void setRotationPoint(Vec3f rotationPoint) { this->rotationPoint = rotationPoint; }

	virtual void update(float frameTime);

	virtual void setPosition(Vec3f position) { GameObjectClient::setPosition(position); }
	virtual void setRadius(float radius) { GameObjectClient::setRadius(radius); }
	
	virtual void setRotation(Vec3f rotation) { GameObjectClient::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { GameObjectClient::setScale(scale); }

protected:
	float idleRadius;
	float speed;
	bool isOnIdleCircle;
	Vec3f forward;
	Vec3f rotationPoint;

	virtual void drawAtTransformation() const;
	void moveIdleFormation(float frameTime);
};