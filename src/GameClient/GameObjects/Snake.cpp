#include "Snake.h"

#include <chrono>

#include "../GameDefinitions.h"
#include "../State/GameStateType.h"

GameClient::GameObjects::Snake::Snake(std::shared_ptr<Collision::CollisionDetector> collisionDetector) :
	CollidableBase(std::move(collisionDetector))
{
	// Add this instance to the CollisionDetector
	GetCollisionDetector()->AddCollidable(GetCollidableName(), this);

	// Create the snake
	CreatePlayer();
}

GameClient::GameObjects::Snake::~Snake()
{
	// Remove this instance from the CollisionDetector
	GetCollisionDetector()->RemoveCollidable(GetCollidableName());
}

bool GameClient::GameObjects::Snake::OnInput(const Input::Input input)
{
	if (input == Input::Input::SpaceBar)
	{
		// Reset the snake if space bar was pressed
		Reset();
		return true;
	}

#ifdef _DEBUG
	// Allows growing of the snake of pressing 'G' in debug mode
	if (input == Input::Input::G)
	{
		_growNextUpdate = true;
		return true;
	}
#endif

	// Handles snake direction input
	const auto originalDirection = _direction;
	switch (input)
	{
	case Input::Input::Up:
		if (_direction == Direction::Down) return false;
		_direction = Direction::Up;
		break;
	case Input::Input::Down:
		if (_direction == Direction::Up) return false;
		_direction = Direction::Down;
		break;
	case Input::Input::Left:
		if (_direction == Direction::Right) return false;
		_direction = Direction::Left;
		break;
	case Input::Input::Right:
		if (_direction == Direction::Left) return false;
		_direction = Direction::Right;
		break;
	default:
		return false;
	}

	// The input was handle if the direction has changed
	return _direction != originalDirection;
}

void GameClient::GameObjects::Snake::OnUpdate(State::IStateMachine& context)
{
	// Grow the snake now before moving the snake
	if (_growNextUpdate)
	{
		++_growSnake;
		_growNextUpdate = false;
	}

	// Now move the snake, the growth will be taken into account here
	return MoveSnake(context);
}

void GameClient::GameObjects::Snake::Reset()
{
	// Clear and re-create the snake
	_segments.clear();
	CreatePlayer();

	// Default snake direction
	_direction = Direction::Right;

	// Reset growth
	_growSnake = 0;
}

int GameClient::GameObjects::Snake::GetScore() const
{
	return static_cast<int>(_segments.size() - _initialSize);
}

void GameClient::GameObjects::Snake::Draw(Engine::IRender& renderEngine)
{
	// Draw simple squares for each snake segment
	for (const auto& segment : _segments)
	{
		renderEngine.FillUnitRectangle({ 0, 0 }, segment, { 0, 0 }, Engine::Colour::Blue);
	}
}

GameClient::GameObjects::Collision::CollidableName GameClient::GameObjects::Snake::GetCollidableName()
{
	return Collision::CollidableName::Snake;
}

bool GameClient::GameObjects::Snake::AteFood(const int x, const int y)
{
	// Check collision with food
	return GetCollisionDetector()->IsColliding(x, y, GetCollidableName(), Collision::CollidableName::Food);
}

void GameClient::GameObjects::Snake::MoveSnake(State::IStateMachine& context)
{
	int deltaX = 0;
	int deltaY = 0;
	switch (_direction)
	{
	case Direction::Right:
		++deltaX;
		break;
	case Direction::Down:
		++deltaY;
		break;
	case Direction::Left:
		--deltaX;
		break;
	case Direction::Up:
		--deltaY;
		break;
	default:
		throw std::out_of_range("Direction was not a handled value");
	}

	// Calculate the new head position
	const auto& [x, y] = _segments.front();
	const auto newHead = Engine::Coordinate2d{x + deltaX, y + deltaY};

	// Check collision with itself + any walls
	if (GetCollisionDetector()->IsColliding(newHead.x, newHead.y, GetCollidableName(),
	                                        Collision::CollidableName::PlayArea | Collision::CollidableName::Snake))
	{
		context.ChangeState(static_cast<int>(State::GameStateType::GameOver));
		return;
	}

	// Check if we're colliding with food after moving
	if (AteFood(newHead.x, newHead.y))
	{
		_growNextUpdate = true;
	}

	_segments.insert(_segments.begin(), newHead);

	// If we need to grow the snake don't pop the back element
	if (_growSnake > 0)
	{
		--_growSnake;
	}
	else
	{
		_segments.pop_back();
	}
}

void GameClient::GameObjects::Snake::CreatePlayer()
{
	// Creates the snake at the initial start position
	constexpr auto initialX = 10;
	constexpr auto initialSnakeHead = Engine::Coordinate2d{initialX, game_height_units / 2};
	_segments.push_back(initialSnakeHead);

	// Push more segments so we're not just a head
	for (auto x = initialX - 1; x > initialX - 5; --x)
	{
		auto segment = Engine::Coordinate2d{x, game_height_units / 2};
		_segments.push_back(segment);
	}

	_initialSize = _segments.size();
}

bool GameClient::GameObjects::Snake::IsColliding(const int x, const int y, Collision::CollidableName source)
{
	// Check if the coordinates match any of our segments
	if (const auto findIt = std::ranges::find(_segments, Engine::Coordinate2d{x, y}); findIt != _segments.end())
		return true;

	return false;
}
