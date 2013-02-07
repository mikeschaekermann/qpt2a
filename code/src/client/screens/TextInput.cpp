#include "TextInput.h"


TextInput::TextInput(Screen* screen, std::function<void()> callback, ci::Vec2f position, const ci::gl::Texture * texture, const ci::gl::Texture * hoverTexture, const ci::gl::Texture * clickTexture) :
	GUIItem(screen,
			callback,
			position,
			texture,
			hoverTexture,
			clickTexture
	)
{
}


TextInput::~TextInput(void)
{
}

void TextInput::onKeyInput(KeyEvent& e)
{
	if(isdigit(e.getChar()) || e.getChar() == '.')
	{
		text += e.getChar();
	}

	GUIItem::onKeyInput(e);
}