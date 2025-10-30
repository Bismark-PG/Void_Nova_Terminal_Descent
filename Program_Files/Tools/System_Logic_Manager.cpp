/*==============================================================================

	Manage System Logic [System_Logic_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "System_Logic_Manager.h"

void System_Initialize(HWND hWnd)
{
	// Initialize System Tools
	Game_Screen_Manager_Initialize();
	SystemTimer_Initialize();
	KeyLogger_Initialize();
	Mouse_Initialize(hWnd);
	Save_Data_Initialize();

	// Initialize Engine Tools
	Direct3D_Initialize(hWnd);
	Sound_M = Audio_Manager::Get_Audio_Instance();
	Sound_M->Init();

	// Initialize Draw Tools
	Shader_M = Shader_Manager::Get_Shader_Instance();
	Shader_M->Init(Direct3D_GetDevice(), Direct3D_GetContext());
	Texture_M = Texture_Manager::Get_Texture_Instance();
	Texture_M->Init(Direct3D_GetDevice(), Direct3D_GetContext());
	Sprite_Initialize(Direct3D_GetDevice(), Direct3D_GetContext());

	// Initialize Game Resource
	Game_Audio_Initialize();
	Game_Texture_Initialize();
	Debug_Initialize();
	Ending_Initialize();

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

	// Initialize Game Resource
	Ending_Finalize();
	Game_Texture_Finalize();

	Game_Audio_Finalize();
	// Finalize Draw Tools
	Sprite_Finalize();
	Texture_M->Final();
	Shader_M->Final();

	// Initialize Engine Tools
	Sound_M->UnInit();
	Direct3D_Finalize();

	// Finalize System Tools
	Mouse_Finalize();

	Game_Screen_Manager_Finalize();
}

void System_Reset_For_Ending()
{
	Stage_Finalize();
	Main_UI_Finalize();
	SpriteAni_Finalize();
	Fade_Finalize();
	Controller_Set_Finalize();

	Sound_M->Stop_BGM();
	Sound_M->Stop_All_SFX();

	Game_Screen_Manager_Initialize();

	Controller_Set_Initialize();
	Fade_Initialize();
	SpriteAni_Initialize();
	Main_UI_Initialize();
	Stage_Initialize();
	Title_Reset_For_Ending();
	Menu_Reset_For_Ending();
	Ending_Initialize();
}