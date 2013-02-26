#include "MenuScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"
#include "../../common/ConfigurationDataHandler.h"

using namespace ci;

MenuScreen::MenuScreen()
{
	auto background = &(ASSET_MGR->getGuiTexture(string("soma-menu-background")));
	rootItem->addSubItem(
		this,
		[]() {},
		Vec2f(CONFIG_FLOAT("data.menu.menuscreen.background.x"), CONFIG_FLOAT("data.menu.menuscreen.background.y")),
		background,
		background,
		background
	);

	auto connectTextureBasic = &(ASSET_MGR->getGuiTexture(string("soma-menu-basic-connect")));
	auto connectTextureClick = &(ASSET_MGR->getGuiTexture(string("soma-menu-clicked-connect")));
	auto connectTextureHover = &(ASSET_MGR->getGuiTexture(string("soma-menu-hover-connect")));
	rootItem->addSubItem(
		this,
		[]()
		{
			SCREEN_MGR->openConnectScreen();
		},
		Vec2f(CONFIG_FLOAT("data.menu.menuscreen.connect.x"), CONFIG_FLOAT("data.menu.menuscreen.connect.y")),
		connectTextureBasic,
		connectTextureClick,
		connectTextureHover
	);

	auto startServerTextureBasic = &(ASSET_MGR->getGuiTexture(string("soma-menu-basic-start")));
	auto startServerTextureClick = &(ASSET_MGR->getGuiTexture(string("soma-menu-clicked-start")));
	auto startServerTextureHover = &(ASSET_MGR->getGuiTexture(string("soma-menu-hover-start")));
	rootItem->addSubItem(
		this,
		[this]()
		{
			MenuScreen * self = this;

			boost::thread([self]()
			{

				ZeroMemory( &(self->serverStartupInfo), sizeof((self->serverStartupInfo)) );
				(self->serverStartupInfo).cb = sizeof((self->serverStartupInfo));
				ZeroMemory( &(self->serverProcessInfo), sizeof((self->serverProcessInfo)) );
				if (!
						CreateProcess(
							TEXT("..\\..\\server\\Debug\\server.exe"),
							NULL,NULL,NULL,FALSE,
							CREATE_NEW_CONSOLE,
							NULL,NULL,
							&(self->serverStartupInfo),
							&(self->serverProcessInfo)
						)
					&&
						!CreateProcess(
							TEXT("..\\server\\server.exe"),
							NULL,NULL,NULL,FALSE,
							CREATE_NEW_CONSOLE,
							NULL,NULL,
							&(self->serverStartupInfo),
							&(self->serverProcessInfo)
						)
					)
					
				{
					LOG_ERROR("Could not start server in a thread!");
				}
				else
				{
					LOG_INFO("Server started in a thread!");
				}
			});
		},
		Vec2f(CONFIG_FLOAT("data.menu.menuscreen.start.x"), CONFIG_FLOAT("data.menu.menuscreen.start.y")),
		startServerTextureBasic,
		startServerTextureClick,
		startServerTextureHover
	);

	auto creditsTextureBasic = &(ASSET_MGR->getGuiTexture(string("soma-menu-basic-credits")));
	auto creditsTextureClick = &(ASSET_MGR->getGuiTexture(string("soma-menu-clicked-credits")));
	auto creditsTextureHover = &(ASSET_MGR->getGuiTexture(string("soma-menu-hover-credits")));
	rootItem->addSubItem(
		this,
		[]()
		{
			SCREEN_MGR->openCreditsScreen();
		},
		Vec2f(CONFIG_FLOAT("data.menu.menuscreen.credits.x"), CONFIG_FLOAT("data.menu.menuscreen.credits.y")),
		creditsTextureBasic,
		creditsTextureClick,
		creditsTextureHover
	);

	auto exitButtonBasic = &(ASSET_MGR->getGuiTexture(string("soma-menu-basic-exit")));
	auto exitButtonClick = &(ASSET_MGR->getGuiTexture(string("soma-menu-clicked-exit")));
	auto exitButtonHover = &(ASSET_MGR->getGuiTexture(string("soma-menu-hover-exit")));
	rootItem->addSubItem(
		this,
		[]()
		{
			GAME_MGR->quit();
		},
		Vec2f(CONFIG_FLOAT("data.menu.menuscreen.exit.x"), CONFIG_FLOAT("data.menu.menuscreen.exit.y")),
		exitButtonBasic,
		exitButtonClick,
		exitButtonHover
	);
}

MenuScreen::~MenuScreen(void)
{
}

void MenuScreen::terminateServer()
{
	TerminateProcess(serverProcessInfo.hProcess, 0);
}