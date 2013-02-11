#include "MenuScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"

using namespace ci;

MenuScreen::MenuScreen()
{
	auto connectTexture = &(ASSET_MGR->getGuiTexture(string("connect")));
	rootItem->addSubItem(
		this,
		[]()
		{
			SCREEN_MGR->openConnectScreen();
		},
		Vec2f(100, 50),
		connectTexture,
		connectTexture,
		connectTexture
	);

	auto startServerTexture = &(ASSET_MGR->getGuiTexture(string("startServer")));
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
				CreateProcess
						(
						TEXT("..\\..\\server\\Debug\\server.exe"),
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
			sleep(500);
			GAME_MGR->startGame("Mike");
		},
		Vec2f(100, 200),
		startServerTexture,
		startServerTexture,
		startServerTexture
	);

	auto creditsTexture = &(ASSET_MGR->getGuiTexture(string("credits")));
	rootItem->addSubItem(
		this,
		[]()
		{
			SCREEN_MGR->openCreditsScreen();
		},
		Vec2f(100, 300),
		creditsTexture,
		creditsTexture,
		creditsTexture
	);

	auto exitButton = &(ASSET_MGR->getGuiTexture(string("exit")));
	rootItem->addSubItem(
		this,
		[]()
		{
			GAME_MGR->quit();
		},
		Vec2f(100, 400),
		exitButton,
		exitButton,
		exitButton
	);
}

MenuScreen::~MenuScreen(void)
{
}

void MenuScreen::terminateServer()
{
	TerminateProcess(serverProcessInfo.hProcess, 0);
}