/*==============================================================================
	
	Project Main CPP [Update_Game.cpp]

	Project : DirectX 2D Shooting Game
	Project Name : Void Nova

	Author : Choi HyungJoon
	Date : 2025. 09. 09
	Version : 1.0.0

==============================================================================*/
#include "Tools/Game_Header_Manager.h"
#include "Tools/System_Logic_Manager.h"

Audio_Manager* SM = nullptr;
Window_Manager WM;
Game_Manager GM;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR IpCmdline, _In_ int nCmdShow)
{
	SM = Audio_Manager::Get_Audio_Instance();

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	Handle_Instance = hInstance;
	CmdShow = nCmdShow;
	WM.Screen_Mode_Manager(WM.Get_Now_Screen_Mode());	
	//WM.Screen_Mode_Manager(FullScreen_Mode);					// TEST
	Game_Window = WM.Game_Window_Create(Handle_Instance);

	System_Initialize(Game_Window);

	std::unique_ptr<Text::DebugText> Debug_T;

	// Make Debug Text (Use DX)
	auto Create_DebugText = [&]() {
		Debug_T = std::make_unique<Text::DebugText>(
			Direct3D_GetDevice(), Direct3D_GetContext(),
			L"Resource/Texture/Other/Console_ASCII_512.png",
			Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight(),
			0.f, 0.f, 0, 0, 0.f, 0.f);
		Debug_T->SetScale(0.75f);
		};

	Create_DebugText();

	// Set Frame
	double Exec_Last_Time = SystemTimer_GetTime();
	double Fps_Last_Time = Exec_Last_Time;
	double Current_Time = 0.0;
	ULONG Frame_Count = 0;
	double FPS = 0.0; // Frame Per Second
	bool IS_Controller_Set = false;

	MSG Message = {};  // Make Game Loop & Message Loop

	if (FAILED(hr))
	{
		MessageBoxW(nullptr, L"Failed To Set Function In Main.cpp : [CoInitializeEx]", L">> FATAL ERROR <<", MB_OK | MB_ICONERROR);
		return (int)Message.wParam;;
	}

	do
	{
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		
		// Set Game
		else
		{
			// Window Re-Create
			ScreenMode RequestedMode = Screen_Mode_Change_Request();
			if ((int)RequestedMode != -1) 
			{
				// Memory Reset
				Debug_T.reset();

				Window_Re_Create(RequestedMode);
				WM.Set_Now_Screen_Mode(RequestedMode);

				Create_DebugText();

				// Reset Run Time
				Exec_Last_Time = SystemTimer_GetTime();
				Fps_Last_Time = Exec_Last_Time;
				continue;
			}

			if (WM.Is_MessageBox_Active())
				continue;

			if (IF_IS_Game_Done())
			{
				// Game Done (EXIT)
				if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
					PostQuitMessage(0);
			}

			// Time Set
			Current_Time = SystemTimer_GetTime(); // Get System Time
			double Elapsed_Time = Current_Time - Fps_Last_Time; // Get Time For FPS

			//  Set FPS
			if (Elapsed_Time >= 1.0)
			{
				FPS = Frame_Count / Elapsed_Time;
				Fps_Last_Time = Current_Time; // Save FPS
				Frame_Count = 0; // Clear Count
			}

			// Set Elapsed Time
			Elapsed_Time = Current_Time - Exec_Last_Time; // If you want to limit FPS, set condition here

			if (Elapsed_Time >= (1.0 / 60.0)) // 60 FPS
			{
				Exec_Last_Time = Current_Time; // Save Last Time

				Fade_Update(Elapsed_Time);

				// Set Key Logger With FPS
				KeyLogger_Update();
				Controller_Set_Update();
				IS_Controller_Set = Controller_Set_UP();

				// Update Game Texture
				if (IS_Controller_Set)
				{
					SpriteAni_Update(Elapsed_Time);
				}
				else if (!IS_Controller_Set)
				{
					Main_UI_Update(Elapsed_Time);
					SpriteAni_Update(Elapsed_Time);

					if (GM.Get_Current_Game_Select_Screen() == Game_Select_Screen::GAME_PLAYING)
						Stage_Update(Elapsed_Time);
					else if (GM.Get_Current_Game_Select_Screen() == Game_Select_Screen::GAME_ENDING)
						Ending_Update(Elapsed_Time);
				}
				
				// Draw Texture
				Direct3D_Clear();
				Sprite_Begin();

				// Debug Test
				//Debug_BG_Draw();
				//Stage_5_Background_Draw();
				//Game_UI_And_Logo_Draw();

				// Real Draw Start
				Game_Screen_Update();
				Fade_Draw();
				Controller_Set_Draw();

				// Show FPS

				Direct3D_Present();


				Frame_Count++;
			}	
		}
	} while (Message.message != WM_QUIT);
	
	System_Finalize();

	return (int)Message.wParam;
}