#pragma once

#include "../../common/Polypeptide.h"
#include "../../client/actors/GameObjectClient.h"
#include "../../common/ConfigurationDataHandler.h"

class CellClient;

class PolypeptideClient :
	virtual public Polypeptide,
	virtual public GameObjectClient
{
public:
	PolypeptideClient(Vec3f focusCenter, float focusRadius, float cellRadius) :
		speed(CONFIG_FLOAT("data.polypeptide.speed")),
		polyRotationSpeed(CONFIG_FLOAT("data.polypeptide.rotationSpeed")),
		followPointRotationSpeed(CONFIG_FLOAT("data.polypeptide.pointRotationSpeed")),
		maxAmplitudeEruption(CONFIG_INT("data.polypeptide.maxPointAmplitudeEruption")),
		forward(Vec3f(1, 0, 0)),
		focusCenter(focusCenter),
		focusRadius(focusRadius)
	{
		this->cellRadius = cellRadius;

		scale = Vec3f(
			CONFIG_FLOAT("data.polypeptide.scale"),
			CONFIG_FLOAT("data.polypeptide.scale"),
			CONFIG_FLOAT("data.polypeptide.scale")
		);

		float followPointAngle = toRadians(float(rand() % 361));
		followPoint = Vec3f(focusCenter.x + (focusRadius * cos(followPointAngle)), focusCenter.y + (focusRadius * sin(followPointAngle)), focusCenter.z);
	}

	PolypeptideClient();

	CellClient * getOwner();

	virtual void update(float frameTime);

	virtual void setPosition(Vec3f position) { GameObjectClient::setPosition(position); }
	virtual void setRadius(float radius) { GameObjectClient::setRadius(radius); }
	
	virtual void setRotation(Vec3f rotation) { GameObjectClient::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { GameObjectClient::setScale(scale); }

	virtual void setCenterOfFocus(Vec3f center);
	virtual void setRadiusOfFocus(float radius);
	virtual void setFocus(Vec3f center, float radius);

	virtual void setOwner(Cell* owner);

protected:
	float cellRadius;
	float speed;
	float polyRotationSpeed;
	float followPointRotationSpeed;
	int maxAmplitudeEruption;
	Vec3f forward;

	virtual void drawAtTransformation() const;
	void arrivalBehavior(float frameTime);
	void updateFollowPoint(float frameTime);

private:
	Vec3f focusCenter;
	float focusRadius;
	Vec3f followPoint;
};