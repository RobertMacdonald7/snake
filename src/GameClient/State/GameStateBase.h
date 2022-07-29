#pragma once

#include "IGameState.h"

namespace GameClient::State
{
	/**
	 * \brief Base class of IGameState
	 */
	class GameStateBase : public IGameState // NOLINT(cppcoreguidelines-special-member-functions)
	{
	private:
		std::list<std::shared_ptr<Engine::IDrawable>> _drawables = {};
		std::list<std::shared_ptr<Engine::IDrawable>> _uiDrawables = {};

	public:
		~GameStateBase() override = default;

		GameStateType GetType() override = 0;
		void Enter(std::shared_ptr<IGameState> previousState) override;
		void Leave() override;
		void OnUpdate(IStateMachine& context) override;
		void OnInput(IStateMachine& context, Input::Input input) override;
		std::list<std::shared_ptr<Engine::IDrawable>>& GetDrawables() override;
		std::list<std::shared_ptr<GameClient::Engine::IDrawable>>& GetUIDrawables() override;
		int GetScore() override;

	protected:
		GameStateBase() = default;
	};
}
