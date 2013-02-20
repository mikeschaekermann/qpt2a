#include "CellClient.h"
#include "PolypeptideClient.h"
#include "../rendering/RenderManager.h"

void PolypeptideClient::drawAtTransformation() const
{
	RENDER_MGR->renderBlackShadedModel("poly");
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
	auto newForward = (forward + d * frameTime * polyRotationSpeed * (position.distance(focusCenter) * cellRadius / 100.)).normalized();
	
	auto alpha = toDegrees(forward.dot(newForward));
	rotation.z += alpha;
	
	forward = newForward;
	forward.normalize();
	position += forward;
}

void PolypeptideClient::updateFollowPoint(float frameTime)
{
	auto distanceVectorToCell = followPoint - focusCenter;
	auto followPointAngle = atan2(distanceVectorToCell.y, distanceVectorToCell.x);
	followPointAngle += followPointRotationSpeed * frameTime;
	auto r = focusRadius + cos(followPointAngle) + (rand() % maxAmplitudeEruption);
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