#include "ConnectScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"
#include "TextInput.h"
#include "IpInput.h"

using namespace ci;

ConnectScreen::ConnectScreen()
{
	auto nameBox = &(ASSET_MGR->getGuiTexture(string("textBox")));
	auto nameBoxItem = rootItem->addSubItem(new TextInput(
		this,
		[]()
		{
			
		},
		Vec2f(100, 40),
		nameBox,
		nameBox,
		nameBox)
	);

	auto ipBox = &(ASSET_MGR->getGuiTexture(string("textBox")));
	auto ipBoxItem = rootItem->addSubItem(new IpInput(
		this,
		[]()
		{
			
		},
		Vec2f(100, 150),
		ipBox,
		ipBox,
		ipBox)
	);

	auto connectTexture = &(ASSET_MGR->getGuiTexture(string("connect")));
	rootItem->addSubItem(
		this,
		[ipBoxItem, nameBoxItem]()
		{
			GAME_MGR->startGame(nameBoxItem->getValue(), ipBoxItem->getValue());
		},
		Vec2f(100, 220),
		connectTexture,
		connectTexture,
		connectTexture
	);

	auto back = &(ASSET_MGR->getGuiTexture(string("back")));
	rootItem->addSubItem(
		this,
		[]()
		{
			SCREEN_MGR->openMenuScreen();
		},
		Vec2f(100, 300),
		back,
		back,
		back
	);
}

ConnectScreen::~ConnectScreen(void)
{
}

void ConnectScreen::update(float frameTime)
{
}

void ConnectScreen::draw()
{
	Screen::draw();
}

void ConnectScreen::touchBegan(const TouchWay & touchWay)
{
	Screen::touchBegan(touchWay);
	LOG_INFO("touch way started");
};

void ConnectScreen::touchMoved(const TouchWay & touchWay)
{
	LOG_INFO("touch way moved");
};

void ConnectScreen::touchEnded(TouchWay touchWay)
{
	Screen::touchEnded(touchWay);
	LOG_INFO("touch way ended");
};

void ConnectScreen::touchClick(TouchWay touchWay)
{
	LOG_INFO("touch click!");
};

void ConnectScreen::resize(ResizeEvent event)
{
}