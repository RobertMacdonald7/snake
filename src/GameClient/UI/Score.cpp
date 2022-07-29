#include "Score.h"

#include "../GameDefinitions.h"

GameClient::UI::Score::Score()
{
	_scoreLabel = std::make_unique<Controls::Label>();

	_scoreLabel->text = L"0";
	_scoreLabel->size = { game_width_pixels - 3 * unit_size_pixels, 50 };
	_scoreLabel->location = { unit_size_pixels * 3 / 2, 0 };
	_scoreLabel->alignment = Engine::TextAlignment::TopRight;
	_scoreLabel->colour = Engine::Colour::BlackAlpha;
}

void GameClient::UI::Score::SetScore(const int score)
{
	_scoreLabel->text = std::to_wstring(score);
}

void GameClient::UI::Score::Draw(Engine::IRender& renderEngine)
{
	_scoreLabel->Draw(renderEngine);
}
