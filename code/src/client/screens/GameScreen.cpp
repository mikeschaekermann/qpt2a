#include "GameScreen.h"
#include "../../common/network/NetworkManager.h"
#include "../managers/AssetManager.h"
#include "../actors/CellClient.h"
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
}

void GameScreen::draw()
{
	gl::pushMatrices();
	{
		gl::enableDepthWrite();
		gl::enableDepthRead();

		gl::setMatrices(cam);

		for (auto it = gameObjectsToDraw.begin(); it != gameObjectsToDraw.end(); ++it)
		{
			it->second->draw();
		}

		state->draw3D();

		gl::color(ColorA(1, 1, 1, 1));

		gl::disableDepthWrite();
		gl::disableDepthRead();
	}
	
	
		////////////// render test
		gl::color(1,1,1,1);
		gl::pushMatrices();

		gl::translate(100, -100, 20);
		gl::scale(100, 100, 100);
		string name("stemcell");
		auto model = ASSET_MGR->getModel(name);
		auto shader = ASSET_MGR->getShaderProg(string("test"));
	
		shader.bind();

		shader.uniform("viewPos", cam.getEyePoint());
		shader.uniform("lightPos", Vec3f(0, 0, 100));

		shader.uniform("matModelView", cam.getModelViewMatrix());
		shader.uniform("matProjection", cam.getProjectionMatrix());
	
		shader.uniform("ambientColor", Vec3f(0.1, 0.1, 0.1));
		shader.uniform("diffuseColor", Vec3f(0.1, 0.1, 0.1));
		shader.uniform("specularColor", Vec3f(0.1, 0.1, 0.1));
		shader.uniform("shininess", 2);

		gl::draw(model);
		shader.unbind();

		gl::popMatrices();

	gl::popMatrices();

	Screen::draw();

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
	cam.setEyePoint(cam.getEyePoint() + Vec3f(-touchWay.getLastDeltaVector().x, touchWay.getLastDeltaVector().y, 0));
	return state->touchMoved(touchWay);
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

void GameScreen::touchClick(TouchWay touchWay)
{
	state->touchClick(touchWay);
};

void GameScreen::resize(ResizeEvent event)
{
	state->resize(event);
	cam.setAspectRatio(getWindowAspectRatio());
}

void GameScreen::onKeyInput(KeyEvent& e)
{
	if (e.getCode() == KeyEvent::KEY_ESCAPE)
	{
		switchToState(new GameScreenStateNeutral(this));
	}
	else if(e.getCode() == KeyEvent::KEY_LEFT)
	{
		cam.setEyePoint(cam.getEyePoint() + Vec3f(10.f, 0.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_RIGHT)
	{
		cam.setEyePoint(cam.getEyePoint() + Vec3f(-10.f, 0.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_UP)
	{
		cam.setEyePoint(cam.getEyePoint() + Vec3f(0.f, -10.f, 0.f));
	}
	else if(e.getCode() == KeyEvent::KEY_DOWN)
	{
		cam.setEyePoint(cam.getEyePoint() + Vec3f(0.f, 10.f, 0.f));
	}

	state->onKeyInput(e);
}

void GameScreen::mouseWheel(MouseEvent & e)
{
	cam.setEyePoint(cam.getEyePoint() + Vec3f(0.f, 0.f, -e.getWheelIncrement() * 100.f));
}

void GameScreen::addGameObjectToUpdate(GameObjectClient * gameObject, bool collidable)
{
	if (collidable)
	{
		gameObjectsToCollide.createGameObject(gameObject);
	}

	gameObjectsToUpdate.createGameObject(gameObject);
}

void GameScreen::addGameObjectToDraw(GameObjectClient * gameObject, bool collidable)
{
	addGameObjectToUpdate(gameObject, collidable);

	gameObjectsToDraw.createGameObject(gameObject);
}

void GameScreen::addCellToPick(CellClient * cell, bool collidable)
{
	addGameObjectToDraw(cell, collidable);

	cellsToPick.createGameObject(cell);
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

void GameScreen::pickCell(CellClient * cell)
{
	if (cell != nullptr)
	{
		pickedCell = cell;

		auto menuPosition3D = pickedCell->getPosition() + Vec3f(pickedCell->getRadius() + 5, 0, 0);
		auto menuPosition2D = cam.worldToScreen(menuPosition3D, getWindowWidth(), getWindowHeight());
		cellMenu->setPosition(menuPosition2D);
		cellMenu->setVisible(true);
	}
}

void GameScreen::unpickCell()
{
	pickedCell = nullptr;
	cellMenu->setVisible(false);
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