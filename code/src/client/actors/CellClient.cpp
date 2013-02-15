#include "CellClient.h"
#include "../../common/ConfigurationDataHandler.h"

void CellClient::drawHealthBar(float healthBarOpacity)
{
	if (healthPoints < maxHealthPoints)
	{
		auto outerHalfSize = Vec2f(
			CONFIG_FLOAT2("data.ingamefeedback.healthBar.width", 50),
			CONFIG_FLOAT2("data.ingamefeedback.healthBar.height", 10)
		);

		auto cornerRadius = CONFIG_FLOAT2("data.ingamefeedback.healthBar.cornerRadius", 5);

		auto position2D = getPosition2D();

		auto outerTopLeft = position2D - outerHalfSize;
		auto outerBottomRight = position2D + outerHalfSize;
	
		ColorA borderColor(
			CONFIG_FLOAT2("data.ingamefeedback.healthBar.borderColor.r", 0.f),
			CONFIG_FLOAT2("data.ingamefeedback.healthBar.borderColor.g", 0.f),
			CONFIG_FLOAT2("data.ingamefeedback.healthBar.borderColor.b", 0.f),
			healthBarOpacity
		);

		gl::color(borderColor);
		gl::drawSolidRoundedRect(Rectf(outerTopLeft, outerBottomRight), cornerRadius, 5);

		auto innerHalfSize = outerHalfSize - Vec2f(1, 1);

		auto innerTopLeft = position2D - innerHalfSize;
		auto innerBottomRight = innerTopLeft;
		innerBottomRight.y += innerHalfSize.y * 2;
		auto healthInPercent = getHealthPercentage();
		innerBottomRight.x += (innerHalfSize.x * 2) * healthInPercent;

		ColorA barColor;
		auto criticalPercentage = CONFIG_FLOAT2("data.ingamefeedback.healthBar.criticalPercentage", 0.3f);

		if (healthInPercent > criticalPercentage)
		{
			barColor = ColorA(
				CONFIG_FLOAT2("data.ingamefeedback.healthBar.goodColor.r", 0.f),
				CONFIG_FLOAT2("data.ingamefeedback.healthBar.goodColor.g", 1.f),
				CONFIG_FLOAT2("data.ingamefeedback.healthBar.goodColor.b", 0.f),
				healthBarOpacity
			);
		}
		else
		{
			barColor = ColorA(
				CONFIG_FLOAT2("data.ingamefeedback.healthBar.criticalColor.r", 1.f),
				CONFIG_FLOAT2("data.ingamefeedback.healthBar.criticalColor.g", 0.f),
				CONFIG_FLOAT2("data.ingamefeedback.healthBar.criticalColor.b", 0.f),
				healthBarOpacity
			);
		}

		gl::color(barColor);
		gl::drawSolidRoundedRect(Rectf(innerTopLeft, innerBottomRight), cornerRadius, 5);
	}
}

float CellClient::getHealthPercentage() const
{
	return (healthPoints / maxHealthPoints);
}

float CellClient::getBrightnessFromHealthPoints() const
{
	return (getBrightness() * getHealthPercentage());
}

float CellClient::getOpacityFromHealthPoints() const
{
	return (getOpacity() * getHealthPercentage());
}
