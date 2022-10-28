#pragma once

#include <memory>

#include "EngineLib/IDrawable.h"
#include "Controls/Label.h"
#include "Web.Client/Client.h"

namespace GameClient::UI
{
	class Scoreboard final : public Engine::IDrawable
	{
	private:
		std::unique_ptr<Controls::Label> _scoreLabel = nullptr;
		std::unique_ptr<Controls::Label> _highScoreLabel = nullptr;
		std::unique_ptr<Controls::Label> _restartPromptLabel = nullptr;
		int _highScore = 0;
		std::unique_ptr<GameClient::Web::Client::ScoreClient> _client = nullptr;


	public:
		Scoreboard();
		~Scoreboard() override = default;
		Scoreboard(Scoreboard& copyOther) = delete;
		Scoreboard operator=(Scoreboard& copyOther) = delete;

		Scoreboard(Scoreboard&& moveOther) = delete;
		Scoreboard operator=(Scoreboard&& moveOther) = delete;

		/**
		 * \brief Sets the score and high score
		 */
		void SetScore(int score);

		/**
		 * \copydoc Engine::IDrawable::Draw
		 */
		void Draw(Engine::IRender & renderEngine) override;
	};
}
