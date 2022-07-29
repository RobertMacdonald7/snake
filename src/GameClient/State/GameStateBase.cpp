#include "GameStateBase.h"

void GameClient::State::GameStateBase::Enter(const std::shared_ptr<GameClient::State::IGameState> previousState)
{
	// NoOp
}

void GameClient::State::GameStateBase::Leave()
{
	// NoOp
}

void GameClient::State::GameStateBase::OnUpdate(IStateMachine& context)
{
	// NoOp
}

void GameClient::State::GameStateBase::OnInput(IStateMachine& context, const Input::Input input)
{
	// NoOp
}

std::list<std::shared_ptr<GameClient::Engine::IDrawable>>& GameClient::State::GameStateBase::GetDrawables()
{
	return _drawables;
}

std::list<std::shared_ptr<GameClient::Engine::IDrawable>>& GameClient::State::GameStateBase::GetUIDrawables()
{
	return _uiDrawables;
}

int GameClient::State::GameStateBase::GetScore()
{
	return -1;
}
