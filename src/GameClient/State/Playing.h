#pragma once
#include "GameStateBase.h"
#include "../GameObjects/GameObjects.h"
#include "../UI/Score.h"

namespace GameClient::State
{
	/**
	 * \brief Defines the IGameState when the game is in progress, and the player can move the snake.
	 */
	class Playing final : public GameStateBase
	{
	private:
		std::shared_ptr<GameObjects::PlayArea> _playArea = nullptr;
		std::shared_ptr<GameObjects::Food> _food = nullptr;
		std::shared_ptr<GameObjects::Snake> _snake = nullptr;
		std::shared_ptr<UI::Score> _score = nullptr;

		bool _blockInputUntilNextUpdate = false;
		Input::Input _cachedInput = Input::Input::None;

	public:
		explicit Playing(std::shared_ptr<UI::Score> score);
		Playing() = delete;

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
		 * \copydoc GameStateBase::OnUpdate
		 */
		void OnUpdate(IStateMachine& context) override;

		/**
		 * \copydoc GameStateBase::OnInput
		 */
		void OnInput(IStateMachine& context, Input::Input input) override;

		/**
		 * \copydoc GameStateBase::GetScore
		 */
		int GetScore() override;
	};
}
