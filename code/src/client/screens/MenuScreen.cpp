#include "MenuScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"

using namespace ci;

MenuScreen::MenuScreen()
{
	auto connectTexture = &(ASSET_MGR->getGuiTexture(string("connect")));
	rootItem.addSubItem(
		[]()
		{
			GAME_MGR->startGame("Mike");
		},
		Vec2f(100, 50),
		connectTexture,
		connectTexture,
		connectTexture
	);

	auto startServerTexture = &(ASSET_MGR->getGuiTexture(string("startServer")));
	rootItem.addSubItem(
		[]()
		{
			system("server.exe");
		},
		Vec2f(100, 200),
		startServerTexture,
		startServerTexture,
		startServerTexture
	);

	auto exitButton = &(ASSET_MGR->getGuiTexture(string("exit")));
	rootItem.addSubItem(
		[]()
		{
			/// exit application
		},
		Vec2f(100, 350),
		exitButton,
		exitButton,
		exitButton
	);
}

MenuScreen::~MenuScreen(void)
{
}

void MenuScreen::update(float frameTime)
{
}

void MenuScreen::draw()
{
	Screen::draw();
}

void MenuScreen::loadContent()
{
	/// if everything was loaded set the isInitialized to true
	isInitialized = true;
}

void MenuScreen::touchBegan(const TouchWay & touchWay)
{
	Screen::touchBegan(touchWay);
	LOG_INFO("touch way started");
};

void MenuScreen::touchMoved(const TouchWay & touchWay)
{
	LOG_INFO("touch way moved");
};

void MenuScreen::touchEnded(TouchWay touchWay)
{
	Screen::touchEnded(touchWay);
	LOG_INFO("touch way ended");
};

void MenuScreen::touchClick(TouchWay touchWay)
{
	LOG_INFO("touch click!");
};

void MenuScreen::resize(ResizeEvent event)
{
}