#pragma once

#include <vector>
#include <cinder/Vector.h>
#include <cinder/gl/Texture.h>

class GUIItem
{
public:
	GUIItem(void (*callback)(), ci::Vec2f position = ci::Vec2f::zero(), const ci::gl::Texture & texture = ci::gl::Texture(), const ci::gl::Texture & hoverTexture = ci::gl::Texture(), const ci::gl::Texture & clickTexture = ci::gl::Texture());

	GUIItem * addSubItem(void (*callback)(), ci::Vec2f position, const ci::gl::Texture & texture, const ci::gl::Texture & hoverTexture, const ci::gl::Texture & clickTexture);
	GUIItem * parent();

	bool isMouseOverItem(ci::Vec2f position);
	bool isMouseDownOnItem(ci::Vec2f position);
	void isMouseUp();

	void setPosition(ci::Vec2f newPosition);
	void setVisible(bool visible, bool recursively = false);

	void draw();
protected:
	bool isPositionInItem(ci::Vec2f position);

	void (*callback)();

	ci::Vec2f position;

	const ci::gl::Texture * currentTexture;
	const ci::gl::Texture & texture;
	const ci::gl::Texture & hoverTexture;
	const ci::gl::Texture & clickTexture;

	bool isVisible;

	GUIItem * parentItem;
	std::vector<GUIItem> subItems;
};