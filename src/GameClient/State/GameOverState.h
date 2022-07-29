#pragma once
#include "GameStateBase.h"
#include "../UI/Scoreboard.h"

namespace GameClient::State
{
	/**
	 * \brief Defines the IGameState when the game is over, and the player cannot move the snake anymore.
	 */
	class GameOverState final : public GameStateBase
	{
	private:
		std::shared_ptr<UI::Scoreboard> _scoreboard = nullptr;

	public:
		explicit GameOverState(std::shared_ptr<UI::Scoreboard> scoreboard);
		GameOverState() = delete;

		/**
		 * \copydoc GameStateBase::GetType
		 */
		GameStateType GetType() override;

		/**
		 * \copydoc GameStateBase::Enter
		 */
		void Enter(std::shared_ptr<IGameState> previousState) override;

		/**
		 * \copydoc GameStateBase::Leave
		 */
		void Leave() override;

		/**
		 * \copydoc GameStateBase::OnInput
		 */
		void OnInput(IStateMachine& context, Input::Input input) override;
	};
}
