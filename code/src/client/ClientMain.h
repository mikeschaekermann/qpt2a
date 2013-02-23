/*
	Copyright (c) 2013 Daniel Margreiter, Daniel Satanik, Bernhard Öhlinger, Mike Schäkermann, Barbara Sterflinger, Sebastian Drack, Thomas Pichler, Philipp Födermayr, Wilfried Gruber
	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
	THIS PROJECT WAS CONCEIVED AND IMPLEMENTED BY
	STUDENTS OF THE UNIVERSITY OF APPLIED SCIENCES SALZBURG
	FROM THE DEPARTMENTS OF MULTIMEDIA TECHNOLOGY
	AND MULTIMEDIA ART AS A QUALIFICATIONAL SEMESTER PROJECT.
 */

#include "cinder/app/AppBasic.h"
#include "../common/GameObject.h"
#include "cinder/System.h"
#include "input/TouchWay.h"
class ClientNetworkManager;

using namespace ci;
using namespace ci::app;

class ClientMain : public AppBasic
{
 public:
	/// @brief initializes the app
	void	setup();
	/// @brief cleans up the app
	void	shutdown();

	/**
		@brief prepares the app for the specific environment (i.e. desktop, mobile, multi-touch table app etc.)
		@param settings			settings object that allows for settings to be modified
	 */
	void	prepareSettings( Settings *settings );

	/// @brief handles all updates needed for time-based events (e.g. animations etc.)
	void	update();
	
	/// @brief draws the current screen
	void	draw();

	/// input methods
	void	mouseDown( MouseEvent event );
	void	mouseDrag( MouseEvent event );
	void	mouseUp( MouseEvent event );
	void	mouseMove( MouseEvent event );
	void	mouseWheel( MouseEvent event );

	void	touchesBegan( TouchEvent event );
	void	touchesMoved( TouchEvent event );
	void	touchesEnded( TouchEvent event );

	void	keyDown( KeyEvent event );

	void	resize( ResizeEvent );

private:
	float m_fFrameTime;
	float m_fElapsedGameTimeLastFrame;
	map<uint32_t, TouchWay> m_touchWays;
};