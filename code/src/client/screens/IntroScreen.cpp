#include "IntroScreen.h"
#include "../managers/ScreenManager.h"
#include "../managers/AssetManager.h"

IntroScreen::IntroScreen(void) :
	isRunning(true)
{
	movie = ASSET_MGR->getMovie(string("intro"));
	movie.play();
}

IntroScreen::~IntroScreen(void)
{

}

void IntroScreen::update(float frameTime)
{

}

void IntroScreen::draw()
{
	if(isRunning)
	{
		gl::draw(movie.getTexture(), getWindowBounds());
		
		if(movie.isDone())
		{
			isRunning = false;
		}
	}
	else
	{
		SCREEN_MGR->openMenuScreen();
	}
}

void IntroScreen::resize()
{

}
