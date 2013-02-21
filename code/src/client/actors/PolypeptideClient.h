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
	PolypeptideClient(Vec3f focusCenter, float focusRadius, float cellRadius);
	PolypeptideClient();
	~PolypeptideClient();

	CellClient * getOwner();

	virtual void update(float frameTime);
	virtual void draw() const;

	virtual void setPosition(Vec3f position) { GameObjectClient::setPosition(position); }
	virtual void setRadius(float radius) { GameObjectClient::setRadius(radius); }
	
	virtual void setRotation(Vec3f rotation) { GameObjectClient::setRotation(rotation); }
	virtual void setScale(Vec3f scale) { GameObjectClient::setScale(scale); }

	virtual void setCenterOfFocus(Vec3f center);
	virtual void setRadiusOfFocus(float radius);
	virtual void setFocus(Vec3f center, float radius);

	virtual void setOwner(Cell* owner);

	void setAttackOptions(bool selfDestruct, bool dieTrying = false);

protected:
	bool selfDestruct;
	bool dieTrying;
	bool wayBackFromFocus;
	
	float cellRadius;
	float speed;
	float polyRotationSpeed;
	float followPointRotationSpeed;
	int maxAmplitudeEruption;
	Vec3f forward;

	virtual void drawAtTransformation() const;
	void arrivalBehavior(float frameTime);
	void attackBehavior(float frameTime);
	void updateFollowPoint(float frameTime);

	float remainingDistancePercentagePerSecond;

private:
	Vec3f focusCenter;
	float focusRadius;
	Vec3f followPoint;
};