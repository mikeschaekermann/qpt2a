#include "CellClient.h"
#include "PolypeptideClient.h"
#include "../rendering/RenderManager.h"
#include "CellClient.h"
#include "../managers/GameManager.h"

PolypeptideClient::PolypeptideClient(Vec3f focusCenter, float focusRadius, float cellRadius) :
	speed(CONFIG_FLOAT("data.polypeptide.speed")),
	polyRotationSpeed(CONFIG_FLOAT("data.polypeptide.rotationSpeed")),
	followPointRotationSpeed(CONFIG_FLOAT("data.polypeptide.pointRotationSpeed")),
	maxAmplitudeEruption(CONFIG_INT("data.polypeptide.maxPointAmplitudeEruption")),
	forward(Vec3f(1, 0, 0)),
	focusCenter(focusCenter),
	focusRadius(focusRadius),
	cellRadius(cellRadius),
	selfDestruct(false),
	dieTrying(false),
	wayBackFromFocus(false),
	remainingDistancePercentagePerSecond(1.f - CONFIG_FLOAT("data.polypeptide.attack.distancePercentageTravelledPerSecond"))
{
	this->cellRadius = cellRadius;

	scale = Vec3f(
		CONFIG_FLOAT("data.polypeptide.scale"),
		CONFIG_FLOAT("data.polypeptide.scale"),
		CONFIG_FLOAT("data.polypeptide.scale")
	);

	float followPointAngle = toRadians(float(rand() % 361));
	followPoint = Vec3f(focusCenter.x + (focusRadius * cos(followPointAngle)), focusCenter.y + (focusRadius * sin(followPointAngle)), focusCenter.z);

	this->show();
}

PolypeptideClient::PolypeptideClient() :
	speed(CONFIG_FLOAT("data.polypeptide.speed")),
	polyRotationSpeed(CONFIG_FLOAT("data.polypeptide.rotationSpeed")),
	followPointRotationSpeed(CONFIG_FLOAT("data.polypeptide.pointRotationSpeed")),
	maxAmplitudeEruption(CONFIG_INT("data.polypeptide.maxPointAmplitudeEruption")),
	forward(Vec3f(1, 0, 0)),
	focusCenter(),
	focusRadius(0.f),
	cellRadius(0.f),
	selfDestruct(false),
	dieTrying(false),
	wayBackFromFocus(false),
	remainingDistancePercentagePerSecond(1.f - CONFIG_FLOAT("data.polypeptide.attack.distancePercentageTravelledPerSecond"))
{
	this->cellRadius = cellRadius;

	scale = Vec3f(
		CONFIG_FLOAT("data.polypeptide.scale"),
		CONFIG_FLOAT("data.polypeptide.scale"),
		CONFIG_FLOAT("data.polypeptide.scale")
	);

	float followPointAngle = toRadians(float(rand() % 361));
	followPoint = Vec3f(focusCenter.x + (focusRadius * cos(followPointAngle)), focusCenter.y + (focusRadius * sin(followPointAngle)), focusCenter.z);

	this->show();
}

PolypeptideClient::~PolypeptideClient()
{
	if (selfDestruct)
	{
		GAME_SCR.removeGameObjectToUpdate(this);
		GAME_SCR.removeGameObjectToDraw(this);
		/// TODO delete from other containers my own polys could be in!
	}
}

void PolypeptideClient::draw() const
{
	GameObjectClient::draw();
}

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
		case Polypeptide::State::CELLFIGHT:
			attackBehavior(frameTime);
			break;
		case Polypeptide::State::TRAVEL:
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

void PolypeptideClient::attackBehavior(float frameTime)
{
	Vec3f currentTarget;

	if (!wayBackFromFocus)
	{
		currentTarget = focusCenter;
	}
	else
	{
		currentTarget = owner->getPosition();
	}

	/// current target is reached
	if (position.distance(currentTarget) < 20)
	{
		/// on its way to the attack target
		if (!wayBackFromFocus)
		{
			wayBackFromFocus = true;

			if (dieTrying)
			{
				delete this;
			}
		}
		/// on its way back to its origin
		else
		{
			setState(Polypeptide::IDLE);
			setFocus(owner->getPosition(), owner->getRadius());

			if (selfDestruct)
			{
				delete this;
			}
		}
	}

	auto distance = currentTarget - position;
	position += distance * (1.f - pow(remainingDistancePercentagePerSecond, frameTime));
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

void PolypeptideClient::setAttackOptions(bool selfDestruct, bool dieTrying)
{
	this->selfDestruct = selfDestruct;
	this->dieTrying = dieTrying;
	this->wayBackFromFocus = false;
}
