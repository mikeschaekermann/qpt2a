#include "CreditsScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"
#include "TextInput.h"
#include "IpInput.h"

using namespace ci;

CreditsScreen::CreditsScreen()
{
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

void CreditsScreen::touchClick(TouchWay touchWay)
{
	LOG_INFO("touch click!");
};

void CreditsScreen::resize(ResizeEvent event)
{
}