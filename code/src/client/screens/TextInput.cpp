#include "TextInput.h"
#include "../../common/ConfigurationDataHandler.h"
#include <ostream>
#include <sstream>

TextInput::TextInput(Screen* screen, std::function<void()> callback, ci::Vec2f position, const ci::gl::Texture * texture, const ci::gl::Texture * hoverTexture, const ci::gl::Texture * clickTexture) :
	GUIItem(screen,
			callback,
			position,
			texture,
			hoverTexture,
			clickTexture
	),
	renderBox(true)
{
}


TextInput::~TextInput(void)
{
}

void TextInput::draw()
{
	if (renderBox)
	{
		GUIItem::draw();
		ci::gl::drawString(text, this->position + Vec2f(20, 15), ci::ColorA::white(), cinder::Font(CONFIG_STRING2("data.input.text.font", "Arial"), CONFIG_FLOAT2("data.input.text.size", 34)));
	}
	else
	{
		ci::gl::drawString(text, this->position + Vec2f(20, 15), ci::ColorA(0.42f, 0.81f, 0.22f), cinder::Font(CONFIG_STRING2("data.input.text.font", "Arial"), CONFIG_FLOAT2("data.input.text.size", 34)));
	}
	
}

void TextInput::onKeyInput(KeyEvent& e)
{
	if (enabled)
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
}