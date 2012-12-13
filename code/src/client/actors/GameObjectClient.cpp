#include "GameObjectClient.h"

void GameObjectClient::draw() const
{
	pushMatrices();

	translate(m_position);
	rotate(m_rotation);
	scale(m_scale);

	drawAtTransformation();

	popMatrices();
}

void GameObjectClient::drawAtTransformation() const
{
	drawSphere(Vec3f(0, 0, 0), 15);
}