#include "IpInput.h"


IpInput::IpInput(Screen* screen, std::function<void()> callback, ci::Vec2f position, const ci::gl::Texture * texture, const ci::gl::Texture * hoverTexture, const ci::gl::Texture * clickTexture) :
	TextInput(screen,
			  callback,
			  position,
			  texture,
			  hoverTexture,
			  clickTexture
	)
{
}

IpInput::~IpInput(void)
{
}

void IpInput::onKeyInput(KeyEvent& e)
{
	if(isdigit((unsigned char)e.getChar()) || e.getChar() == '.' || e.getCode() == KeyEvent::KEY_BACKSPACE)
	{
		TextInput::onKeyInput(e);
	}
}