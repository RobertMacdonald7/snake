#include "Direct2dEngine.h"

#include <stdexcept>

#include "Direct2dColour.h"
#include "../../Utility/Direct2dUtility.h"

GameClient::Engine::Direct2D::Direct2dEngine::Direct2dEngine(HWND windowHandle): IEngine(windowHandle)
{
}

GameClient::Engine::Direct2D::Direct2dEngine::~Direct2dEngine()
{
	DiscardDeviceIndependentResources();
	DiscardDeviceResources();
}

HRESULT GameClient::Engine::Direct2D::Direct2dEngine::Initialize()
{
	// Create a Direct2D factory
	auto result = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&_pDirect2dFactory
	);
	RETURN_FAILED_HRESULT(result);

	// Create a DWrite factory
	result = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),  // NOLINT(clang-diagnostic-language-extension-token)
		std::bit_cast<IUnknown**>(&_pDWriteFactory)
	);
	RETURN_FAILED_HRESULT(result);

	// TODO - move these into some other place responsible for creating all available text formats. Potentially lazy?
	IDWriteTextFormat* centerFormat;
	result = _pDWriteFactory->CreateTextFormat(
		L"",
		nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50.0f,
		L"en-us",
		&centerFormat
	);
	RETURN_FAILED_HRESULT(result);

	result = centerFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	RETURN_FAILED_HRESULT(result);

	result = centerFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	RETURN_FAILED_HRESULT(result);

	_textFormats[TextAlignment::Center] = centerFormat;

	IDWriteTextFormat* topRightFormat;
	result = _pDWriteFactory->CreateTextFormat(
		L"",
		nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50.0f,
		L"en-us",
		&topRightFormat
	);
	RETURN_FAILED_HRESULT(result);

	result = topRightFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	RETURN_FAILED_HRESULT(result);

	result = topRightFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	RETURN_FAILED_HRESULT(result);

	_textFormats[TextAlignment::TopRight] = topRightFormat;

	return result;
}

void GameClient::Engine::Direct2D::Direct2dEngine::Resize(const int width, const int height) const
{
	if (!_pRenderTarget)
	{
		// Can't resize without a render target!
		return;
	}

	const auto d2Size = D2D1::SizeU(width, height);
	_pRenderTarget->Resize(d2Size);
}

HRESULT GameClient::Engine::Direct2D::Direct2dEngine::Draw(const std::list<std::shared_ptr<IDrawable>>& drawables)
{
	// Create any resources needed before drawing
	auto result = CreateDeviceResources();
	RETURN_FAILED_HRESULT(result);

	_pRenderTarget->BeginDraw();
	_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// Draw each IDrawable
	for (auto& drawable : drawables)
	{
		drawable->Draw(*this);
	}

	result = _pRenderTarget->EndDraw();

	if (result == D2DERR_RECREATE_TARGET)
	{
		// Discard, resources will be recreated next Draw()
		result = S_OK;
		DiscardDeviceResources();
	}

	return result;
}

void GameClient::Engine::Direct2D::Direct2dEngine::FillUnitRectangle(const Coordinate2d offset, const Coordinate2d position,
                                                                     const Coordinate2dF scale, const Colour colour)
{
	// Create and draw
	const auto rectangle = Utility::Direct2dUtility::CreateUnitRectangle(offset, position, scale);
	_pRenderTarget->FillRectangle(rectangle, _brushes[colour]);
}

void GameClient::Engine::Direct2D::Direct2dEngine::DrawRectangle(const Coordinate2d location, const Size size, float strokeWidth, const Colour colour)
{
	const D2D1_RECT_F rectangle = D2D1::RectF(
		static_cast<float>(location.x),
		static_cast<float>(location.y),
		static_cast<float>(location.x + size.width),
		static_cast<float>(location.y + size.height)
	);
	_pRenderTarget->DrawRectangle(rectangle, _brushes[colour], strokeWidth);
}

void GameClient::Engine::Direct2D::Direct2dEngine::FillRectangle(const Coordinate2d location, const Size size,
                                                                 const Colour colour)
{
	const D2D1_RECT_F rectangle = D2D1::RectF(
		static_cast<float>(location.x),
		static_cast<float>(location.y),
		static_cast<float>(location.x + size.width),
		static_cast<float>(location.y + size.height)
	);
	_pRenderTarget->FillRectangle(rectangle, _brushes[colour]);
}

void GameClient::Engine::Direct2D::Direct2dEngine::DrawLine(const Coordinate2dF p0, const Coordinate2dF p1, const float strokeWidth,
                                                            const Colour colour)
{
	// Create and draw
	const auto d2Point0 = D2D1::Point2F(p0.x, p0.y);
	const auto d2Point1 = D2D1::Point2F(p1.x, p1.y);
	_pRenderTarget->DrawLine(d2Point0, d2Point1, _brushes[colour], strokeWidth);
}

void GameClient::Engine::Direct2D::Direct2dEngine::DrawString(const std::wstring& text, const Coordinate2d location, const Size size, const Colour colour, const TextAlignment alignment)
{
	const auto textLength = static_cast<UINT32>(text.length());
	const D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<float>(location.x),
		static_cast<float>(location.y),
		static_cast<float>(location.x + size.width),
		static_cast<float>(location.y + size.height)
	);
	_pRenderTarget->DrawTextW(text.data(), textLength, _textFormats[alignment], layoutRect, _brushes[colour]);
}

HRESULT GameClient::Engine::Direct2D::Direct2dEngine::CreateDeviceResources()
{
	auto result = S_OK;

	if (_pRenderTarget)
	{
		// render target is already created, no need to re-create
		return result;
	}

	// Make the render target buffer equal to the window size
	RECT rect;
	GetClientRect(GetWindowHandle(), &rect);
	const D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);

	// Create the render target
	result = _pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(GetWindowHandle(), size),
		&_pRenderTarget);

	// Assumes renderTarget is nullptr if CreateHwndRenderTarget fails
	RETURN_FAILED_HRESULT(result);

	// Create a brush for each possible colour
	for (auto c = 0; c < static_cast<int>(Colour::Last); ++c)
	{
		result = CreateAndAddBrush(static_cast<Colour>(c));
		RETURN_FAILED_HRESULT(result);
	}

	return result;
}

void GameClient::Engine::Direct2D::Direct2dEngine::DiscardDeviceIndependentResources()
{
	for (auto& [_, format] : _textFormats)
	{
		SafeRelease(&format);
	}
	_textFormats.clear();

	SafeRelease(&_pDWriteFactory);
	SafeRelease(&_pDirect2dFactory);
}

void GameClient::Engine::Direct2D::Direct2dEngine::DiscardDeviceResources()
{
	// Release all the brushes
	for (auto& [colour, brush] : _brushes)
	{
		SafeRelease(&brush);
	}

	_brushes.clear();

	SafeRelease(&_pRenderTarget);
}

HRESULT GameClient::Engine::Direct2D::Direct2dEngine::CreateAndAddBrush(const Colour colour)
{
	// Convert the colour
	D2D1_COLOR_F brushColour;
	try
	{
		brushColour = Direct2dColour::GetDirect2dColour(colour);
	}
	catch (const std::out_of_range&)
	{
		// The colour is not supported, bubble up the failure and abort.
		return E_FAIL;
	}

	// Create the brush
	ID2D1SolidColorBrush* brush = nullptr;
	const auto result = _pRenderTarget->CreateSolidColorBrush(brushColour, &brush);

	RETURN_FAILED_HRESULT(result);

	// Store the brush
	_brushes[colour] = brush;
	return result;
}
