#include "GameScreen.h"
#include "../managers/GameManager.h"
#include "../../common/network/NetworkManager.h"
#include "../../common/network/messages/game/ingame/polypeptide/creation/CreatePolypeptideRequest.h"
#include "../managers/AssetManager.h"
#include "../actors/CellClient.h"
#include "../actors/StandardCellClient.h"
#include "../actors/BoneCellClient.h"
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
			switchToState(new GameScreenStateCreateCell(this, CellType::BoneCell));
		},
		Vec2f::zero(),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-knochen-normal"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-knochen-hover"))),
		&(ASSET_MGR->getGuiTexture(string("ingame-button-knochen-clicked")))
	)));

	cellMenuButtons.insert(make_pair("polypeptid", cellMenu->addSubItem(
		this,
		[this]()
		{
			LOG_INFO("Create attacker polypeptide button was clicked!");
			CreatePolypeptideRequest * request = new CreatePolypeptideRequest();
			request->endpoint = GAME_MGR->getServerEndpoint();
			request->playerId = GAME_MGR->getMyPlayer()->getId();
			NETWORK_MGR->registerCreatePolypeptideRequest(request);
			NETWORK_MGR->send(request);
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
	SOUND_PLAYER->setListener3d(Vec3f(RENDER_MGR->cam.getEyePoint().xy(), 10), Vec3f::zero(), Vec3f::zero(), RENDER_MGR->cam.getWorldUp());

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
		RenderManager::getInstance()->renderPhongShadedModel("petriDish",
												  Vec4f(0.1f, 0.1f, 0.1f, 0.1f),
												  Vec4f(0.1f, 0.1f, 0.1f, 0.2f),
												  Vec4f(1.f, 1.f, 1.f, 0.4f),
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

	/// draw player skins
	for (auto it = GAME_MGR->getPlayers().begin(); it != GAME_MGR->getPlayers().end(); ++it)
	{
		it->second->drawSkin();
	}

	state->draw3D();

	RenderManager::getInstance()->shutdown3d();

	///////////// 2D rendering

	state->draw2D();

	containerMutex.lock();

	/// draw health bars
	for (auto it = gameObjectsToDraw.begin(); it != gameObjectsToDraw.end(); ++it)
	{
		auto cell = dynamic_cast<CellClient *>(it->second);

		if (cell != nullptr)
		{
			cell->drawHealthBar();
		}
	}

	containerMutex.unlock();

	containerMutex.lock();

	for (auto it = textList.begin(); it != textList.end(); ++it)
	{
		drawString(it->getText(), worldToScreen(it->getPos()), ColorA(1.f, 0.f, 0.f, 1.f), 
			Font(CONFIG_STRING2("data.ingamefeedback.renderedDamage.font", "Comic Sans MS"), (float) CONFIG_INT2("data.ingamefeedback.renderedDamage.size", 18)));
	}

	containerMutex.unlock();

	/// DRAW CELL IDs FOR DEBUGGING
	if (GAME_MGR->isInDebugMode())
	{
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

	gl::color(ColorA(1, 1, 1, 1));
	rootItem->draw();
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

	if (e.getCode() == KeyEvent::KEY_d)
	{
		GAME_MGR->toggleDebugMode();
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
	updateVisibilityOf(gameObject);
}

void GameScreen::removeGameObjectToDraw(GameObjectClient * gameObject)
{
	gameObjectsToDraw.removeGameObject(gameObject->getId(), false);
	
	auto cell = dynamic_cast<CellClient *>(gameObject);

	if(cell != nullptr && cell->isVisible())
	{
		auto owner = GAME_MGR->getPlayerById(cell->getOwner()->getId());

		if (owner != nullptr)
		{
			owner->removeSkinBubble(Sphere(cell->getPosition(), cell->getRadius() + CONFIG_FLOAT2("data.skin.distanceFromCells", 10.f)));
		}
	}
	
	updateVisibilityOf(gameObject);
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

void GameScreen::removeCellToPick(CellClient * cell)
{
	cellsToPick.removeGameObject(cell->getId(), false);

	if (cell == pickedCell)
	{
		unpickCells();
	}
}

void GameScreen::pickCell(CellClient * cell)
{
	if (cell != nullptr)
	{
		pickedCell = cell;
	}
}

void GameScreen::unpickCells()
{
	pickedCell = nullptr;
}

void GameScreen::addIncompleteCell(CellClient * cell)
{
	cellsIncomplete.createGameObject(cell);
	updateVisibilityOf(cell);

	addGameObjectToCollide(cell);
}

void GameScreen::removeIncompleteCell(CellClient * cell)
{
	cellsIncomplete.removeGameObject(cell->getId(), false);
	updateVisibilityOf(cell);

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
	case CellType::BoneCell:
		{
			cell = new BoneCellClient(cellId, position, angle, player);
			break;
		}
	}

	cell->setOpacity(CONFIG_FLOAT2("data.ingamefeedback.building.incompleteOpacity", 0.5f));
	cell->setHue(GAME_MGR->getHueByPlayerId(playerId));

	addIncompleteCell(cell);
}

void GameScreen::completeCellById(unsigned int id)
{
	auto cell = cellsIncomplete.find(id);

	if (cell != nullptr)
	{
		cell->setOpacity(CONFIG_FLOAT2("data.ingamefeedback.building.completeOpacity", 1.0f));

		removeIncompleteCell(cell);
		addGameObjectToDraw(cell);
		addGameObjectToCollide(cell);
		
		auto cellOwner= cell->getOwner();
		auto myPlayer = GAME_MGR->getMyPlayer();

		if (cellOwner == myPlayer)
		{
			addCellToPick(cell);
			LOG_INFO("Own cell was completed.");
			SOUND_PLAYER->playSound(string("cellSuccess"), cell->getPosition(), Vec3f::zero());
		}
		else
		{
			LOG_INFO("Other player's cell was completed.");
			if(cell->isVisible())
			{
				SOUND_PLAYER->playSound(string("cellNew"), cell->getPosition(), Vec3f::zero());
			}
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

void GameScreen::addExploringCell(CellClient * cell)
{
	cellsExploring.createGameObject(cell);
	
	auto area = new CellClient();
	area->setId(cell->getId());
	area->setPosition(cell->getPosition());
	area->setRadius(cell->getRadius() + CONFIG_FLOAT2("data.world.fogOfWar.innerRadius", 20) + CONFIG_FLOAT2("data.world.fogOfWar.outerRadius", 20));
	
	visibleAreas.createGameObject(area);

	updateVisibleGameObjects();
}

void GameScreen::removeExploringCell(CellClient * cell)
{
	cellsExploring.removeGameObject(cell->getId(), false);
	visibleAreas.removeGameObject(cell->getId(), false);

	updateVisibleGameObjects();
}

GameObjectContainer<GameObject> & GameScreen::getExploringCells()
{
	return cellsExploring;
}

vector<CellClient *> GameScreen::getCellsPicked(Vec2f position)
{
	auto pointInWorldPlane = RenderManager::getInstance()->cam.screenToWorldPlane(position);
	
	return cellsToPick.pick(pointInWorldPlane);
}

bool GameScreen::canCellBePlaced(CellClient * cell)
{
	auto collidingObjects = gameObjectsToCollide.findInRadiusOf(cell->getPosition(), cell->getRadius());

	if (collidingObjects.size() == 0)
	{
		return true;
	}

	return false;
}

void GameScreen::switchToState(GameScreenState * newState)
{
	switchScreenMutex.lock();
	auto oldState = state;
	state = newState;
	if (oldState != nullptr)
	{
		delete oldState;
	}
	switchScreenMutex.unlock();
}

GameObjectContainer<GameObjectClient> & GameScreen::getGameObjectsToDraw()
{
	return gameObjectsToDraw;
}

IdGameObjectMap<PolypeptideClient> & GameScreen::getMyPolypeptides()
{
	return myPolypeptides;
}

ci::Vec2f GameScreen::worldToScreen(ci::Vec3f position)
{
	return RenderManager::getInstance()->cam.worldToScreen(position, (float) getWindowWidth(), (float) getWindowHeight());
}

void GameScreen::addRenderText(RenderText const & text)
{
	textList.push_back(text);
}

void GameScreen::updateVisibilityOf(GameObjectClient * gameObject)
{
	auto intersections = visibleAreas.findInRadiusOf(gameObject->getPosition(), gameObject->getRadius());
	auto newVisible = intersections.size() > 0;

	auto cell = dynamic_cast<CellClient *>(gameObject);

	if (cell != nullptr && cell->isVisible() != newVisible)
	{
		auto owner = GAME_MGR->getPlayerById(cell->getOwner()->getId());

		if (owner != nullptr)
		{
			if (newVisible)
			{
				owner->addSkinBubble(Sphere(cell->getPosition(), cell->getRadius() + CONFIG_FLOAT2("data.skin.distanceFromCells", 10.f)));
			}
			else
			{
				owner->removeSkinBubble(Sphere(cell->getPosition(), cell->getRadius() + CONFIG_FLOAT2("data.skin.distanceFromCells", 10.f)));
			}
		}
	}

	gameObject->setVisibility(newVisible);
}

void GameScreen::updateVisibleGameObjects()
{
	for (auto it = gameObjectsToDraw.begin(); it != gameObjectsToDraw.end(); ++it)
	{
		updateVisibilityOf(it->second);
	}
	
	for (auto it = cellsIncomplete.begin(); it != cellsIncomplete.end(); ++it)
	{
		updateVisibilityOf(it->second);
	}
}

boost::mutex & GameScreen::getContainerMutex()
{
	return containerMutex;
}