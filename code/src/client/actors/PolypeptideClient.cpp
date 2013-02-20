#include "CellClient.h"
#include "PolypeptideClient.h"
#include "../rendering/RenderManager.h"

void PolypeptideClient::drawAtTransformation() const
{
	gl::pushMatrices();
		RENDER_MGR->renderBlackShadedModel("poly");
	gl::popMatrices();
}

void PolypeptideClient::update(float frameTime)
{
	switch(state)
	{
		case Polypeptide::State::IDLE:
			circularMovement(frameTime);
			break;
		case Polypeptide::State::TRAVEL:
			break;
		case Polypeptide::State::CELLFIGHT:
			break;
		case Polypeptide::State::POLYPEPTIDEFIGHT:
			break;
	}
}

void PolypeptideClient::circularMovement(float frameTime)
{
	if(!isInCircularMovement)
	{
		auto d = focusCenter - position;
		auto a = atan2(d.x, d.y);

		auto t = Vec3f(focusCenter.x, focusCenter.y + focusRadius, focusCenter.z);
		forward = (t - position).normalized();
		position += forward * speed * frameTime;

		if(position.distance(t) < 1.0)
		{
			isInCircularMovement = true;
			position = t;
		}
	}
	else
	{
		auto d = focusCenter - position;
		auto a = atan2(d.x, d.y);
		auto r = cos(a) + isInCircularMovement;
		auto t = Vec3f(r * cos(a), r * sin(a), position.z);
	
		forward = forward + t.normalized();
		forward.normalize();
		position += forward * speed * frameTime;
	}
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