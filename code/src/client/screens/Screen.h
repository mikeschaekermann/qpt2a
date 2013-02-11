#pragma once

#include "../managers/ScreenManager.h"
#include "../input/TouchWay.h"
#include "GUIItem.h"

class ScreenManager;
class GUIItem;
class Screen
{
public:
	GUIItem* focusedItem;

	Screen();
	virtual ~Screen(void);

	virtual void update(float frameTime);
	virtual void draw();

	virtual bool touchBegan(const TouchWay & touchWay);	
	virtual void touchMoved(const TouchWay & touchWay);
	virtual bool mouseMove( MouseEvent event );
	virtual void touchEnded(TouchWay touchWay);
	virtual bool touchClick(TouchWay touchWay);
	virtual void resize(ResizeEvent event);
	virtual void onKeyInput(KeyEvent& e);
	virtual void mouseWheel(MouseEvent & e);

protected:
	GUIItem* rootItem;
};