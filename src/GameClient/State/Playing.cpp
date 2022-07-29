#include "Playing.h"

#include <utility>

GameClient::State::Playing::Playing(std::shared_ptr<UI::Score> score) :
	_score(std::move(score))
{
	const auto collisionDetector = std::make_shared<GameObjects::Collision::CollisionDetector>();
	_playArea = std::make_shared<GameObjects::PlayArea>(collisionDetector);
	_food = std::make_shared<GameObjects::Food>(collisionDetector);
	_snake = std::make_shared<GameObjects::Snake>(collisionDetector);

	// Add our IDrawables to the list
	GameStateBase::GetDrawables().push_back(_playArea);
	GameStateBase::GetDrawables().push_back(_food);
	GameStateBase::GetDrawables().push_back(_snake);

	// Add our UI IDrawables to the list
	GameStateBase::GetUIDrawables().push_back(_score);
};

GameClient::State::GameStateType GameClient::State::Playing::GetType()
{
	return GameStateType::Playing;
}

void GameClient::State::Playing::Enter(const std::shared_ptr<IGameState> previousState)
{
	// Reset the snake and food now we've started playing
	_snake->Reset();
	_food->Reset();

	// Update the score
	_score->SetScore(this->GetScore());
}

void GameClient::State::Playing::Leave()
{
	_blockInputUntilNextUpdate = false;
}

void GameClient::State::Playing::OnUpdate(IStateMachine& context)
{
	_blockInputUntilNextUpdate = false;

	// Update our updatable objects
	_snake->OnUpdate(context);
	_food->OnUpdate();

	// Update the score
	_score->SetScore(this->GetScore());

	// If there is some cached input to process, do it now
	if (_cachedInput != Input::Input::None)
	{
		_snake->OnInput(_cachedInput);
		_food->OnInput(_cachedInput);

		_cachedInput = Input::Input::None;

		// Don't allow any other input until we've processed the cached input next update
		_blockInputUntilNextUpdate = true;
	}
}

void GameClient::State::Playing::OnInput(IStateMachine& context, const Input::Input input)
{
	// Cache the input if we can't process it now
	if (_blockInputUntilNextUpdate)
	{
		_cachedInput = input;
		return;
	}

	// Process the input
	const auto snakeHandledInput = _snake->OnInput(input);
	const auto foodHandledInput = _food->OnInput(input);

	// Block addition input if either objects handled the current input
	_blockInputUntilNextUpdate = snakeHandledInput || foodHandledInput;
}

int GameClient::State::Playing::GetScore()
{
	return _snake->GetScore();
}
