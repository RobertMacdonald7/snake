#pragma once

#include <string>

#include "Colour.h"
#include "Coordinate2d.h"
#include "Size.h"
#include "TextAlignment.h"

namespace GameClient::Engine
{
	/**
	 * \brief Interface for the engine's render methods.
	 */
	class IRender // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		virtual ~IRender() = default;

		/**
		 * \brief Draws a unit rectangle.
		 * \param offset X,Y offset from the rectangle's position.
		 * \param position X,Y position of the rectangle's top-left corner.
		 * \param scale How much vertical/horizontal space to shrink the rectangle.
		 * \param colour The colour of the filled rectangle.
		 */
		virtual void FillUnitRectangle(Coordinate2d offset, Coordinate2d position, Coordinate2dF scale,
									   Colour colour) = 0;

		/**
		 * \brief Draws a rectangle.
		 * \param location The top,left location of the rectangle.
		 * \param size The size of the rectangle.
		 * \param strokeWidth The stroke width.
		 * \param colour The colour of the rectangle.
		 */
		virtual void DrawRectangle(Coordinate2d location, Size size, float strokeWidth, Colour colour) = 0;

		/**
		 * \brief Draws a filled rectangle.
		 * \param location The top,left location of the rectangle.
		 * \param size The size of the rectangle.
		 * \param colour The colour of the rectangle.
		 */
		virtual void FillRectangle(Coordinate2d location, Size size, Colour colour) = 0;

		/**
		 * \brief Draws a line.
		 * \param p0 The first point of the line.0
		 * \param p1 The second point of the line.
		 * \param strokeWidth The width of the line.
		 * \param colour The colour of the line.
		 */
		virtual void DrawLine(Coordinate2dF p0, Coordinate2dF p1, float strokeWidth, Colour colour) = 0;

		/**
		 * \brief Draws the given string.
		 * \param text The text to draw.
		 * \param location The top,left location of the text's rectangle.
		 * \param size The size of the text's rectangle.
		 * \param colour The text's colour.
		 * \param alignment The text alignment.
		 */
		virtual void DrawString(const std::wstring& text, Coordinate2d location, Size size, Colour colour,
								TextAlignment alignment) = 0;
	};
}
