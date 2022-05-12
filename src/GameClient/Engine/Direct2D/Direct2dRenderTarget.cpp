#include "Direct2dRenderTarget.h"

#include "Direct2dColour.h"
#include "../../Utility/Direct2dUtility.h"
#include "../../Macros.h"

GameClient::Engine::Direct2D::Direct2dRenderTarget::Direct2dRenderTarget(ID2D1HwndRenderTarget* renderTarget):
	_pRenderTarget(renderTarget)
{
}

GameClient::Engine::Direct2D::Direct2dRenderTarget::~Direct2dRenderTarget()
{
	SafeRelease(&_pRenderTarget);
}

HRESULT GameClient::Engine::Direct2D::Direct2dRenderTarget::CreateDeviceResources()
{
	auto result = S_OK;
	for (auto c = 0; c < static_cast<int>(Colour::Last); ++c)
	{
		result = CreateAndAddBrush(static_cast<Colour>(c));
		RETURN_FAILED_HRESULT(result);
	}

	return result;
}

void GameClient::Engine::Direct2D::Direct2dRenderTarget::BeginDraw()
{
	_pRenderTarget->BeginDraw();
	_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
}

HRESULT GameClient::Engine::Direct2D::Direct2dRenderTarget::EndDraw()
{
	return _pRenderTarget->EndDraw();
}

void GameClient::Engine::Direct2D::Direct2dRenderTarget::DrawUnitRectangle(const Coordinate2d offset, const Coordinate2d position, const Coordinate2dF scale, const Colour colour)
{
	const auto rectangle = Utility::Direct2dUtility::CreateUnitRectangle(offset, position, scale);

	_pRenderTarget->FillRectangle(&rectangle, _brushes[colour]);
}

void GameClient::Engine::Direct2D::Direct2dRenderTarget::DrawLine(const Coordinate2dF p0, const Coordinate2dF p1, const float strokeWidth, const Colour colour)
{
	const auto d2Point0 = D2D1::Point2F(p0.x, p0.y);
	const auto d2Point1 = D2D1::Point2F(p1.x, p1.y);

	_pRenderTarget->DrawLine(d2Point0, d2Point1, _brushes[colour], strokeWidth);
}

void GameClient::Engine::Direct2D::Direct2dRenderTarget::Resize(const Size size)
{
	const auto d2Size = D2D1::SizeU(size.width, size.height);
	_pRenderTarget->Resize(d2Size);
}

HRESULT GameClient::Engine::Direct2D::Direct2dRenderTarget::CreateAndAddBrush(const Colour colour)
{
	const auto brushColour = Direct2dColour::GetDirect2dColour(colour);
	ID2D1SolidColorBrush* brush = nullptr;
	const auto result = _pRenderTarget->CreateSolidColorBrush(brushColour, &brush);

	RETURN_FAILED_HRESULT(result);

	_brushes[colour] = brush;
	return result;
}
