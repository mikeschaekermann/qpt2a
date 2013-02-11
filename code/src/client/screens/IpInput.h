#pragma once
#include "textinput.h"

class IpInput :
	public TextInput
{
public:
	IpInput(Screen* screen, std::function<void()> callback, ci::Vec2f position = ci::Vec2f::zero(), const ci::gl::Texture * texture = nullptr, const ci::gl::Texture * hoverTexture = nullptr, const ci::gl::Texture * clickTexture = nullptr);
	~IpInput(void);

	virtual void onKeyInput(KeyEvent& e);
};

