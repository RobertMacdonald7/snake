#include "Game.h"

#include "State/GameStateMachine.h"

GameClient::Game::Game(std::unique_ptr<Engine::IEngine> engine, std::unique_ptr<State::GameStateMachine> stateMachine):
	_engine(std::move(engine)),
	_stateMachine(std::move(stateMachine))
{ }

GameClient::Game::~Game() = default;

HRESULT GameClient::Game::Initialize() const
{
	return _engine->Initialize();
}

void GameClient::Game::OnResize(const int width, const int height) const
{
	_engine->Resize(width, height);
}

void GameClient::Game::OnInput(const Input::Input input)
{
	// Handle input for changing game speed by adjusting the update timer
	switch (input)
	{
	case Input::Input::One:
		_timeStep = std::chrono::milliseconds(_slow);
		break;
	case Input::Input::Two:
		_timeStep = std::chrono::milliseconds(_normal);
		break;
	case Input::Input::Three:
		_timeStep = std::chrono::milliseconds(_fast);
		break;
	default:
		break;
	}

	// Forward the input to the game
	_stateMachine->OnInput(input);
}

void GameClient::Game::ProcessFrame()
{
	const auto currentTime = std::chrono::steady_clock::now();
	const auto frameTime = currentTime - _lastUpdateTime;
	_accumulatedFrameTime += frameTime;
	_lastUpdateTime = currentTime;
	int numberOfUpdates = 0;

	// Perform as many updates as we need to catch up to the accumulated time
	// Performs at most _maxUpdatesPerFrame before drawing
	// A smaller _timeStep will allow updates to occur more frequently
	while (numberOfUpdates < _maxUpdatesPerFrame && _accumulatedFrameTime > _timeStep)
	{
		_accumulatedFrameTime -= _timeStep;
		_stateMachine->OnUpdate();
		++numberOfUpdates;
	}

	// Draw the frame
	_engine->Draw(_stateMachine->GetDrawableEntities());
}
