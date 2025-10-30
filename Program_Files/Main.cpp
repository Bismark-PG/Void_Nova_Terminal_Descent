/*==============================================================================
	
	Project Main CPP [Update_Game.cpp]

	Project : DirectX 2D Shooting Game
	Project Name : Void Nova

	Author : Choi HyungJoon
	Date : 2025. 10. 26
	Version : 1.1.0
	Note : Alpha version complete

	Version List :
		Date : 2025. 09. 09
		Version : 1.0.0
		Note : Prototype Completed

		Date : 2025. 10. 02
		Version : 1.0.1
		Note : Refactored shader logic into Singleton module

		Date : 2025. 10. 04
		Version : 1.0.2
		Note : Refactored texture logic into Singleton module		
		
		Date : 2025. 10. 20
		Version : 1.0.3
		Note : Fixed Stage 5 boss bugs

		Date : 2025. 10. 24
		Version : 1.0.4
		Note : Ending sequence debugging and refactoring complete

==============================================================================*/
#include "Tools/Game_Header_Manager.h"
#include "Tools/System_Logic_Manager.h"

Audio_Manager*	 Sound_M = nullptr;
Shader_Manager*  Shader_M = nullptr;
Texture_Manager* Texture_M = nullptr;
Window_Manager	 Window_M;
Game_Manager	 Game_M;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR IpCmdline, _In_ int nCmdShow)
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	Handle_Instance = hInstance;
	CmdShow = nCmdShow;
	Window_M.Screen_Mode_Manager(Window_M.Get_Now_Screen_Mode());
	//Window_M.Screen_Mode_Manager(FullScreen_Mode);					// TEST
	Game_Window = Window_M.Game_Window_Create(Handle_Instance);

	System_Initialize(Game_Window);

	std::unique_ptr<Text::DebugText> Debug_T;

	// Make Debug Text (Use DX)
	auto Create_DebugText = [&]() {
		Debug_T = std::make_unique<Text::DebugText>(
			Direct3D_GetDevice(), Direct3D_GetContext(),
			Texture_M->GetID("Debug_Text"),
			Direct3D_GetBackBufferWidth(), Direct3D_GetBackBufferHeight());
		Debug_T->SetScale(0.5f);
		Debug_T->SetOffset(10.0f, 10.0f);
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
				Window_M.Set_Now_Screen_Mode(RequestedMode);

				Create_DebugText();

				// Reset Run Time
				Exec_Last_Time = SystemTimer_GetTime();
				Fps_Last_Time = Exec_Last_Time;
				continue;
			}

			if (Window_M.Is_MessageBox_Active())
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

					if (Game_M.Get_Current_Game_Select_Screen() == Game_Select_Screen::GAME_PLAYING)
						Stage_Update(Elapsed_Time);

					if (Game_M.Get_Current_Game_Select_Screen() == Game_Select_Screen::GAME_ENDING)
						Ending_Update(Elapsed_Time);
				}

				// Draw Texture
				Direct3D_Clear();
				Sprite_Begin();

				// Debug Test
				//Ending_Draw();

				// Real Draw Start
				Game_Screen_Update();
				Fade_Draw();
				Controller_Set_Draw();

				// Show FPS
#if defined(DEBUG) || defined(_DEBUG)
				std::stringstream Debug_FPS_State;
				Debug_FPS_State << "FPS : " << FPS << std::endl;
				Debug_FPS_State << "Screen  Mode : " << static_cast<int>(Window_M.Get_Now_Screen_Mode()) << "\n\n";
				Debug_FPS_State << "- State" << "\n";
				Debug_FPS_State << "Main : " << static_cast<int>(Game_M.Get_Current_Main_Screen()) << "\n";
				Debug_FPS_State << "Sub  : " << static_cast<int>(Game_M.Get_Current_Sub_Screen()) << "\n";
				Debug_FPS_State << "Game : " << static_cast<int>(Game_M.Get_Current_Game_Select_Screen()) << "\n\n";
				Debug_FPS_State << "BGM	: " << static_cast<int>(Get_BGM_Scale_Buffer()) << "\n";
				Debug_FPS_State << "SFX : " << static_cast<int>(Get_SFX_Scale_Buffer()) << "\n";
				Debug_FPS_State << "Stage : " << static_cast<int>(Get_Stage_Select_Buffer_State()) << "\n\n";
				Debug_FPS_State << "- Buffer : " << "\n";
				Debug_FPS_State << "Main    : " << static_cast<int>(Get_Main_Select_Buffer()) << "\n";
				Debug_FPS_State << "Setting : " << static_cast<int>(Get_Setting_Buffer()) << "\n";
				Debug_FPS_State << "Sound	: " << static_cast<int>(Get_Setting_State()) << "\n";
				Debug_FPS_State << "Select	: " << static_cast<int>(Get_Game_Select_Buffer()) << "\n";
				Debug_FPS_State << "Stage	: " << static_cast<int>(Get_Now_Stage_Flow()) << "\n";
				Debug_FPS_State << "Ending	: " << static_cast<int>(Get_Ending_Status()) << "\n\n";
				Debug_FPS_State << "- In Game Timer" << "\n";
				Debug_FPS_State << "State	: " << Get_In_Game_Timer() << "\n";
				Debug_FPS_State << "Stage	: " << Stage_Get_Timer() << "\n";
				Debug_FPS_State << "clear	: " << Stage_Get_Wait_Timer() << "\n";
				Debug_FPS_State << "Bomb	: " << Get_Bomb_Timer() << "\n";
				Debug_FPS_State << "Invincible	: " << Status_Get_Invincible_Time() << "\n\n";
				Debug_FPS_State << "- In Game Status" << "\n";
				Debug_FPS_State << "Live	: " << Status_Get_Lives() << "\n";
				Debug_FPS_State << "Power	: " << Status_Get_Power() << "\n";
				XMFLOAT2 POS = Player_Get_Position();
				Debug_FPS_State << "POS.X	: " << POS.x << "\n";
				Debug_FPS_State << "POS.Y	: " << POS.y << "\n";
				Debug_FPS_State << "BOMB	: " << Status_Get_Bombs() << "\n\n";
				Debug_FPS_State << "- Boss Phase" << "\n";
				Debug_FPS_State << "Stage 1	: " << static_cast<int>(Get_Stage_1_State()) << "\n";
				Debug_FPS_State << "Stage 2	: " << static_cast<int>(Get_Stage_2_State()) << "\n";
				Debug_FPS_State << "Stage 3	: " << static_cast<int>(Get_Stage_3_State()) << "\n";
				Debug_FPS_State << "Stage 4	: " << static_cast<int>(Get_Stage_4_State()) << "\n";
				Debug_FPS_State << "Stage 5	: " << static_cast<int>(Get_Stage_5_State()) << "\n";

				Debug_T->Print(Debug_FPS_State.str().c_str(), Light_Green);

				Debug_T->Draw();
				Debug_T->Clear();
#endif	
				Direct3D_Present();

				Frame_Count++;
			}	
		}
	} while (Message.message != WM_QUIT);
	
	System_Finalize();

	return (int)Message.wParam;
}