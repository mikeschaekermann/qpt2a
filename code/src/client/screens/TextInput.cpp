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

void TextInput::draw()
{
	GUIItem::draw();
	ci::gl::drawString(text, this->position + Vec2f(20, 25), ci::ColorA::black());
}

void TextInput::onKeyInput(KeyEvent& e)
{
	if(e.getCode() == KeyEvent::KEY_BACKSPACE && text.length() > 0)
	{
		text = text.substr(0, text.length() - 1);
	}
	else
	{
		text += e.getChar();
	}

	GUIItem::onKeyInput(e);
}