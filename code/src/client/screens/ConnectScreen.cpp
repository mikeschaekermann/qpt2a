#include "ConnectScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"
#include "TextInput.h"

using namespace ci;

ConnectScreen::ConnectScreen()
{
	auto back = &(ASSET_MGR->getGuiTexture(string("back")));
	rootItem->addSubItem(
		this,
		[]()
		{
			system("server.exe");
		},
		Vec2f(100, 200),
		back,
		back,
		back
	);

	auto connectTexture = &(ASSET_MGR->getGuiTexture(string("connect")));
	rootItem->addSubItem(
		this,
		[]()
		{
			GAME_MGR->startGame("Daniel");
		},
		Vec2f(100, 200),
		connectTexture,
		connectTexture,
		connectTexture
	);

	auto textBox = &(ASSET_MGR->getGuiTexture(string("textBox")));
	rootItem->addSubItem(new TextInput(
		this,
		[]()
		{
			/// exit application
		},
		Vec2f(100, 350),
		textBox,
		textBox,
		textBox)
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

bool ConnectScreen::touchBegan(const TouchWay & touchWay)
{
	return Screen::touchBegan(touchWay);
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