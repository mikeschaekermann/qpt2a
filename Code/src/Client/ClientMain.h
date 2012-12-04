#include "cinder/app/AppBasic.h"
#include "../common/GameObject.h"
#include "cinder/System.h"

using namespace ci;
using namespace ci::app;

class ClientMain : public AppBasic
{
 public:
	/**
		@brief prepares the app for the specific environment (i.e. desktop, mobile, multi-touch table app etc.)
		@param settings			settings object that allows for settings to be modified
	 */
	void setup();

	void	prepareSettings( Settings *settings );

	/// @brief handles all updates needed for time-based events (e.g. animations etc.)
	void	update();
	
	/// @brief draws the current screen
	void	draw();

	/// input methods
	void	mouseDown( MouseEvent event );
	void	mouseDrag( MouseEvent event );

	void	touchesBegan( TouchEvent event );
	void	touchesMoved( TouchEvent event );
	void	touchesEnded( TouchEvent event );

	void	keyDown( KeyEvent event );

private:
	float m_fFrameTime;
	float m_fElapsedGameTimeLastFrame;
};