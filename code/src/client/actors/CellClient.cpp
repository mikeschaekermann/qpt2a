#include "CellClient.h"
#include "../../common/ConfigurationDataHandler.h"
#include "../managers/GameManager.h"
#include "cinder/Sphere.h"
#include "PolypeptideClient.h"

void CellClient::update(float frameTime)
{
	list<PolypeptideClient *> tmpToBeDeleted;

	for (auto it = polypeptides.begin(); it != polypeptides.end(); ++it)
	{
		it->second->update(frameTime);

		if (it->second->wantsToBeDestroyed())
		{
			tmpToBeDeleted.emplace_back(dynamic_cast<PolypeptideClient *>(it->second));
		}
	}

	for(auto it = tmpToBeDeleted.begin(); it != tmpToBeDeleted.end(); ++it)
	{
		/// remove from player's global poly list
		GAME_SCR->getMyPolypeptides().removeGameObject(*it);
		/// remove from cell's local poly list
		removePolypeptide(*it);

		delete *it;
	}
}

void CellClient::drawHealthBar(float healthBarOpacity)
{
	if (isVisible() && getHealthPoints() < maxHealthPoints)
	{
		auto outerHalfSize = Vec2f(
			CONFIG_FLOAT("data.ingamefeedback.healthBar.width"),
			CONFIG_FLOAT("data.ingamefeedback.healthBar.height")
		);

		auto cornerRadius = CONFIG_FLOAT("data.ingamefeedback.healthBar.cornerRadius");

		auto position2D = getPosition2D();

		auto outerTopLeft = position2D - outerHalfSize;
		auto outerBottomRight = position2D + outerHalfSize;
	
		ColorA borderColor(
			CONFIG_FLOAT("data.ingamefeedback.healthBar.borderColor.r"),
			CONFIG_FLOAT("data.ingamefeedback.healthBar.borderColor.g"),
			CONFIG_FLOAT("data.ingamefeedback.healthBar.borderColor.b"),
			healthBarOpacity
		);

		gl::color(borderColor);
		gl::drawSolidRoundedRect(Rectf(outerTopLeft, outerBottomRight), cornerRadius, 5);

		auto innerHalfSize = outerHalfSize - Vec2f(1, 1);

		/// health bar must not be drawn if the
		/// corner radii do not fit inside,
		/// because this leads to graphical defects!
		if (innerHalfSize.x >= cornerRadius)
		{
			auto innerTopLeft = position2D - innerHalfSize;
			auto innerBottomRight = innerTopLeft;
			innerBottomRight.y += innerHalfSize.y * 2;
			auto healthInPercent = getHealthPercentage();
			innerBottomRight.x += (innerHalfSize.x * 2) * healthInPercent;

			ColorA barColor;
			auto criticalPercentage = CONFIG_FLOAT("data.ingamefeedback.healthBar.criticalPercentage");

			if (healthInPercent > criticalPercentage)
			{
				barColor = ColorA(
					CONFIG_FLOAT("data.ingamefeedback.healthBar.goodColor.r"),
					CONFIG_FLOAT("data.ingamefeedback.healthBar.goodColor.g"),
					CONFIG_FLOAT("data.ingamefeedback.healthBar.goodColor.b"),
					healthBarOpacity
				);
			}
			else
			{
				barColor = ColorA(
					CONFIG_FLOAT("data.ingamefeedback.healthBar.criticalColor.r"),
					CONFIG_FLOAT("data.ingamefeedback.healthBar.criticalColor.g"),
					CONFIG_FLOAT("data.ingamefeedback.healthBar.criticalColor.b"),
					healthBarOpacity
				);
			}

			gl::color(barColor);
			gl::drawSolidRoundedRect(Rectf(innerTopLeft, innerBottomRight), cornerRadius, 5);
		}
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

void CellClient::draw() const
{
	GameObjectClient::draw();

	for (auto it = polypeptides.begin(); it != polypeptides.end(); ++it)
	{
		auto polypeptide = dynamic_cast<PolypeptideClient *>(it->second);
		polypeptide->draw();
	}
}

bool CellClient::addPolypeptide(Polypeptide * polypeptide)
{
	if (Cell::addPolypeptide(polypeptide))
	{
		auto polypeptideClient = dynamic_cast<PolypeptideClient *>(polypeptide);
		if (polypeptideClient != nullptr)
		{
			polypeptideClient->setFocus(position, radius);
		}
		return true;
	}

	return false;
}
