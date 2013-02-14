#include "GameScreen.h"
#include "../managers/GameManager.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../actors/CellClient.h"
#include "../actors/StandardCellClient.h"
#include "../actors/GameObjectClient.h"
#include "GameScreenStates/GameScreenStateNeutral.h"
#include "GameScreenStates/GameScreenStateCreateCell.h"
#include "../../common/network/messages/enum/CellType.h"
#include "../rendering/RenderManager.h"
#include <ostream>

GameScreen::GameScreen()
{
	state = new GameScreenStateNeutral(this);
	RenderManager::getInstance()->zoomToWorld();

	worldRadius = CONFIG_FLOAT1("data.world.radius");

	cellMenu = rootItem->addSubItem(this, nullptr);

	cellMenuButtons.insert(make_pair("standard", cellMenu->addSubItem(
		this,
		[this]()
		{
			switchToState(new GameScreenStateCreateCell(this, CellType::StandardCell));
		},
		Vec2f::zero(),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-standard-normal"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-standard-hover"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-standard-clicked")))
	)));

	cellMenuButtons.insert(make_pair("knochen", cellMenu->addSubItem(
		this,
		[this]()
		{
			LOG_INFO("Create bone cell button was clicked!");
		},
		Vec2f::zero(),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-knochen-normal"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-knochen-hover"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-knochen-clicked")))
	)));

	cellMenuButtons.insert(make_pair("verbindung", cellMenu->addSubItem(
		this,
		[this]()
		{
			LOG_INFO("Create branch cell button was clicked!");
		},
		Vec2f::zero(),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-verbindung-normal"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-verbindung-hover"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-verbindung-clicked")))
	)));

	cellMenuButtons.insert(make_pair("polypeptid", cellMenu->addSubItem(
		this,
		[this]()
		{
			LOG_INFO("Create attacker polypeptide button was clicked!");
		},
		Vec2f::zero(),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-polypeptid-normal"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-polypeptid-hover"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-polypeptid-clicked")))
	)));

	cellMenu->setVisible(false);
}

GameScreen::~GameScreen(void)
{
	if (state != nullptr)
	{
		delete state;
	}
}

void GameScreen::update(float frameTime)
{
	state->update(frameTime);

	containerMutex.lock();

	for (auto it = textList.begin(); it != textList.end();)
	{
		if (it->getTimeDeath() < getElapsedSeconds())
		{
			it = textList.erase(it);
		}
		else
		{
			++it;
		}
	}

	containerMutex.unlock();

	containerMutex.lock();

	for (auto it = gameObjectsToUpdate.begin(); it != gameObjectsToUpdate.end(); ++it)
	{
		it->second->update(frameTime);
	}

	containerMutex.unlock();
}

void GameScreen::draw()
{
	RenderManager::getInstance()->setUp3d();

	gl::pushMatrices();
		gl::scale(worldRadius, worldRadius, worldRadius);
		RenderManager::getInstance()->renderModel("petriDish", "test",
												  Vec4f(0.1f, 0.1f, 0.1f, 0.1f),
												  Vec4f(0.1f, 0.1f, 0.1f, 0.2f),
												  Vec4f(1.f, 1.f, 1.f, 0.6f),
												  100.f);
	gl::popMatrices();

	containerMutex.lock();

	for (auto it = gameObjectsToDraw.begin(); it != gameObjectsToDraw.end(); ++it)
	{
		it->second->draw();
	}

	containerMutex.unlock();
	containerMutex.lock();

	for (auto it = cellsIncomplete.begin(); it != cellsIncomplete.end(); ++it)
	{
		it->second->draw();
	}

	containerMutex.unlock();
	containerMutex.lock();

	for (auto it = cellPreviews.begin(); it != cellPreviews.end(); ++it)
	{
		(*it)->draw();
	}

	containerMutex.unlock();

	state->draw3D();

	RenderManager::getInstance()->shutdown3d();

	gl::color(ColorA(1, 1, 1, 1));
	rootItem->draw();

	///////////// 2D rendering

	state->draw2D();

	containerMutex.lock();

	for (auto it = textList.begin(); it != textList.end(); ++it)
	{
		/// TODO: Load Font size in float?
		drawString(it->getText(), worldToScreen(it->getPos()), ColorA(1.f, 0.f, 0.f, 1.f), 
			Font(CONFIG_STRING2("data.ingamefeedback.renderedDamage.font", "Comic Sans MS"), (float) CONFIG_INT2("data.ingamefeedback.renderedDamage.size", 18)));
	}

	containerMutex.unlock();

	/// DRAW CELL IDs FOR DEBUGGING
	containerMutex.lock();

	for (auto it = gameObjectsToDraw.begin(); it != gameObjectsToDraw.end(); ++it)
	{
		auto idPosition = worldToScreen(it->second->getPosition()) - Vec2f(10, 15);
		auto id = it->second->getId();

		drawString(stringify(ostringstream() << id), idPosition, ColorA(1.f, 0.f, 0.f, 1.f), 
			Font(CONFIG_STRING2("data.ingamefeedback.renderedDamage.font", "Comic Sans MS"), (float) CONFIG_INT2("data.ingamefeedback.renderedDamage.size", 18)));
	}

	containerMutex.unlock();
}

bool GameScreen::touchBegan(const TouchWay & touchWay)
{
	auto touchedAnything = false;

	auto touchedGUI = Screen::touchBegan(touchWay);
	touchedAnything = touchedGUI;

	if (!touchedGUI)
	{
		touchedAnything |= state->touchBegan(touchWay);
	}

	return touchedAnything;
};

