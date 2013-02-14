#include "GameObjectClient.h"
#include "../managers/GameManager.h"
#include "../rendering/RenderManager.h"

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
	gl::color(ColorA(1.f, 0.f, 0.f, opacity));
	drawSphere(Vec3f(0.f, 0.f, 0.f), radius, (int) radius);
}

Vec2f GameObjectClient::getPosition2D() const
{
	return GAME_SCR.worldToScreen(getPosition());
}