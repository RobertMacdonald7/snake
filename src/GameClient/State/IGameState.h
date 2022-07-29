#pragma once
#include <memory>
#include <list>

#include "GameStateType.h"
#include "IStateMachine.h"
#include "../Engine/IDrawable.h"

namespace GameClient::State
{
	/**
	 * \brief Interface describing a game state.
	 */
	class IGameState // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		virtual ~IGameState() = default;

		/**
		 * \brief Gets the type of game state.
		 * \return The game state type.
		 */
		virtual GameStateType GetType() = 0;

		/**
		 * \brief Enters this state
		 * \param previousState The state we're leaving.
		 */
		virtual void Enter(std::shared_ptr<IGameState> previousState) = 0;

		/**
		 * \brief Leaves this state.
		 */
		virtual void Leave() = 0;

		/**
		 * \brief Updates all updatable objects.
		 * \param context The state machine context.
		 */
		virtual void OnUpdate(IStateMachine& context) = 0;

		/**
		 * \brief Sends input to all objects that can process it.
		 * \param context The state machine context.
		 * \param input The pressed key.
		 */
		virtual void OnInput(IStateMachine& context, Input::Input input) = 0;

		/**
		 * \brief Gets the IDrawable objects.
		 * \return A list of IDrawables.
		 */
		virtual std::list<std::shared_ptr<Engine::IDrawable>>& GetDrawables() = 0;

		/**
		 * \brief Gets the UI IDrawable objects.
		 * \return A list of IDrawables.
		 */
		virtual std::list<std::shared_ptr<GameClient::Engine::IDrawable>>& GetUIDrawables() = 0;

		/**
		 * \brief Gets the current score of the game.
		 * \return The score. -1 if no score is available.
		 */
		virtual int GetScore() = 0;
	};
}
