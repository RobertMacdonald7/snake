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

// TODO - use the message loop for this instead?
void GameClient::Game::HandleInput()
{
	auto currentKeyboardState = 0;
	if (GetAsyncKeyState(VK_UP))
	{
		currentKeyboardState |= static_cast<int>(Input::Keys::UpArrow);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		currentKeyboardState |= static_cast<int>(Input::Keys::DownArrow);
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		currentKeyboardState |= static_cast<int>(Input::Keys::LeftArrow);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		currentKeyboardState |= static_cast<int>(Input::Keys::RightArrow);
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		currentKeyboardState |= static_cast<int>(Input::Keys::SpaceBar);
	}

	const auto changedKeys = currentKeyboardState ^ _previousKeyboardState;
	const auto keysDown = static_cast<Input::Keys>(currentKeyboardState & changedKeys);

	State::GameStateMachine::GetInstance().OnInput(keysDown);
	_previousKeyboardState = currentKeyboardState;
}

void GameClient::Game::ProcessFrame()
{
	const auto currentTime = std::chrono::steady_clock::now();
	const auto frameTime = currentTime - _lastUpdateTime;
	_accumulatedFrameTime += frameTime;
	_lastUpdateTime = currentTime;
	unsigned short numberOfUpdates = 0;

	// TODO - Input buffering
	HandleInput();

	while (numberOfUpdates < _maxUpdatesPerFrame && _accumulatedFrameTime > _timeStep)
	{
		_accumulatedFrameTime -= _timeStep;
		State::GameStateMachine::GetInstance().OnUpdate();
		++numberOfUpdates;
	}
	
	_engine->Draw(State::GameStateMachine::GetInstance().GetDrawableEntities());
}
