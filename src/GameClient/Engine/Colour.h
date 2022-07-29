#pragma once

namespace GameClient::Engine
{
	/**
	 * \brief Basic, engine agnostic, colours.
	 */
	enum class Colour
	{
		White,
		WhiteAlpha,
		Gray,
		DarkRed,
		Green,
		Blue,
		Black,
		BlackAlpha,
		Last // Used for detecting the end of iteration!
	};
}
