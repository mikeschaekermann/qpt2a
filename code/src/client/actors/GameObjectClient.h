#pragma once

#include "../../common/GameObject.h"
#include <cmath>

class GameObjectClient :
	virtual public GameObject
{
public:
	GameObjectClient():
		hue(0.0f),
		saturation(0.7f),
		brightness(0.9f),
		opacity(0.7f),
		visible(false)
	{}
	
	virtual void draw() const;
	
	float getOpacity() const { return opacity; }
	void setOpacity(float newOpacity) { opacity = min<float>(max<float>(newOpacity, 0.0), 1.0); }
	float getHue() const { return hue; }
	void setHue(float newHue) { hue = min<float>(max<float>(newHue, 0.0), 1.0); }
	float getSaturation() const { return saturation; }
	void setSaturation(float newSaturation) { saturation = min<float>(max<float>(newSaturation, 0.0), 1.0); }
	float getBrightness() const { return brightness; }
	void setBrightness(float newBrightness) { brightness = min<float>(max<float>(newBrightness, 0.0), 1.0); }
	bool isVisible() const { return visible; }
	virtual void setVisibility(bool newVisible) { visible = newVisible; }
	void show() { setVisibility(true); }
	void hide() { setVisibility(false); }

	Vec2f getPosition2D() const;

protected:
	/// @brief here the pure draw process is defined, independent from the transformation
	virtual void drawAtTransformation() const;

	float	hue,
			saturation,
			brightness,
			opacity;

	bool	visible;
};