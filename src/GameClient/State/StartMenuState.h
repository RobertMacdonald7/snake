#pragma once

#include "GameStateBase.h"
#include "../UI/StartMenu.h"

namespace GameClient::State
{
	class StartMenuState final : public GameStateBase
	{
	private:
		std::shared_ptr<UI::StartMenu> _menu = nullptr;

	public:
		explicit StartMenuState(std::shared_ptr<UI::StartMenu> menu);
		StartMenuState() = delete;

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
