#include "MenuScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"

using namespace ci;

MenuScreen::MenuScreen(ScreenManager& screenManager) :
	Screen(screenManager)
{
}

MenuScreen::~MenuScreen(void)
{
}

void MenuScreen::update(float frameTime)
{
}

void MenuScreen::draw()
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		gl::draw((*it)->tex, (*it)->pos);
	}
}

void MenuScreen::loadContent()
{
	MenuItem * m = new MenuItem();
	m->pos = Vec2f(0.f, 0.f);
	m->tex = AssetManager::getInstance()->getGuiTexture(string("img"));
	m->callback = nullptr;
	items.push_back(m);
	/// if everything was loaded set the isInitialized to true
	isInitialized = true;
}

void MenuScreen::touchBegan(const TouchWay & touchWay)
{
	LOG_INFO("touch way started");
};

void MenuScreen::touchMoved(const TouchWay & touchWay)
{
	LOG_INFO("touch way moved");
};

void MenuScreen::touchEnded(TouchWay touchWay)
{
	LOG_INFO("touch way ended");
};

void MenuScreen::touchClick(TouchWay touchWay)
{
	LOG_INFO("touch click!");
};

void MenuScreen::resize(ResizeEvent event)
{
}