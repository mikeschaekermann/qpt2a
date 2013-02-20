#pragma once

#include "../../common/Polypeptide.h"
#include "../../client/actors/GameObjectClient.h"
#include "../../common/ConfigurationDataHandler.h"

class PolypeptideClient :
	virtual public Polypeptide,
	virtual public GameObjectClient
{
public:
	PolypeptideClient(Vec3f focusCenter, float focusRadius) :
		speed(CONFIG_FLOAT("data.polypeptide.speed")),
		forward(Vec3f(1, 0, 0)),
		focusCenter(focusCenter),
		focusRadius(focusRadius),
		isInCircularMovement(false)
	{
		scale = Vec3f(
			CONFIG_FLOAT("data.polypeptide.scale"),
			CONFIG_FLOAT("data.polypeptide.scale"),
			CONFIG_FLOAT("data.polypeptide.scale")
		);
	}

	PolypeptideClient();

	CellClient * getOwner() { return dynamic_cast<CellClient *>(owner); }

	virtual void update(float frameTime);

	virtual void setPosition(Vec3f position) { GameObjectClient::setPosition(position); }
	virtual void setRadius(float radius) { GameObjectClient::setRadius(radius); }
	
	virtual void setRotation(Vec3f rotation) { GameObjectClient::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { GameObjectClient::setScale(scale); }

	virtual void setCenterOfFocus(Vec3f center);
	virtual void setRadiusOfFocus(float radius);
	virtual void setFocus(Vec3f center, float radius);

protected:
	float speed;
	bool isInCircularMovement;
	Vec3f forward;

	virtual void drawAtTransformation() const;
	void circularMovement(float frameTime);

private:
	Vec3f focusCenter;
	float focusRadius;
};