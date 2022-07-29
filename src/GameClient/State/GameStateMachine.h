#pragma once
#include "IGameState.h"
#include "GameStates.h"
#include "IStateMachine.h"

namespace GameClient::State
{
	/**
	 * \brief An IStateMachine that defines the overall game state.
	 */
	class GameStateMachine final : public IStateMachine
	{
	private:
		std::map<int, std::shared_ptr<IGameState>> _states;
		std::shared_ptr<IGameState> _currentState = nullptr;

	public:
		GameStateMachine() = delete;
		explicit GameStateMachine(const std::vector<std::shared_ptr<IGameState>>& states);
		~GameStateMachine() override = default;

		GameStateMachine(GameStateMachine& copyOther) = delete;
		GameStateMachine operator=(GameStateMachine& copyOther) = delete;

		GameStateMachine(GameStateMachine&& moveOther) = delete;
		GameStateMachine operator=(GameStateMachine&& moveOther) = delete;

		/**
		 * \copydoc IStateMachine::ChangeState
		 */
		void ChangeState(int state) override;

		/**
		 * \copydoc IStateMachine::OnInput
		 */
		void OnInput(Input::Input input) override;

		/**
		 * \copydoc IStateMachine::OnUpdate
		 */
		void OnUpdate() override;

		/**
		 * \brief Gets the IDrawable objects.
		 * \return A list of IDrawables.
		 */
		[[nodiscard]] std::list<std::shared_ptr<Engine::IDrawable>> GetDrawableEntities() const;
	};
}
