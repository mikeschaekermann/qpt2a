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
			moveIdleFormation(frameTime);
			break;
		case Polypeptide::State::TRAVEL:
			break;
		case Polypeptide::State::CELLFIGHT:
			break;
		case Polypeptide::State::POLYPEPTIDEFIGHT:
			break;
	}
}

void PolypeptideClient::moveIdleFormation(float frameTime)
{
	if(!isOnIdleCircle)
	{
		forward = Vec3f(0, 1, 0);
		position += forward * speed * frameTime;

		if(position.y > rotationPoint.y + idleRadius)
		{
			isOnIdleCircle = true;
		}
	}
	else
	{
		auto d = rotationPoint - position;
		auto a = atan2(d.x, d.y);
		auto r = cos(a) + idleRadius;
		auto t = Vec3f(r * cos(a), r * sin(a), position.z);
	
		forward = forward + t.normalized();
		forward.normalize();
		position += forward * speed * frameTime;
	}
}