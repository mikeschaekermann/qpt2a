#pragma once

#include "Screen.h"
#include "../common/Config.h"
#include "../common/GameObject.h"
#include "../common/Player.h"
#include "cinder/Vector.h"
#include "Cam.h"

class GameScreen :
	public Screen
{
public:
	GameScreen(ScreenManager& screenManager);
	virtual ~GameScreen(void);

	virtual void update(float frameTime);
	virtual void draw();

	virtual void loadContent();

	/**
		@brief event method called when a new touch has begun
		@param touchWay			touch way of the newly begun touch
	 */
	virtual void touchBegan(const TouchWay & touchWay);
	/**
		@brief event method called when an existing touch has moved
		@param touchWay			touch way of the touch, concerned
	 */
	virtual void touchMoved(const TouchWay & touchWay);
	/**
		@brief event method called when an existing touch has ended
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	virtual void touchEnded(TouchWay touchWay);
	/**
		@brief event method called when an touch was released as a click (i.e. no drag!)
		@param touchWay			touch way of the touch, concerned; must be called by copy,
								because touch way object will be destroyed after call to this method
	 */
	virtual void touchClick(TouchWay touchWay);

private:
	/// scene object
	GameObject* m_pScene;

	/// all active players
	vector<Player*> m_players;

	/// perspective cam for the game
	Cam cam;
};

