#include "GameOverState.h"
#include <cassert>

GameClient::State::GameOverState::GameOverState(std::shared_ptr<UI::Scoreboard> scoreboard) :
	_scoreboard(std::move(scoreboard))
{
	GameStateBase::GetUIDrawables().push_back(_scoreboard);
}

void GameClient::State::GameOverState::OnInput(IStateMachine& context, const Input::Input input)
{
	if (input == Input::Input::SpaceBar)
	{
		context.ChangeState(static_cast<int>(GameStateType::Playing));
	}
}

GameClient::State::GameStateType GameClient::State::GameOverState::GetType()
{
	return GameStateType::GameOver;
}

void GameClient::State::GameOverState::Enter(const std::shared_ptr<IGameState> previousState)
{
	// We should only ever transition from the Playing state
	assert(previousState->GetType() == GameStateType::Playing);

	// Save the last playing state's drawable entities so that they can be drawn in the background still
	auto& background = previousState->GetDrawables();
	for (auto it = background.rbegin(); it != background.rend(); ++it)
	{
		GetDrawables().push_front(*it);
	}

	_scoreboard->SetScore(previousState->GetScore());
}

void GameClient::State::GameOverState::Leave()
{
	// Erase all drawables that are not native to the state
	auto& drawables = GetDrawables();
	const auto start = drawables.begin();
	auto end = drawables.end();
	std::advance(end, -2);

	drawables.erase(start, end);
}
