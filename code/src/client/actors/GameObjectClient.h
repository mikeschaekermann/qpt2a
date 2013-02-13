#pragma once

#include "../../common/GameObject.h"
#include <cmath>

class GameObjectClient :
	virtual public GameObject
{
public:
	GameObjectClient(float opacity=0.5): opacity(opacity) {};
	/// @brief transforms the object and draws it
	void draw() const;
	float getOpacity() const { return opacity; }
	float setOpacity(float newOpacity) { opacity = min<float>(max<float>(newOpacity, 0.0), 1.0); }

protected:
	/// @brief here the pure draw process is defined, independent from the transformation
	virtual void drawAtTransformation() const;

	float opacity;
};