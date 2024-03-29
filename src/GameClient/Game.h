#pragma once
#include <memory>
#include <chrono>

#include "../Macros.h"
#include "IEngine.h"
#include "Input/Input.h"
#include "State/GameStateMachine.h"

namespace GameClient
{
	/**
	 * \brief Wraps the Engine::IEngine and State::GameStateMachine, processes frames, controls game update speed, and forwards input.
	 */
	class Game
	{
	private:
		// Game speed
		inline static int _slow = 200;
		inline static int _normal = 100;
		inline static int _fast = 50;

		std::unique_ptr<Engine::IEngine> _engine;
		std::unique_ptr<State::GameStateMachine> _stateMachine;

		std::chrono::steady_clock::time_point _lastUpdateTime = std::chrono::steady_clock::now();
		std::chrono::nanoseconds _accumulatedFrameTime = std::chrono::nanoseconds(0);
		int _maxUpdatesPerFrame = 1;
		std::chrono::milliseconds _timeStep = std::chrono::milliseconds(_normal);

	public:
		Game(std::unique_ptr<Engine::IEngine> engine,
		     std::unique_ptr<State::GameStateMachine> stateMachine);
		~Game();
		HRESULT Initialize() const;
		void OnResize(int width, int height) const;
		void OnInput(Input::Input input);

		Game(Game& copyOther) = delete;
		Game operator=(Game& copyOther) = delete;

		Game(Game&& moveOther) = delete;
		Game operator=(Game&& moveOther) = delete;

		/**
		 * \brief Processes a frame by calling Update then Draw.
		 */
		void ProcessFrame();
	};
}
