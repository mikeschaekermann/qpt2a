#include "MenuScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"

using namespace ci;

MenuScreen::MenuScreen()
{
	rootItem.addSubItem([]()
	{
		GAME_MGR->startGame("Mike");
	}, Vec2f::zero(), &(ASSET_MGR->getGuiTexture(string("connect"))),  &(ASSET_MGR->getGuiTexture(string("connect"))), &(ASSET_MGR->getGuiTexture(string("connect"))));
	rootItem.addSubItem([]()
	{
		system("server.exe");
	}, Vec2f::zero(), &(ASSET_MGR->getGuiTexture(string("start_server"))),  &(ASSET_MGR->getGuiTexture(string("start_server"))), &(ASSET_MGR->getGuiTexture(string("start_server"))));
	rootItem.addSubItem([]()
	{
		/// exit application
	}, Vec2f::zero(), &(ASSET_MGR->getGuiTexture(string("exit"))),  &(ASSET_MGR->getGuiTexture(string("exit"))), &(ASSET_MGR->getGuiTexture(string("exit"))));
}

MenuScreen::~MenuScreen(void)
{
}

void MenuScreen::update(float frameTime)
{
}

void MenuScreen::draw()
{
}

void MenuScreen::loadContent()
{
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