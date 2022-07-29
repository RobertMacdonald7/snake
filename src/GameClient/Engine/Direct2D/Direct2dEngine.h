#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <map>

#include "../IEngine.h"
#include "../TextAlignment.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite")

namespace GameClient::Engine::Direct2D
{
	/**
	 * \brief %Direct2D implementation of IEngine
	 */
	class Direct2dEngine final : public IEngine
	{
	private:
		ID2D1Factory* _pDirect2dFactory = nullptr;
		IDWriteFactory* _pDWriteFactory = nullptr;

		std::map<TextAlignment, IDWriteTextFormat*> _textFormats = {};

		ID2D1HwndRenderTarget* _pRenderTarget = nullptr;
		std::map<Colour, ID2D1SolidColorBrush*> _brushes = {};

	public:
		explicit Direct2dEngine(HWND windowHandle);
		~Direct2dEngine() override;

		Direct2dEngine(Direct2dEngine& copyOther) = delete;
		Direct2dEngine operator=(Direct2dEngine& copyOther) = delete;

		Direct2dEngine operator=(Direct2dEngine&& moveOther) = delete;
		Direct2dEngine(Direct2dEngine&& moveOther) = delete;

		/**
		 * \copydoc IEngine::Initialize
		 */
		HRESULT Initialize() override;

		/**
		 * \copydoc IEngine::Resize
		 */
		void Resize(int width, int height) const override;

		/**
		 * \copydoc IEngine::Draw
		 */
		HRESULT Draw(const std::list<std::shared_ptr<IDrawable>>& drawables) override;

		/**
		 * \copydoc IRender::FillUnitRectangle
		 */
		void FillUnitRectangle(Coordinate2d offset, Coordinate2d position, Coordinate2dF scale, Colour colour) override;

		/**
		 * \copydoc IRender::DrawRectangle
		 */
		void DrawRectangle(Coordinate2d location, Size size, float strokeWidth, Colour colour) override;

		/**
		 * \copydoc IRender::FillRectangle
		 */
		void FillRectangle(Coordinate2d location, Size size, Colour colour) override;

		/**
		 * \copydoc IRender::DrawLine
		 */
		void DrawLine(Coordinate2dF p0, Coordinate2dF p1, float strokeWidth, Colour colour) override;

		/**
		 * \copydoc IRender::DrawString
		 */
		void DrawString(const std::wstring& text, Coordinate2d location, Size size, Colour colour, TextAlignment alignment) override;

	private:
		/**
		 * \brief Creates the render target and it's device resources.
		 * \return Result of the operation.
		 */
		HRESULT CreateDeviceResources();

		/**
		 * \brief Safely discards the Direct2D factory
		 */
		void DiscardDeviceIndependentResources();

		/**
		 * \brief Safely discards the render target
		 */
		void DiscardDeviceResources();

		/**
		 * \brief Creates and adds a Direct2D colour brush to _brushes.
		 * \param colour The colour of brush to create.
		 * \return The result of the operation.
		 */
		HRESULT CreateAndAddBrush(Colour colour);
	};
}
