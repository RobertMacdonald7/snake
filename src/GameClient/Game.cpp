#include "Game.h"

#include <string>

#include "GameStateMachine.h"

GameClient::Game::Game(std::unique_ptr<Engine::IEngine> engine):
_engine(std::move(engine))
{
	const HRESULT result = _engine->Initialize();

	if (FAILED(result))
	{
		const auto message = "Engine failed to initialize: " + std::to_string(result);
		throw std::exception(message.c_str());
	}
}

GameClient::Game::~Game() = default;

void GameClient::Game::OnResize(const UINT width, const UINT height) const
{
	_engine->Resize(width, height);
}

void GameClient::Game::OnInput(const Input::Keys input)
{
	State::GameStateMachine::GetInstance().OnInput(input);
}

void GameClient::Game::ProcessFrame()
{
	const auto currentTime = std::chrono::steady_clock::now();
	const auto frameTime = currentTime - _lastUpdateTime;
	_accumulatedFrameTime += frameTime;
	_lastUpdateTime = currentTime;
	unsigned short numberOfUpdates = 0;

	while (numberOfUpdates < _maxUpdatesPerFrame && _accumulatedFrameTime > _timeStep)
	{
		_accumulatedFrameTime -= _timeStep;
		State::GameStateMachine::GetInstance().OnUpdate();
		++numberOfUpdates;
	}
	
	_engine->Draw(State::GameStateMachine::GetInstance().GetDrawableEntities());
}
