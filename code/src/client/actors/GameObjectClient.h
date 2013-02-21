#pragma once

#include "../../common/GameObject.h"
#include <cmath>

class GameObjectClient :
	virtual public GameObject
{
public:
	GameObjectClient():
		standardHue(0.f),
		hue(0.f),
		standardSaturation(0.7f),
		saturation(0.7f),
		standardBrightness(0.8f),
		brightness(0.8f),
		standardOpacity(0.7f),
		opacity(0.7f),
		visible(false)
	{}
	
	virtual void draw() const;
	
	Vec2f getPosition2D() const;

	float getOpacity() const;
	void setOpacity(float newOpacity);

	float getHue() const;
	void setHue(float newHue);

	float getSaturation() const;
	void setSaturation(float newSaturation);

	float getBrightness() const;
	void setBrightness(float newBrightness);

	virtual void resetColor();
	virtual void setHoverColor();
	virtual void setMouseDownColor();

	bool isVisible() const;
	virtual void setVisibility(bool newVisible);
	void show();
	void hide();

protected:
	/// @brief here the pure draw process is defined, independent from the transformation
	virtual void drawAtTransformation() const;

	float	hue,
			saturation,
			brightness,
			opacity,
			standardHue,
			standardSaturation,
			standardBrightness,
			standardOpacity;

	bool	visible;
};