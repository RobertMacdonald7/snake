#include "Window.h"

#include "GameDefinitions.h"
#include "Engine/Direct2D/Direct2dEngine.h"
#include "Resource/resource.h"
#include "Input/VirtualKeyCodes.h"
#include "UI/Score.h"

GameClient::Window::Window() = default;

GameClient::Window::~Window() = default;

HRESULT GameClient::Window::Initialize(HINSTANCE hInstance)
{
	// Register the window class
	WNDCLASSEX windowClassEx = {};
	windowClassEx.cbSize = sizeof(WNDCLASSEX);
	windowClassEx.style = CS_HREDRAW | CS_VREDRAW;
	windowClassEx.lpfnWndProc = WndProc;
	windowClassEx.cbClsExtra = 0;
	windowClassEx.cbWndExtra = sizeof(LONG_PTR);
	windowClassEx.hInstance = HINST_THISCOMPONENT;
	windowClassEx.hbrBackground = nullptr;
	windowClassEx.lpszMenuName = nullptr;
	windowClassEx.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
	windowClassEx.lpszClassName = L"Window";
	windowClassEx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE));

	RegisterClassEx(&windowClassEx);


	// Because the CreateWindow function takes its size in pixels,
	// obtain the system DPI and use it to scale the window size
	const auto dpiX = static_cast<float>(GetDpiForWindow(GetDesktopWindow()));
	const auto dpiY = dpiX;

	// Adjust the window to fit desired resolution
	constexpr DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	RECT windowRectangle = {0, 0, game_width_pixels, game_height_pixels};
	AdjustWindowRectEx(&windowRectangle, windowStyle, FALSE, 0);

	// Create the window
	const auto windowHeight = static_cast<float>(windowRectangle.bottom - windowRectangle.top);
	const auto windowWidth = static_cast<float>(windowRectangle.right - windowRectangle.left);
	_hwnd = CreateWindowEx(
		0,
		L"Window",
		L"Snake",
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		static_cast<int>(ceil(windowWidth * dpiX / _defaultDpi)),
		static_cast<int>(ceil(windowHeight * dpiY / _defaultDpi)),
		nullptr,
		nullptr,
		HINST_THISCOMPONENT,
		this
	);

	auto result = _hwnd ? S_OK : E_FAIL;
	RETURN_FAILED_HRESULT(result);

	auto states = std::vector<std::shared_ptr<State::IGameState>>{
		std::make_shared<State::StartMenuState>(std::make_shared<UI::StartMenu>()),
		std::make_shared<State::Playing>(std::make_shared<UI::Score>()),
		std::make_shared<State::GameOverState>(std::make_shared<UI::Scoreboard>())
	};

	_game = std::make_unique<Game>(std::make_unique<Engine::Direct2D::Direct2dEngine>(_hwnd),
	                               std::make_unique<State::GameStateMachine>(states));

	result = _game->Initialize();
	RETURN_FAILED_HRESULT(result);

	ShowWindow(_hwnd, SW_SHOWNORMAL);
	UpdateWindow(_hwnd);

	return result;
}

void GameClient::Window::Run() const
{
	MSG message{};
	message.message = WM_NULL;
	auto nextFrame = std::chrono::steady_clock::now();
	nextFrame += std::chrono::milliseconds(1);

	// Process messages. If there are none, process a frame
	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			_game->ProcessFrame();
		}
	}
}

void GameClient::Window::OnKeyDown(const Input::Input pressedKey) const
{
	if (_game)
	{
		_game->OnInput(pressedKey);
	}
}

void GameClient::Window::OnResize(const int width, const int height) const
{
	if (_game)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called
		_game->OnResize(width, height);
	}
}

std::tuple<LRESULT, bool> GameClient::Window::OnSize(const Window& pClient, const LPARAM lParam)
{
	const int width = LOWORD(lParam);
	const int height = HIWORD(lParam);
	pClient.OnResize(width, height);

	return std::make_tuple(0, true);
}

std::tuple<LRESULT, bool> GameClient::Window::OnDisplayChange(HWND hWnd)
{
	InvalidateRect(hWnd, nullptr, false);
	return std::make_tuple(0, true);
}

std::tuple<LRESULT, bool> GameClient::Window::OnPaint(HWND hWnd)
{
	ValidateRect(hWnd, nullptr);
	return std::make_tuple(0, true);
}

std::tuple<LRESULT, bool> GameClient::Window::OnDestroy()
{
	PostQuitMessage(0);
	return std::make_tuple(0, true);
}

std::tuple<LRESULT, bool> GameClient::Window::OnKeyDown(const Window& pClient, const WPARAM wParam)
{
	// Map input messages to Input enums
	Input::Input pressedKey;
	switch (wParam)
	{
	case VK_SPACE:
		pressedKey = Input::Input::SpaceBar;
		break;
	case VK_W:
	case VK_UP:
		pressedKey = Input::Input::Up;
		break;
	case VK_S:
	case VK_DOWN:
		pressedKey = Input::Input::Down;
		break;
	case VK_A:
	case VK_LEFT:
		pressedKey = Input::Input::Left;
		break;
	case VK_D:
	case VK_RIGHT:
		pressedKey = Input::Input::Right;
		break;
	case VK_G:
		pressedKey = Input::Input::G;
		break;
	case VK_1:
	case VK_NUMPAD1:
		pressedKey = Input::Input::One;
		break;
	case VK_2:
	case VK_NUMPAD2:
		pressedKey = Input::Input::Two;
		break;
	case VK_3:
	case VK_NUMPAD3:
		pressedKey = Input::Input::Three;
		break;
	default:
		return std::make_tuple(0, false);
	}

	// Forward the pressed key to the window instance
	pClient.OnKeyDown(pressedKey);
	return std::make_tuple(0, true);
}

LRESULT CALLBACK GameClient::Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		// Associate our window instance with the window handle
		const auto pcs = std::bit_cast<LPCREATESTRUCT>(lParam);
		const auto pClient = static_cast<Window*>(pcs->lpCreateParams);

		SetWindowLongPtrW(hWnd, GWLP_USERDATA, std::bit_cast<LONG_PTR>(pClient));

		result = 0;
	}
	else
	{
		const auto pClient = std::bit_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		auto wasHandled = false;

		if (pClient)
		{
			// Process messages
			std::tuple<LRESULT, bool> ret;
			switch (message)
			{
			case WM_SIZE:
				ret = OnSize(*pClient, lParam);
				break;
			case WM_DISPLAYCHANGE:
				ret = OnDisplayChange(hWnd);
				break;
			case WM_PAINT:
				ret = OnPaint(hWnd);
				break;
			case WM_DESTROY:
				ret = OnDestroy();
				break;
			case WM_KEYDOWN:
				ret = OnKeyDown(*pClient, wParam);
				break;
			default:
				ret = std::make_tuple(0, false);
			}

			result = std::get<0>(ret);
			wasHandled = std::get<1>(ret);
		}

		if (!wasHandled)
		{
			// Forward any unhandled messages back to Windows
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return result;
}
