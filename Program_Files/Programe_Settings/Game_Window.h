/*==============================================================================

	Make Window [Game_Window.h]

	Author : Choi HyungJoon

==============================================================================*/
#pragma once
#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#define WIN32_LEAN_AND_MEAN
#include <sdkddkver.h>
#include <Windows.h>
#include <DirectXMath.h>
using namespace DirectX;

enum ScreenMode : int
{
	Window_Mode = 0,
	FullScreen_Mode,
	Borderless_Mode
};

extern HWND Game_Window;
extern HINSTANCE Handle_Instance;
extern int CmdShow;

extern int Original_Screen_Width;
extern int Original_Screen_Height;
extern int UI_Space_Scale;

extern int Win_Mode_Size;

extern int Player_Screen_Width;
extern int Player_Screen_Height;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern int Game_Screen_Width;
extern int Game_Screen_Height;

extern DirectX::XMFLOAT2 Game_Offset;
extern struct UI_Rect UI_Area;

extern float Game_Scale;

struct UI_Rect
{
	int Rx, Lx, y, Width, Height;
};

class Window_Manager
{
public:

	void Screen_Mode_Manager(ScreenMode Mode);
	HWND Game_Window_Create(HINSTANCE hInstance) const;
	void Game_Window_Re_Create(ScreenMode Mode, HINSTANCE hInstance);

	ScreenMode Get_Now_Screen_Mode();
	void Set_Now_Screen_Mode(ScreenMode Mode);

	bool Is_MessageBox_Active();

private:
	ScreenMode MODE = Window_Mode;
	//ScreenMode MODE = FullScreen_Mode;

	DWORD WinStyle = 0;
};

extern Window_Manager WM;

#endif // GAME_WINDOW_H