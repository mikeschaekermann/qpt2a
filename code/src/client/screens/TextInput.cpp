#include "TextInput.h"
#include <ostream>
#include <sstream>


TextInput::TextInput(Screen* screen, std::function<void()> callback, ci::Vec2f position, const ci::gl::Texture * texture, const ci::gl::Texture * hoverTexture, const ci::gl::Texture * clickTexture) :
	GUIItem(screen,
			callback,
			position,
			texture,
			hoverTexture,
			clickTexture
	)
{
	this->text = static_cast<ostringstream*>(&(ostringstream() <<  rand()))->str();
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
	else if(isalnum((unsigned char)e.getChar()) || e.getChar() == '.')
	{
		text += e.getChar();
	}

	GUIItem::onKeyInput(e);
}