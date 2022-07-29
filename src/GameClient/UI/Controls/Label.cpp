#include "Label.h"

void GameClient::UI::Controls::Label::Draw(Engine::IRender& renderEngine)
{
	if (border)
	{
		renderEngine.DrawRectangle(location, size, 1.0f, Engine::Colour::Black);
	}

	renderEngine.DrawString(text, location, size, colour, alignment);
}
