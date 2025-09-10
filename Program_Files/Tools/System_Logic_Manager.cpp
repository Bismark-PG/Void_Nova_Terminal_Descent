/*==============================================================================

	Manage System Logic [System_Logic_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "System_Logic_Manager.h"

void System_Initialize(HWND hWnd)
{
	// Initialize System Tools
	Game_Screen_Manager_Initialize();
	Game_Audio_Initialize();
	SystemTimer_Initialize();
	KeyLogger_Initialize();
	Mouse_Initialize(hWnd);
	Save_Data_Initialize();

	// Initialize Draw Tools
	Direct3D_Initialize(hWnd);
	Shader_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Texture_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());
	Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

	// Initialize Game Logic
	Controller_Set_Initialize();
	Fade_Initialize();
	SpriteAni_Initialize();
	Main_UI_Initialize();
	Stage_Initialize();

	// Show Mouse (True = Show // False = Don`t Show)
	Mouse_SetVisible(true);
	// Mouse_SetMode(MOUSE_POSITION_MODE_ABSOLUTE);

	ShowWindow(hWnd, CmdShow);
	UpdateWindow(hWnd);
}

void System_Finalize()
{
	// Finalize Game Logic
	Stage_Finalize();
	Main_UI_Finalize();
	SpriteAni_Finalize();
	Fade_Finalize();
	Controller_Set_Finalize();

	// Finalize Draw Tools
	Sprite_Finalize();
	Texture_Finalize();
	Shader_Finalize();
	Direct3D_Finalize();

	// Finalize System Tools
	Mouse_Finalize();
	Game_Audio_Finalize();
	Game_Screen_Manager_Finalize();
}
