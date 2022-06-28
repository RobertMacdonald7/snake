#pragma once

#include <string>

#include "../../Engine/IDrawable.h"

namespace GameClient::UI::Controls
{
	class Label final : public Engine::IDrawable
	{
	public:
		std::wstring text;
		Engine::Coordinate2d location = { 0,0 };
		Engine::Size size = { 0,0 };
		bool border = false;

		/**
		 * \copydoc Engine::IDrawable::Draw
		 */
		void Draw(Engine::IRender& renderEngine) override;
	};
}
