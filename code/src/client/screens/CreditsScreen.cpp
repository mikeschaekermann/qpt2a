#include "CreditsScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"
#include "TextInput.h"
#include "IpInput.h"
#include "../../common/ConfigurationDataHandler.h"

using namespace ci;

CreditsScreen::CreditsScreen()
{
	auto background = &(ASSET_MGR->getGuiTexture(string("soma-background")));
	rootItem->addSubItem(
		this,
		[]() {},
		Vec2f(CONFIG_FLOAT2("data.menu.creditscreen.background.x", 0), CONFIG_FLOAT2("data.menu.creditscreen.background.y", 0)),
		background,
		background,
		background
	);

	auto content = &(ASSET_MGR->getGuiTexture(string("soma-credits")));
	rootItem->addSubItem(
		this,
		[]() {},
		Vec2f(CONFIG_FLOAT2("data.menu.creditscreen.content.x", 0), CONFIG_FLOAT2("data.menu.creditscreen.content.y", 0)),
		content,
		content,
		content
	);

	auto backBasic = &(ASSET_MGR->getGuiTexture(string("soma-connect-basic-back")));
	auto backClick = &(ASSET_MGR->getGuiTexture(string("soma-connect-clicked-back")));
	auto backHover = &(ASSET_MGR->getGuiTexture(string("soma-connect-hover-back")));
	rootItem->addSubItem(
		this,
		[]()
		{
			SCREEN_MGR->openMenuScreen();
		},
		Vec2f(CONFIG_FLOAT2("data.menu.creditscreen.back.x", 100), CONFIG_FLOAT2("data.menu.creditscreen.back.y", 300)),
		backBasic,
		backClick,
		backHover
	);
}

CreditsScreen::~CreditsScreen(void)
{
}

void CreditsScreen::update(float frameTime)
{
}

void CreditsScreen::draw()
{
	Screen::draw();
}

bool CreditsScreen::touchBegan(const TouchWay & touchWay)
{
	return Screen::touchBegan(touchWay);
	LOG_INFO("touch way started");
};

void CreditsScreen::touchMoved(const TouchWay & touchWay)
{
	LOG_INFO("touch way moved");
};

void CreditsScreen::touchEnded(TouchWay touchWay)
{
	Screen::touchEnded(touchWay);
	LOG_INFO("touch way ended");
};

void CreditsScreen::resize(ResizeEvent event)
{
}