#pragma once

#include <memory>

#include "../Engine/IDrawable.h"
#include "Controls/Label.h"

namespace GameClient::UI
{
	class Score final : public Engine::IDrawable
	{
	private:
		std::unique_ptr<Controls::Label> _scoreLabel = nullptr;

	public:
		Score();
		~Score() override = default;

		Score(Score& copyOther) = delete;
		Score operator=(Score& copyOther) = delete;

		Score(Score&& moveOther) = delete;
		Score operator=(Score&& moveOther) = delete;

		/**
		 * \brief Sets the score
		 */
		void SetScore(int score);

		/**
		 * \copydoc Engine::IDrawable::Draw
		 */
		void Draw(Engine::IRender& renderEngine) override;
	};
}
