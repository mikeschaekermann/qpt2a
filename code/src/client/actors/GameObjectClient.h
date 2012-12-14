#pragma once

#include "../../common/GameObject.h"

class GameObjectClient :
	virtual public GameObject
{
public:
	/// @brief transforms the object and draws it
	void draw() const;

protected:
	/// @brief here the pure draw process is defined, independent from the transformation
	virtual void drawAtTransformation() const;
};