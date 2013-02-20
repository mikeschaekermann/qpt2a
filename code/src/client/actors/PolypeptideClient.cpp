#include "CellClient.h"
#include "PolypeptideClient.h"
#include "../rendering/RenderManager.h"
#include "CellClient.h"

void PolypeptideClient::drawAtTransformation() const
{
	RENDER_MGR->renderBlackShadedModel("poly");
	
	// follow points debug code
	/*popMatrices();
	pushMatrices();
	translate(followPoint);
	drawSphere(Vec3f::zero(), 3);*/
}

void PolypeptideClient::update(float frameTime)
{
	updateFollowPoint(frameTime);
	switch(state)
	{
		case Polypeptide::State::IDLE:
			arrivalBehavior(frameTime);
			break;
		case Polypeptide::State::TRAVEL:
			break;
		case Polypeptide::State::CELLFIGHT:
			break;
		case Polypeptide::State::POLYPEPTIDEFIGHT:
			break;
	}
}

void PolypeptideClient::arrivalBehavior(float frameTime)
{
	auto d = (followPoint - position).normalized();
	auto newForward = (forward + d * frameTime * (polyRotationSpeed / cellRadius * 60.)
		* (position.distance(focusCenter)
		/ (cellRadius * 10.))).normalized();
	
	auto alpha = toDegrees(forward.dot(newForward));
	rotation.z += alpha;
	
	forward = newForward;
	forward.normalize();
	position += forward * frameTime * speed * cellRadius / 100.;
}

void PolypeptideClient::updateFollowPoint(float frameTime)
{
	auto distanceVectorToCell = followPoint - focusCenter;
	auto followPointAngle = atan2(distanceVectorToCell.y, distanceVectorToCell.x);
	followPointAngle += followPointRotationSpeed * cellRadius / 40. * frameTime;
	auto r = focusRadius + cos(followPointAngle) - (rand() % maxAmplitudeEruption);
	followPoint = focusCenter + Vec3f(r * cos(followPointAngle), r * sin(followPointAngle), 0);
}

void PolypeptideClient::setCenterOfFocus(Vec3f center)
{
	focusCenter = center;
}

void PolypeptideClient::setRadiusOfFocus(float radius)
{
	focusRadius = radius;
}

void PolypeptideClient::setFocus(Vec3f center, float radius)
{
	setCenterOfFocus(center);
	setRadiusOfFocus(radius);
}

CellClient * PolypeptideClient::getOwner()
{
	return dynamic_cast<CellClient *>(owner);
}

void PolypeptideClient::setOwner(Cell* owner)
{
	Polypeptide::setOwner(owner);

	cellRadius = owner->getRadius();
	focusRadius = cellRadius - 20 < 1 ? 1 : cellRadius - 20;
}