void GameScreen::touchMoved(const TouchWay & touchWay)
{
	state->touchMoved(touchWay);
	Screen::touchMoved(touchWay);
};

bool GameScreen::mouseMove(MouseEvent event)
{
	return (state->mouseMove(event) || Screen::mouseMove(event));
}

void GameScreen::touchEnded(TouchWay touchWay)
{
	state->touchEnded(touchWay);
	Screen::touchEnded(touchWay);
};

bool GameScreen::touchClick(TouchWay touchWay)
{
	auto touchedAnything = false;

	auto touchedGUI = Screen::touchClick(touchWay);
	touchedAnything = touchedGUI;

	if (!touchedGUI)
	{
		touchedAnything |= state->touchClick(touchWay);
	}

	return touchedAnything;
};

void GameScreen::resize(ResizeEvent event)
{
	state->resize(event);
	RenderManager::getInstance()->cam.setAspectRatio(getWindowAspectRatio());
}

void GameScreen::onKeyInput(KeyEvent& e)
{
	state->onKeyInput(e);

	for (auto it = gameObjectsToUpdate.begin(); it != gameObjectsToUpdate.end(); ++it)
	{
		auto standardCell = dynamic_cast<StandardCellClient *>(it->second);
		
		if (standardCell)
		{
			standardCell->startAttackAnimation();
		}
	}
}

void GameScreen::mouseWheel(MouseEvent & e)
{
	state->mouseWheel(e);
}

void GameScreen::addGameObjectToUpdate(GameObjectClient * gameObject)
{
	gameObjectsToUpdate.createGameObject(gameObject);
}

void GameScreen::removeGameObjectToUpdate(GameObjectClient * gameObject)
{
	gameObjectsToUpdate.removeGameObject(gameObject->getId(), false);
}

void GameScreen::addGameObjectToDraw(GameObjectClient * gameObject)
{
	addGameObjectToUpdate(gameObject);

	gameObjectsToDraw.createGameObject(gameObject);
}

void GameScreen::removeGameObjectToDraw(GameObjectClient * gameObject)
{
	gameObjectsToDraw.removeGameObject(gameObject->getId(), false);
}

void GameScreen::addGameObjectToCollide(GameObject * gameObject)
{
	gameObjectsToCollide.createGameObject(gameObject);
}

void GameScreen::removeGameObjectToCollide(GameObject * gameObject)
{
	gameObjectsToCollide.removeGameObject(gameObject->getId(), false);
}

void GameScreen::addCellToPick(CellClient * cell)
{
	cellsToPick.createGameObject(cell);
}

void GameScreen::removeCellToPick(CellClient * gameObject)
{
	cellsToPick.removeGameObject(gameObject->getId(), false);
}

void GameScreen::addIncompleteCell(CellClient * cell)
{
	cellsIncomplete.createGameObject(cell);

	addGameObjectToCollide(cell);
}

void GameScreen::removeIncompleteCell(CellClient * cell)
{
	cellsIncomplete.removeGameObject(cell->getId(), false);

	removeGameObjectToCollide(cell);
}


void GameScreen::addIncompleteCell(
	unsigned int playerId, 
	CellType::Type type, 
	unsigned int cellId, 
	Vec3f position, 
	float angle
)
{
	auto player = GAME_MGR->getPlayerById(playerId);

	CellClient * cell;

	switch (type)
	{
	case CellType::StandardCell:
	default:
		{
			cell = new StandardCellClient(cellId, position, angle, player);
		}
		break;
	}

	cell->setHue(GAME_MGR->getHueByPlayerId(playerId));

	addIncompleteCell(cell);
}

void GameScreen::completeCellById(unsigned int id)
{
	auto cell = cellsIncomplete.find(id);

	if (cell != nullptr)
	{
		removeIncompleteCell(cell);
		addGameObjectToDraw(cell);
		
		auto cellOwner= cell->getOwner();
		auto myPlayer = GAME_MGR->getMyPlayer();

		if (cellOwner == myPlayer)
		{
			addCellToPick(cell);
			LOG_INFO("Own cell was completed.");
		}
		else
		{
			LOG_INFO("Other player's cell was completed.");
		}
	}
	else
	{
		LOG_WARNING("Could not find cell to be completed in GameScreen::completeCellById(unsigned int id).");
	}
}

void GameScreen::addCellPreview(CellClient * cell)
{
	cellPreviews.insert(cell);
}

void GameScreen::removeCellPreview(CellClient * cell)
{
	cellPreviews.erase(cell);
}

vector<CellClient *> GameScreen::getCellsPicked(Vec2f position)
{
	auto pointInWorldPlane = RenderManager::getInstance()->cam.screenToWorldPlane(position);
	
	return cellsToPick.pick(pointInWorldPlane);
}

void GameScreen::switchToState(GameScreenState * newState)
{
	auto oldState = state;
	state = newState;
	if (oldState != nullptr)
	{
		delete oldState;
	}
}

GameObjectContainer<GameObjectClient> & GameScreen::getGameObjectsToDraw()
{
	return gameObjectsToDraw;
}

ci::Vec2f GameScreen::worldToScreen(ci::Vec3f position)
{
	return RenderManager::getInstance()->cam.worldToScreen(position, (float) getWindowWidth(), (float) getWindowHeight());
}

void GameScreen::addRenderText(RenderText const & text)
{
	textList.push_back(text);
}

boost::mutex & GameScreen::getContainerMutex()
{
	return containerMutex;
}