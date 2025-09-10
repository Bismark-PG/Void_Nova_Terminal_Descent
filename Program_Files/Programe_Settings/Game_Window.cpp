/*==============================================================================

	Make Window [Game_Window.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Game_Window.h"
#include "direct3d.h"
#include <algorithm>
#include "KeyLogger.h"
#include "mouse.h"
#include <sstream>
#include "Palette.h"

using namespace PALETTE;

int Original_Screen_Width = 3;
int Original_Screen_Height = 4;
int UI_Space_Scale = 1;

int Win_Mode_Size = 200;

int Player_Screen_Width = GetSystemMetrics(SM_CXSCREEN);
int Player_Screen_Height = GetSystemMetrics(SM_CYSCREEN);

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
int Game_Screen_Width = 0;
int Game_Screen_Height = 0;

XMFLOAT2 Game_Offset = {};
UI_Rect UI_Area = {};

float Game_Scale = 1;

static constexpr wchar_t WINDOW_CLASS[] = L"GameWindow";
static constexpr wchar_t TITLE[] = L"Void Nova";

LRESULT CALLBACK Wnd_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND Game_Window;
HINSTANCE Handle_Instance;
int CmdShow;

static bool IF_IS_Recreating = false;
static bool Is_Message_Box_Open = false;

void Window_Manager::Screen_Mode_Manager(ScreenMode Mode)
{
	std::stringstream Debug;
	Debug << "Switching to Mode: " << Mode << "\n";

	if (Mode == FullScreen_Mode || Mode == Borderless_Mode)
	{
		SCREEN_WIDTH = Player_Screen_Width;
		SCREEN_HEIGHT = Player_Screen_Height;

		Game_Screen_Width = (SCREEN_HEIGHT / Original_Screen_Height) * Original_Screen_Width;
		Game_Screen_Height = SCREEN_HEIGHT;

		Game_Offset.x = (SCREEN_WIDTH - Game_Screen_Width) * A_Half;
		Game_Offset.y = A_Zero;

		UI_Area.Width = (SCREEN_HEIGHT / Original_Screen_Height) * UI_Space_Scale;
		UI_Area.Height = SCREEN_HEIGHT;
		UI_Area.Rx = static_cast<int>(Game_Offset.x) + Game_Screen_Width;
		UI_Area.Lx = static_cast<int>(Game_Offset.x) - UI_Area.Width;
		UI_Area.y = 0;

		Game_Scale = static_cast<float>(SCREEN_WIDTH) / (Game_Screen_Width + (UI_Area.Width * 2));

		switch (Mode)
		{
		case 1: // FullScreen_Mode
			WinStyle = WS_POPUP;
			break;
		case 2: // Borderless_Mode
			WinStyle = WS_POPUP | WS_VISIBLE;
			break;
		}
	}
	else
	{
		Game_Screen_Height = Original_Screen_Height * Win_Mode_Size;
		Game_Screen_Width = Original_Screen_Width * Win_Mode_Size;
		
		UI_Area.Width = (Game_Screen_Height / Original_Screen_Height) * UI_Space_Scale;
		UI_Area.Height = Game_Screen_Height;
		UI_Area.Rx = Game_Screen_Width + UI_Area.Width;
		UI_Area.Lx = 0;
		UI_Area.y  = 0;
		
		Game_Offset.x = (float)UI_Area.Width;
		Game_Offset.y = A_Zero;
		
		SCREEN_WIDTH = Game_Screen_Width + (UI_Area.Width * 2);
		SCREEN_HEIGHT = Game_Screen_Height;

		Game_Scale = static_cast<float>(SCREEN_WIDTH) / (Game_Screen_Width + (UI_Area.Width * 2));

		WinStyle = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);
	}

	if (SCREEN_WIDTH <= 0 || SCREEN_HEIGHT <= 0)
	{
		Debug << "Error : Invalid screen dimensions." << std::endl;
		return;
	}
	if (Game_Screen_Width <= 0 || Game_Screen_Height <= 0)
	{
		Debug << "Error : Invalid game screen dimensions." << std::endl;
		return;
	}
	if (Game_Screen_Width > SCREEN_WIDTH || Game_Screen_Height > SCREEN_HEIGHT)
	{
		Debug << "Error : Game screen is bigger Main Screen." << std::endl;
		return;
	}
	if (UI_Area.Rx + UI_Area.Width > SCREEN_WIDTH)
	{
		Debug << "Warning : UI area is bigger Main Screen width. UI may be clipped." << std::endl;
	}

	OutputDebugStringA(Debug.str().c_str());
}

HWND Window_Manager::Game_Window_Create(HINSTANCE hInstance) const
{
	static bool Window_Class_Registered = false;
	if (!Window_Class_Registered)
	{
		WNDCLASSEX WCex{};
		WCex.cbSize = sizeof(WNDCLASSEX);
		WCex.lpfnWndProc = Wnd_Proc;
		WCex.hInstance = hInstance;
		WCex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		WCex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WCex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		WCex.lpszMenuName = nullptr;
		WCex.lpszClassName = WINDOW_CLASS;
		WCex.hIconSm = LoadIcon(WCex.hInstance, IDI_APPLICATION);

		RegisterClassEx(&WCex);
		Window_Class_Registered = true;
	}

	RECT Window_Rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	AdjustWindowRect(&Window_Rect, WinStyle, FALSE);

	const int WINDOW_WIDTH = Window_Rect.right - Window_Rect.left;
	const int WINDOW_HEIGTH = Window_Rect.bottom - Window_Rect.top;

	const int windowX = std::max((Player_Screen_Width - WINDOW_WIDTH) / 2, 0);
	const int windowY = std::max((Player_Screen_Height - WINDOW_HEIGTH) / 2, 0);

	HWND hWnd = CreateWindow(WINDOW_CLASS,
		TITLE,
		WinStyle,
		windowX,
		windowY,
		WINDOW_WIDTH,
		WINDOW_HEIGTH,
		nullptr, nullptr, hInstance, nullptr);

	return hWnd;
}

void Window_Manager::Game_Window_Re_Create(ScreenMode Mode, HINSTANCE hInstance)
{
	std::stringstream Debug;
	Debug << "[Window_Manager] Re-creating game window...Switching to Mode : " << Mode << "\n";

	if (IF_IS_Recreating)
	{
		if (MessageBox(Game_Window, L"Window is Already Recreating", L"*>>-FETAL_WARNING-<<*", MB_OK) == IDOK)
			PostQuitMessage(0);
	}
	else
		IF_IS_Recreating = true;

	if (Game_Window && IsWindow(Game_Window))
	{
		DestroyWindow(Game_Window);
		Game_Window = nullptr;
	}

	Screen_Mode_Manager(Mode);

	Game_Window = Game_Window_Create(hInstance);

	if (Game_Window)
		Debug << "[Window_Manager] New window created successfully. / Mode : " << Mode << "\n";
	else
		Debug << "[Window_Manager] Failed to create new window! / Mode : " << Mode << "\n";

	OutputDebugStringA(Debug.str().c_str());

	IF_IS_Recreating = false;
}

ScreenMode Window_Manager::Get_Now_Screen_Mode()
{
	return WM.MODE;
}

void Window_Manager::Set_Now_Screen_Mode(ScreenMode Mode)
{
	WM.MODE = Mode;
}
bool Window_Manager::Is_MessageBox_Active()
{
	return Is_Message_Box_Open;
}
LRESULT CALLBACK Wnd_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		Keyboard_ProcessMessage(message, wParam, lParam);
		Mouse_ProcessMessage(message, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	    Mouse_ProcessMessage(message, wParam, lParam);
	    break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	    Keyboard_ProcessMessage(message, wParam, lParam);
	    break;

	case WM_CLOSE:
		Is_Message_Box_Open = true;

		if (MessageBox(hWnd, L"Really Want Exit Game?", L"WARNING", MB_OKCANCEL) == IDOK)
			DestroyWindow(hWnd);

		Is_Message_Box_Open = false;
		break;

	case WM_DESTROY:
		if (!IF_IS_Recreating)
			PostQuitMessage(0);
		break;

	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED)
		{
			UINT Width = LOWORD(lParam);
			UINT Height = HIWORD(lParam);
			if (Width > 0 && Height > 0)
				Direct3D_Re_Size(Width, Height);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}