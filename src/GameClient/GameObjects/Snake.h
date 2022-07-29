#pragma once
#include <chrono>
#include <vector>

#include "../Engine/IDrawable.h"
#include "../Input/Input.h"
#include "../Engine/Coordinate2d.h"
#include "../State/IStateMachine.h"
#include "Collision/CollidableBase.h"

namespace GameClient::GameObjects
{
	/**
	 * \brief Defines the "snake" controlled by the user.
	 */
	class Snake final : public Engine::IDrawable, public Collision::CollidableBase
	{
	private:
		/**
		 * \brief The direction the snake can travel in.
		 */
		enum class Direction
		{
			Up,
			Down,
			Left,
			Right
		};

		Direction _direction = Direction::Right;
		std::vector<Engine::Coordinate2d> _segments;

		bool _growNextUpdate = false;
		int _growSnake = 0;
		size_t _initialSize;

	public:
		explicit Snake(std::shared_ptr<Collision::CollisionDetector> collisionDetector);
		~Snake() override;

		Snake() = delete;
		Snake(Snake& copyOther) = delete;
		Snake operator=(Snake& copyOther) = delete;

		Snake(Snake&& moveOther) = delete;
		Snake operator=(Snake&& moveOther) = delete;

		/**
		 * \brief Handles key presses.
		 * \param input The pressed key.
		 * \return Whether the input was handled.
		 */
		bool OnInput(Input::Input input);

		/**
		 * \brief Updates the snake's position and length
		 * \param context The game state context
		 */
		void OnUpdate(State::IStateMachine& context);

		/**
		 * \brief Reset's the snake to default values.
		 */
		void Reset();

		/**
		 * \brief Gets the current score.
		 */
		[[nodiscard]] int GetScore() const;

		/**
		 * \copydoc Engine::IDrawable::Draw
		 */
		void Draw(Engine::IRender& renderEngine) override;

		/**
		 * \copydoc Collision::CollidableBase::GetCollidableName
		 */
		Collision::CollidableName GetCollidableName() override;

		/**
		 * \copydoc Collision::CollidableBase::IsColliding
		 */
		[[nodiscard]] bool IsColliding(int x, int y, Collision::CollidableName source) override;

	private:
		/**
		 * \brief Tests whether the coordinates collide with any food.
		 * \param x The X coordinate to test.
		 * \param y The Y coordinate to test.
		 * \return Whether the snake is colliding with food.
		 */
		bool AteFood(int x, int y);

		/**
		 * \brief Moves the snake one unit.
		 * \param context The game state context.
		 */
		void MoveSnake(State::IStateMachine& context);

		/**
		 * \brief Creates the snake with default values
		 */
		void CreatePlayer();
	};
}
