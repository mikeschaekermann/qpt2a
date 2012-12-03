#include "cinder\app\AppBasic.h"
#include "../common/GameObject.h"

using namespace ci;
using namespace ci::app;

class ClientMain : public AppBasic
{
 public:
	/**
		@brief initializes video capturing
	 */
	void setup();
	/**
		@brief retrieves the image data and computes the mask
	 */
	void update();
	/**
		@brief draws the original and the mask images to the screen
	 */
	void draw();
};