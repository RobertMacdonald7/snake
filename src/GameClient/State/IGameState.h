#pragma once
#include <memory>
#include <vector>

#include "GameStateType.h"
#include "../Input/Input.h"
#include "../Engine/IDrawable.h"

namespace GameClient::State
{
	class IGameState  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		virtual ~IGameState() = default;

		virtual GameStateType GetType() = 0;

		virtual void Enter(std::shared_ptr<IGameState> previousState) = 0;
		virtual void Leave() = 0;

		virtual void OnUpdate() = 0;
		virtual void OnInput(Input::Input input) = 0;

		virtual std::vector<std::shared_ptr<Engine::IDrawable>>& GetDrawables() = 0;
	};
}