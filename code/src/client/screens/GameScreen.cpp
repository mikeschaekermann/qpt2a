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

GameScreen::GameScreen():
	state(new GameScreenStateNeutral(this))
{
	auto screenSize = getWindowSize();
	cam = Cam(screenSize.x, screenSize.y, CONFIG_FLOAT1("data.rendering.camera.fieldOfView"), CONFIG_FLOAT1("data.rendering.camera.nearPlane"), CONFIG_FLOAT1("data.rendering.camera.farPlane"));

	cam
		.setPosition(Vec3f( 0.0f, 0.0f, 0.0f ))
		.setFocus(Vec3f::zero());

	auto createCellButton = &(ASSET_MGR->getGuiTexture(string("createCell")));
	auto createStandardCellButton = &(ASSET_MGR->getGuiTexture(string("createStandardCell")));
	auto createStandardCellHoverButton = &(ASSET_MGR->getGuiTexture(string("createStandardCellHover")));
	auto createStandardCellClickButton = &(ASSET_MGR->getGuiTexture(string("createStandardCellClick")));

	cellMenu = rootItem->addSubItem(this, nullptr);

	cellMenu
		->addSubItem(
			this,
			[](){},
			Vec2f::zero(),
			createCellButton,
			createCellButton,
			createCellButton
		)
		->addSubItem(
			this,
			[this]()
			{
				switchToState(new GameScreenStateCreateCell(this, CellType::StandardCell));
			},
			Vec2f(45, -10),
			createStandardCellButton,
			createStandardCellHoverButton,
			createStandardCellClickButton
		);

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
}

void GameScreen::draw()
{
	gl::pushMatrices();
	{
		gl::enableDepthWrite();
		gl::enableDepthRead();

		gl::setMatrices(cam);

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
		containerMutex.lock();

		for (auto it = textList.begin(); it != textList.end(); ++it)
		{
			gl::drawStringCentered(it->getText(), it->getPos(), ColorA(1, 1, 1, 1), Font("Comic Sans MS", 18));
		}

		containerMutex.unlock();

		state->draw3D();
	}
	gl::popMatrices();

	gl::color(ColorA(1, 1, 1, 1));
	rootItem->draw();

	///////////// 2D rendering

	state->draw2D();
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
	cam.setAspectRatio(getWindowAspectRatio());
}

void GameScreen::onKeyInput(KeyEvent& e)
{
	state->onKeyInput(e);
}

void GameScreen::mouseWheel(MouseEvent & e)
{
	state->mouseWheel(e);
}

void GameScreen::addGameObjectToUpdate(GameObjectClient * gameObject)
{
	containerMutex.lock();
	gameObjectsToUpdate.createGameObject(gameObject);
	containerMutex.unlock();
}

void GameScreen::addGameObjectToDraw(GameObjectClient * gameObject)
{
	addGameObjectToUpdate(gameObject);

	containerMutex.lock();
	gameObjectsToDraw.createGameObject(gameObject);
	containerMutex.unlock();
}

void GameScreen::addGameObjectToCollide(GameObject * gameObject)
{
	containerMutex.lock();
	gameObjectsToCollide.createGameObject(gameObject);
	containerMutex.unlock();
}

void GameScreen::removeGameObjectToCollide(GameObject * gameObject)
{
	containerMutex.lock();
	gameObjectsToCollide.removeGameObject(gameObject->getId(), false);
	containerMutex.unlock();
}

void GameScreen::addCellToPick(CellClient * cell)
{
	containerMutex.lock();
	cellsToPick.createGameObject(cell);
	containerMutex.unlock();
}

void GameScreen::addIncompleteCell(CellClient * cell)
{
	containerMutex.lock();
	cellsIncomplete.createGameObject(cell);
	containerMutex.unlock();

	addGameObjectToCollide(cell);
}

void GameScreen::removeIncompleteCell(CellClient * cell)
{
	containerMutex.lock();
	cellsIncomplete.removeGameObject(cell->getId(), false);
	containerMutex.unlock();

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
	case CellType::Type::StandardCell:
	default:
	{
		cell = new StandardCellClient(cellId, position, angle, player);
	}
	break;
	}

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
	containerMutex.lock();
	cellPreviews.insert(cell);
	containerMutex.unlock();
}

void GameScreen::removeCellPreview(CellClient * cell)
{
	containerMutex.lock();
	cellPreviews.erase(cell);
	containerMutex.unlock();
}

void GameScreen::zoomToWorld()
{
	/// world radius = sin(camera's fov / 2) * camera's distance
	/// camera's distance = world radius / sin(camera's fov / 2)

	float camDistance = worldRadius / sin(cam.getFov() / 2 / 180.0 * M_PI);

	cam
		.setPosition(Vec3f(0, 0, camDistance))
		.setFocus(Vec3f::zero());
}

vector<CellClient *> GameScreen::getCellsPicked(Vec2f position)
{
	auto pointInWorldPlane = cam.screenToWorldPlane(position);
	
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

void GameScreen::renderModel(string modelName, 
							 string shaderName, 
							 Vec3f lightPos, 
							 Vec3f ambient, 
							 Vec3f diffuse, 
							 Vec3f specular, 
							 float shininess)
{
	gl::pushMatrices();

	auto model = ASSET_MGR->getModel(modelName);
	auto shader = ASSET_MGR->getShaderProg(shaderName);

	shader.bind();

	shader.uniform("lightPos", cam.getProjectionMatrix() * cam.getModelViewMatrix() * lightPos);

	shader.uniform("ambientColor", ambient);
	shader.uniform("diffuseColor", diffuse);
	shader.uniform("specularColor", specular);
	shader.uniform("shininess", shininess);

	gl::pushModelView();
		gl::draw(model);
	gl::popModelView();

	shader.unbind();

	gl::popMatrices();
}

GameObjectContainer<GameObjectClient> & GameScreen::getGameObjectsToDraw()
{
	return gameObjectsToDraw;
}

ci::Vec2f GameScreen::worldToScreen(ci::Vec3f position)
{
	return this->cam.worldToScreen(position, getWindowWidth(), getWindowHeight());
}

void GameScreen::addRenderText(RenderText const & text)
{
	textList.push_back(text);
}