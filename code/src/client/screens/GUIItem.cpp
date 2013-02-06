#include "GUIItem.h"

using namespace ci;
using namespace std;

GUIItem::GUIItem(std::function<void()> callback, ci::Vec2f position, const ci::gl::Texture * texture, const ci::gl::Texture * hoverTexture, const ci::gl::Texture * clickTexture) :
	callback(callback),
	position(position),
	texture(texture),
	hoverTexture(hoverTexture),
	clickTexture(clickTexture),
	isVisible(true),
	parentItem(nullptr),
	currentTexture(texture)
{
}

GUIItem * GUIItem::addSubItem(std::function<void()> callback, ci::Vec2f position, const ci::gl::Texture * texture, const ci::gl::Texture * hoverTexture, const ci::gl::Texture * clickTexture)
{
	subItems.push_back(GUIItem(callback, position, texture, hoverTexture, clickTexture));
	subItems.back().parentItem = this;
	return &subItems.back();
}

GUIItem * GUIItem::parent()
{
	return parentItem;
}

bool GUIItem::isMouseOverItem(Vec2f position)
{
	if (isPositionInItem(position))
	{
		currentTexture = hoverTexture;
		return true;
	}
	else
	{
		bool isOver = false;
		currentTexture = texture;
		for (auto it = subItems.begin(); it != subItems.end(); ++it)
		{
			isOver |= it->isMouseOverItem(position - this->position);
		}
		return isOver;
	}
}

bool GUIItem::isMouseDownOnItem(Vec2f position)
{
	if (isPositionInItem(position))
	{
		currentTexture = clickTexture;
		if (callback != nullptr)
		{
			callback();
			return true;
		}
	}
	else
	{
		currentTexture = texture;
	}
	bool isClicked = false;
	for (auto it = subItems.begin(); it != subItems.end(); ++it)
	{
		isClicked |= it->isMouseDownOnItem(position - this->position);
	}
	return isClicked;
}

void GUIItem::isMouseUp()
{
	currentTexture = texture;
	for (auto it = subItems.begin(); it != subItems.end(); ++it)
	{
		it->isMouseUp();
	}
}

void GUIItem::setPosition(ci::Vec2f newPosition)
{
	position = newPosition;
}

void GUIItem::setVisible(bool visible, bool recursively)
{
	isVisible = visible;
	if (recursively)
	{
		for (auto it = subItems.begin(); it != subItems.end(); ++it)
		{
			it->setVisible(visible, true);
		}
	}
}

void GUIItem::draw()
{
	gl::pushMatrices();
	{
		gl::translate(position);
		if (isVisible && currentTexture != nullptr)
		{
			gl::draw(*currentTexture);
		}
		for (auto it = subItems.begin(); it != subItems.end(); ++it)
		{
			it->draw();
		}
	}
	gl::popMatrices();
}

bool GUIItem::isPositionInItem(Vec2f position)
{
	if (currentTexture != nullptr)
	{
		return currentTexture->getBounds().contains(position);
	}

	return false;
}