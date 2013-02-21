#include "GameObjectClient.h"
#include "../managers/GameManager.h"
#include "../rendering/RenderManager.h"

void GameObjectClient::draw() const
{
	if (visible)
	{
		pushMatrices();

		translate(GameObject::position);
		rotate(GameObject::rotation);
		gl::scale(GameObject::scale);

		drawAtTransformation();

		popMatrices();
	}
}

void GameObjectClient::drawAtTransformation() const
{
	gl::color(ColorA(1.f, 0.f, 0.f, opacity));
	drawSphere(Vec3f(0.f, 0.f, 0.f), radius, (int) radius);
}

Vec2f GameObjectClient::getPosition2D() const
{
	return GAME_SCR->worldToScreen(getPosition());
}

float GameObjectClient::getOpacity() const
{
	return opacity;
}

void GameObjectClient::setOpacity(float newOpacity)
{
	standardOpacity = opacity = min<float>(max<float>(newOpacity, 0.0), 1.0);
}

float GameObjectClient::getHue() const
{
	return hue;
}

void GameObjectClient::setHue(float newHue)
{
	standardHue = hue = min<float>(max<float>(newHue, 0.0), 1.0);
}

float GameObjectClient::getSaturation() const
{
	return saturation;
}

void GameObjectClient::setSaturation(float newSaturation)
{
	standardSaturation = saturation = min<float>(max<float>(newSaturation, 0.0), 1.0);
}

float GameObjectClient::getBrightness() const
{
	return brightness;
}

void GameObjectClient::setBrightness(float newBrightness)
{
	standardBrightness = brightness = min<float>(max<float>(newBrightness, 0.0), 1.0);
}

bool GameObjectClient::isVisible() const
{
	return visible;
}

void GameObjectClient::setVisibility(bool newVisible)
{
	visible = newVisible;
}

void GameObjectClient::show()
{
	setVisibility(true);
}

void GameObjectClient::hide()
{
	setVisibility(false);
}

void GameObjectClient::resetColor()
{
	hue = standardHue;
	saturation = standardSaturation;
	brightness = standardBrightness;
	opacity = standardOpacity;
}

void GameObjectClient::setHoverColor()
{
	brightness = 1.f;
	opacity = 0.8f;
}

void GameObjectClient::setMouseDownColor()
{
	brightness = 0.5f;
	opacity = 1.f;
}