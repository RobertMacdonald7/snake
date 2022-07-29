#include "StartMenuState.h"

GameClient::State::StartMenuState::StartMenuState(std::shared_ptr<UI::StartMenu> menu) :
	_menu(std::move(menu))
{
	GameStateBase::GetUIDrawables().push_back(_menu);
}

GameClient::State::GameStateType GameClient::State::StartMenuState::GetType()
{
	return GameStateType::StartMenu;
}

void GameClient::State::StartMenuState::Enter(const std::shared_ptr<IGameState> previousState)
{
	GameStateBase::Enter(previousState);
}

void GameClient::State::StartMenuState::Leave()
{
	GameStateBase::Leave();
}

void GameClient::State::StartMenuState::OnInput(IStateMachine& context, const Input::Input input)
{
	_menu->OnInput(context, input);
}
