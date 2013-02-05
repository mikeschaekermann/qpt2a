#include "GameObjectClient.h"

void GameObjectClient::draw() const
{
	pushMatrices();

	translate(position);
	rotate(rotation);
	gl::scale(scale);

	drawAtTransformation();

	popMatrices();	
}

void GameObjectClient::drawAtTransformation() const
{
	gl::color(1, 0, 0);
	drawSphere(Vec3f(0, 0, 0), radius, radius);
}