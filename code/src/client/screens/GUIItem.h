#pragma once

#include <vector>
#include <cinder/Vector.h>
#include <cinder/gl/Texture.h>

#include "Screen.h"

class Screen;
class GUIItem
{
public:
	GUIItem(Screen* screen, std::function<void()> callback, ci::Vec2f position = ci::Vec2f::zero(), const ci::gl::Texture * texture = nullptr, const ci::gl::Texture * hoverTexture = nullptr, const ci::gl::Texture * clickTexture = nullptr);
	~GUIItem();
	GUIItem * addSubItem(Screen* screen, std::function<void()> callback, ci::Vec2f position = ci::Vec2f::zero(), const ci::gl::Texture * texture = nullptr, const ci::gl::Texture * hoverTexture = nullptr, const ci::gl::Texture * clickTexture = nullptr);
	GUIItem* addSubItem(GUIItem* item);
	GUIItem * parent();

	virtual bool isMouseOverItem(ci::Vec2f position);
	virtual bool isMouseDownOnItem(ci::Vec2f position);
	virtual void isMouseUp();
	virtual bool hasMouseClickedOnItem(ci::Vec2f position);
	virtual void onKeyInput(KeyEvent& e);

	void setPosition(ci::Vec2f newPosition);
	void setVisible(bool visible, bool recursively = true);

	virtual string const & getValue() { return defaultValue; }

	virtual void draw();

	void setEnabled(bool enabled);

	bool isEnabled();
protected:
	bool isPositionInItem(ci::Vec2f position);

	std::function<void()> callback;

	ci::Vec2f position;

	const ci::gl::Texture * currentTexture;
	const ci::gl::Texture * const texture;
	const ci::gl::Texture * const hoverTexture;
	const ci::gl::Texture * const clickTexture;

	bool isVisible;
	bool hasFocus;

	GUIItem * parentItem;
	std::vector<GUIItem*> subItems;

	Screen* screen;

	bool enabled;
private:
	static std::string defaultValue;
};