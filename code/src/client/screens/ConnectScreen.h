#pragma once

#include "../rendering/Cam.h"
#include "../../common/Config.h"
#include "../actors/GameObjectClient.h"
#include "../../common/Player.h"
#include "../../common/network/messages/enum/JoinErrorCode.h"

#include "Screen.h"
class TextInput;


class ConnectScreen :
	public Screen
{
public:
	ConnectScreen();
	virtual ~ConnectScreen(void);

	virtual void update(float frameTime);
	virtual void draw();

	/**
		@brief event method called when a new touch has begun
		@param touchWay			touch way of the newly begun touch
	 */
	virtual bool touchBegan(const TouchWay & touchWay);
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
	
	virtual void resize(ResizeEvent event);

	void joinFailure(JoinErrorCode error);

	void joinSuccess();

private:
	GUIItem * backButton;
	GUIItem * connectButton;
	TextInput * messageBox;

	float connectTimeout;
};