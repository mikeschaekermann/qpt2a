#include "ConnectScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../managers/GameManager.h"
#include "TextInput.h"
#include "IpInput.h"
#include "../../common/ConfigurationDataHandler.h"

using namespace ci;

ConnectScreen::ConnectScreen()
{
	auto background = &(ASSET_MGR->getGuiTexture(string("soma-background")));
	rootItem->addSubItem(
		this,
		[]() {},
		Vec2f(CONFIG_FLOAT("data.menu.connectcreen.background.x"), CONFIG_FLOAT("data.menu.connectcreen.background.y")),
		background,
		background,
		background
	);

	auto headline = &(ASSET_MGR->getGuiTexture(string("soma-connect-headline")));
	rootItem->addSubItem(
		this,
		[]() {},
		Vec2f(CONFIG_FLOAT("data.menu.connectcreen.headline.x"), CONFIG_FLOAT("data.menu.connectcreen.headline.y")),
		headline,
		headline,
		headline
	);

	auto nameBox = &(ASSET_MGR->getGuiTexture(string("soma-connect-username-without-font")));

	TextInput *nameTextBox = new TextInput(
		this, 
		[]() { },
		Vec2f(CONFIG_FLOAT("data.menu.connectcreen.username.x"), CONFIG_FLOAT("data.menu.connectcreen.username.y")), 
		nameBox, 
		nameBox, 
		nameBox
	);
	auto nameBoxItem = rootItem->addSubItem(nameTextBox);
	nameTextBox->setValue(static_cast<ostringstream*>(&(ostringstream() <<  rand()))->str());


	// TODO: replace texture?
	messageBox = new TextInput(
		this, 
		[]() { },
		Vec2f(CONFIG_FLOAT("data.menu.connectcreen.messagebox.x"), CONFIG_FLOAT("data.menu.connectcreen.messagebox.y")), 
		nameBox, 
		nameBox, 
		nameBox
	);
	messageBox->setEnabled(false);
	messageBox->setRenderBox(false);
	rootItem->addSubItem(messageBox);


	auto ipBox = &(ASSET_MGR->getGuiTexture(string("soma-connect-server-without-font")));
	auto ipBoxItem = rootItem->addSubItem(new IpInput(
		this,
		[]()
		{
			
		},
		Vec2f(CONFIG_FLOAT("data.menu.connectcreen.server.x"), CONFIG_FLOAT("data.menu.connectcreen.server.y")),
		ipBox,
		ipBox,
		ipBox)
	);

	auto connectTextureBasic = &(ASSET_MGR->getGuiTexture(string("soma-connect-basic-connect")));
	auto connectTextureClick = &(ASSET_MGR->getGuiTexture(string("soma-connect-clicked-connect")));
	auto connectTextureHover = &(ASSET_MGR->getGuiTexture(string("soma-connect-hover-connect")));
	connectButton = rootItem->addSubItem(
		this,
		[ipBoxItem, nameBoxItem, this]()
		{
			connectButton->setEnabled(false);
			backButton->setEnabled(false);
			connectTimeout = 5.f;
			GAME_MGR->startGame(nameBoxItem->getValue(), ipBoxItem->getValue());
		},
		Vec2f(CONFIG_FLOAT("data.menu.connectcreen.connect.x"), CONFIG_FLOAT("data.menu.connectcreen.connect.y")),
		connectTextureBasic,
		connectTextureClick,
		connectTextureHover
	);

	auto backBasic = &(ASSET_MGR->getGuiTexture(string("soma-connect-basic-back")));
	auto backClick = &(ASSET_MGR->getGuiTexture(string("soma-connect-clicked-back")));
	auto backHover = &(ASSET_MGR->getGuiTexture(string("soma-connect-hover-back")));
	backButton = rootItem->addSubItem(
		this,
		[this]()
		{
			if (connectButton->isEnabled())
			{
				SCREEN_MGR->openMenuScreen();
			}
			connectButton->setEnabled(true);
			connectButton->setVisible(true);
			messageBox->setValue("");
		},
		Vec2f(CONFIG_FLOAT("data.menu.connectcreen.back.x"), CONFIG_FLOAT("data.menu.connectcreen.back.y")),
		backBasic,
		backClick,
		backHover
	);
}

ConnectScreen::~ConnectScreen(void)
{
}

void ConnectScreen::update(float frameTime)
{
	if (!connectButton->isEnabled() || !backButton->isEnabled())
	{
		if (connectTimeout > 0.f)
		{
			connectTimeout -= frameTime;
		}
		else
		{
			connectButton->setEnabled(true);
			backButton->setEnabled(true);
			messageBox->setValue("Connection will be established \nwhen game is full.");
		}
	}
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

void ConnectScreen::resize(ResizeEvent event)
{
}

void ConnectScreen::joinFailure(JoinErrorCode error)
{
	connectButton->setVisible(true);
	connectButton->setEnabled(true);
	backButton->setEnabled(true);

	switch(error.getType())
	{
	case JoinErrorCode::GameIsFull:
		messageBox->setValue("Game is full.");
		break;
	case JoinErrorCode::NameAlreadyTaken:
		messageBox->setValue("Name is already taken.");
		break;
	default:
		messageBox->setValue("Join failed: Unknown (I'm outta here!)");
		break;
	}
}

void ConnectScreen::joinSuccess()
{
	connectButton->setEnabled(true);
	connectButton->setVisible(false);
	backButton->setEnabled(true);

	connectTimeout = 0.f;
	messageBox->setValue("Waiting for other players.");
}
