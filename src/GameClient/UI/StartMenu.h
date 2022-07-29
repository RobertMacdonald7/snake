#pragma once

#include <memory>

#include "../Engine/IDrawable.h"
#include "../Input/Input.h"
#include "../State/IStateMachine.h"
#include "Controls/Button.h"

namespace GameClient::UI
{
	class StartMenu final : public Engine::IDrawable
	{
	private:
		std::unique_ptr<Controls::Button> _startButton = nullptr;
		std::unique_ptr<Controls::Button> _exitButton = nullptr;

	public:
		StartMenu();
		~StartMenu() override = default;
		StartMenu(StartMenu& copyOther) = delete;
		StartMenu operator=(StartMenu& copyOther) = delete;

		StartMenu(StartMenu&& moveOther) = delete;
		StartMenu operator=(StartMenu&& moveOther) = delete;

		void OnInput(State::IStateMachine& context, Input::Input input) const;
		void OnUpdate(State::IStateMachine& context);

		/**
		 * \copydoc Engine::IDrawable::Draw
		 */
		void Draw(Engine::IRender& renderEngine) override;
	};
}
