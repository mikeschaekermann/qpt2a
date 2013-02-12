#include "GUIItem.h"

using namespace ci;
using namespace std;

GUIItem::GUIItem(Screen* screen, std::function<void()> callback, ci::Vec2f position, const ci::gl::Texture * texture, const ci::gl::Texture * hoverTexture, const ci::gl::Texture * clickTexture) :
	screen(screen),
	callback(callback),
	position(position),
	texture(texture),
	hoverTexture(hoverTexture),
	clickTexture(clickTexture),
	isVisible(true),
	hasFocus(false),
	parentItem(nullptr),
	currentTexture(texture)
{
}

GUIItem::~GUIItem()
{
	for(auto it = subItems.begin(); it != subItems.end(); ++it)
	{
		delete (*it);
	}
}

GUIItem * GUIItem::addSubItem(Screen* screen, std::function<void()> callback, ci::Vec2f position, const ci::gl::Texture * texture, const ci::gl::Texture * hoverTexture, const ci::gl::Texture * clickTexture)
{
	subItems.push_back(new GUIItem(screen, callback, position, texture, hoverTexture, clickTexture));
	subItems.back()->parentItem = this;
	return subItems.back();
}

GUIItem* GUIItem::addSubItem(GUIItem* item)
{
	subItems.push_back(item);
	subItems.back()->parentItem = this;
	return subItems.back();
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
			isOver |= (*it)->isMouseOverItem(position - this->position);
		}
		return isOver;
	}
}

bool GUIItem::isMouseDownOnItem(Vec2f position)
{
	if (isPositionInItem(position))
	{
		currentTexture = clickTexture;
		return true;
	}
	else
	{
		currentTexture = texture;
	}
	bool isMouseDown = false;
	for (auto it = subItems.begin(); it != subItems.end(); ++it)
	{
		isMouseDown |= (*it)->isMouseDownOnItem(position - this->position);
	}
	return isMouseDown;
}

void GUIItem::isMouseUp()
{
	currentTexture = texture;
	for (auto it = subItems.begin(); it != subItems.end(); ++it)
	{
		(*it)->isMouseUp();
	}
}

bool GUIItem::hasMouseClickedOnItem(ci::Vec2f position)
{
	if (isPositionInItem(position))
	{
		if (callback != nullptr)
		{
			callback();
			hasFocus = true;
			screen->focusedItem = this;
			return true;
		}
	}
	else
	{
		currentTexture = texture;
		hasFocus = false;
	}
	bool hasClicked = false;
	for (auto it = subItems.begin(); it != subItems.end(); ++it)
	{
		hasClicked |= (*it)->hasMouseClickedOnItem(position - this->position);
	}
	return hasClicked;
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
			(*it)->setVisible(visible, true);
		}
	}
}

void GUIItem::draw()
{
	gl::pushMatrices();
	{
		gl::translate(position);
		if(isVisible && currentTexture != nullptr && !hasFocus)
		{
			gl::draw(*currentTexture);
		}
		else if(isVisible && hasFocus && clickTexture)
		{
			gl::draw(*clickTexture);
		}

		for (auto it = subItems.begin(); it != subItems.end(); ++it)
		{
			(*it)->draw();
		}
	}
	gl::popMatrices();
}

bool GUIItem::isPositionInItem(Vec2f position)
{
	if (isVisible && currentTexture != nullptr)
	{
		return currentTexture->getBounds().contains(position - this->position);
	}

	return false;
}

void GUIItem::onKeyInput(KeyEvent& e)
{
	for(auto it = subItems.begin(); it != subItems.end(); ++it)
	{
		(*it)->onKeyInput(e);
	}
}