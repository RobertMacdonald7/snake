#include "Scoreboard.h"

#include "../GameDefinitions.h"

GameClient::UI::Scoreboard::Scoreboard()
{
	_scoreLabel = std::make_unique<Controls::Label>();
	_highScoreLabel = std::make_unique<Controls::Label>();
	_restartPromptLabel = std::make_unique<Controls::Label>();

	_scoreLabel->text = L"SCORE: 0";
	_scoreLabel->size = { game_width_pixels, 100 };
	_scoreLabel->location = {
		game_width_pixels / 2 - _scoreLabel->size.width / 2,
		game_height_pixels / 4 - _scoreLabel->size.height / 2
	};

	_highScoreLabel->text = L"HIGH SCORE: " + std::to_wstring(_highScore);
	_highScoreLabel->size = { game_width_pixels, 100 };
	_highScoreLabel->location = {
		_scoreLabel->location.x,
		game_height_pixels * 2 / 4 - _scoreLabel->size.height / 2 };

	_restartPromptLabel->text = L"Press space-bar to start a new game...";
	_restartPromptLabel->size = { game_width_pixels, 100 };
	_restartPromptLabel->location = {
		game_width_pixels / 2 - _restartPromptLabel->size.width / 2,
		game_height_pixels * 3 / 4 - _scoreLabel->size.height / 2 };
}

void GameClient::UI::Scoreboard::SetScore(const int score)
{
	if (score > _highScore)
	{
		_highScore = score;
		_highScoreLabel->text = L"HIGH SCORE: " + std::to_wstring(_highScore);
	}

	_scoreLabel->text = L"SCORE: " + std::to_wstring(score);
}

void GameClient::UI::Scoreboard::Draw(Engine::IRender& renderEngine)
{
	renderEngine.FillRectangle({ 0, 0 }, { game_width_pixels, game_height_pixels }, Engine::Colour::WhiteAlpha);
	_scoreLabel->Draw(renderEngine);
	_highScoreLabel->Draw(renderEngine);
	_restartPromptLabel->Draw(renderEngine);
}
