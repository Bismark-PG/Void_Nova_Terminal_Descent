/*==============================================================================

	Game Main Menu [Game_Menu.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Game_Menu.h"
#include "KeyLogger.h"
#include "Controller_Input.h"
#include "Update_Screen.h"
#include "Game_Intro_Manager.h"
#include "system_timer.h"
#include "Audio_Manager.h"

static int Intro_Text_TexID = -1;
static int Intro_Text_Controller_TexID = -1;
static int Intro_Text_Draw_TexID = -1;

static int Menu_Start_TexID = -1;
static int Menu_Settings_TexID = -1;
static int Menu_Exit_TexID = -1;
static int Menu_Back_TexID = -1;

static float Alpha = 0.0f;

static bool Is_Expand_Done = false;

static bool Intro_Alpha_Done = false;
static bool Is_Intro_Glow = false;
static bool Intro_Glow_Draw = false;
static double Intro_Glow_Time = 0.0f;

static bool EXIT_STATE = false;
static bool SELECT_STATE = false;
static bool Controller_Alert = false;
static bool Wait_For_Release = false;

static double Fade_Out_Timer = 0.0;

UI_STATE State;
MENU_BUFFER Select_Buffer;

void Menu_Initialize()
{
	Intro_Text_TexID			= Texture_Load(L"Resource/Texture/BG/Title_Menu.png");
	Intro_Text_Controller_TexID = Texture_Load(L"Resource/Texture/BG/Title_Menu_Controller.png");
	Menu_Start_TexID			= Texture_Load(L"Resource/Texture/UI/Menu_Start.png");
	Menu_Settings_TexID			= Texture_Load(L"Resource/Texture/UI/Menu_Settings.png");
	Menu_Exit_TexID				= Texture_Load(L"Resource/Texture/UI/Menu_Exit.png");
	Menu_Back_TexID				= Texture_Load(L"Resource/Texture/UI/Menu_Back.png");

	if (Get_Mode_Buffer() == Get_Mode_Pre_Buffer())
	{
		State = UI_STATE::NONE;
		Select_Buffer = MENU_BUFFER::NONE;
	}

	INTRO_WIDTH  *= INTRO_SCALE;
	INTRO_HEIGHT *= INTRO_SCALE; 

	UI_WIDTH  *= UI_SCALE;
	UI_HEIGHT *= UI_SCALE;

	UI_Y = BG_HEIGHT - (BG_HEIGHT * 0.2f);

	Intro_X = (BG_WIDTH * 0.5f)  - (INTRO_WIDTH  * Game_Scale * 0.5f);
	Intro_Y = UI_Y;

	Intro_Width	 = INTRO_WIDTH  * Game_Scale;
	Intro_Height = INTRO_HEIGHT * Game_Scale;

	UI_Width  = UI_WIDTH  * Game_Scale;
	UI_Height = UI_HEIGHT * Game_Scale;

	Start_X =	 (BG_WIDTH * 0.2f) - (UI_Width * 0.5f);
	Settings_X = (BG_WIDTH * 0.5f) - (UI_Width * 0.5f);
	Exit_X =	 (BG_WIDTH * 0.8f) - (UI_Width * 0.5f);
}

void Menu_Finalize()
{
	Fade_Out_Timer = 0.0;
	Intro_Glow_Time = 0.0;

	INTRO_WIDTH  = 10;
	INTRO_HEIGHT = 1;	

	UI_WIDTH  = 5;
	UI_HEIGHT = 1;
	UI_Y = 0;

	Intro_X = 0;
	Intro_Y = 0;
	Intro_Width	 = 0;
	Intro_Height = 0;

	UI_Width	= 0;
	UI_Height	= 0;

	Start_X		= 0;
	Settings_X	= 0;
	Exit_X		= 0;
}

void Menu_Update()
{


	bool Controller_Alert_Now = Controller_Set_UP();

	if (Controller_Alert_Now)
	{
		Controller_Alert = true;
		Wait_For_Release = true;
		return;
	}

	if (Wait_For_Release)
	{
		if (KeyLogger_IsAnyKeyReleased() || XKeyLogger_IsAnyPadReleased())
			Wait_For_Release = false;
		return;
	}

	Controller_Alert = false;

	if (XKeyLogger_IsControllerInput())
		Intro_Text_Draw_TexID = Intro_Text_Controller_TexID;
	else
		Intro_Text_Draw_TexID = Intro_Text_TexID;

	switch (State)
	{
	case UI_STATE::NONE:
		if (Intro_Alpha_Done)
		{
			Is_Intro_Glow = true;
			Intro_Glow_Time = SystemTimer_GetTime();
			Update_Main_Buffer(UI_STATE::GLOW);


		}
		break;

	case UI_STATE::GLOW:
		if (KeyLogger_IsPressed(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Update_Main_Buffer(UI_STATE::MENU_UI);
			SM->Play_SFX("Buffer_Select");
		}

		if ((int)(SystemTimer_GetTime() - Intro_Glow_Time) % 2 == 0)
			Is_Intro_Glow = false;
		else if ((int)(SystemTimer_GetTime() - Intro_Glow_Time) % 2 == 1)
			Is_Intro_Glow = true;
		break;
	case UI_STATE::MENU_UI:
		switch (Select_Buffer)
		{
		case MENU_BUFFER::NONE:
			if (KeyLogger_IsTrigger(KK_A) || KeyLogger_IsTrigger(KK_LEFT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::EXIT);
				SM->Play_SFX("Buffer_Move");
			}
			else if (KeyLogger_IsTrigger(KK_D) || KeyLogger_IsTrigger(KK_RIGHT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::START);
				SM->Play_SFX("Buffer_Move");
			}
			break;

		case MENU_BUFFER::START:
			if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::DONE);
				Update_Game_Select_Buffer(SELECT_GAME::INTRO_WAIT);
				Set_Game_Intro_State(GAME_INTRO_STATE::G_INTRO_WAIT);
				SM->Play_SFX("Buffer_Select");
			}

			if (KeyLogger_IsTrigger(KK_D) || KeyLogger_IsTrigger(KK_RIGHT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::SETTINGS);
				SM->Play_SFX("Buffer_Move");
			}
			break;

		case MENU_BUFFER::SETTINGS:
			if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::DONE);
				Update_Main_Screen(Main_Screen::SELECT_SETTINGS);
				Update_Setting_Buffer(SETTING_BUFFER::SETTING_WAIT);
				SM->Play_SFX("Buffer_Select");
			}

			if (KeyLogger_IsTrigger(KK_A) || KeyLogger_IsTrigger(KK_LEFT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::START);
				SM->Play_SFX("Buffer_Move");
			}
			else if (KeyLogger_IsTrigger(KK_D) || KeyLogger_IsTrigger(KK_RIGHT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::EXIT);
				SM->Play_SFX("Buffer_Move");
			}
			break;

		case MENU_BUFFER::EXIT:
			if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::DONE);
				Fade_Start(FADE_OUT_TIME, true);
				EXIT_STATE = true;
				Fade_Out_Timer = 0.0;

				SM->Play_SFX("Buffer_Select");
			}

			if (KeyLogger_IsTrigger(KK_A) || KeyLogger_IsTrigger(KK_LEFT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				Update_Main_Select_Buffer(MENU_BUFFER::SETTINGS);
				SM->Play_SFX("Buffer_Move");
			}
			break;

		case MENU_BUFFER::DONE:
			break;
		}
		break;
	}
}

void Menu_UI_Draw()
{
	switch (State)
	{
	case UI_STATE::NONE:
		if (Alpha < A_Origin)
		{
			Sprite_Draw(Intro_Text_Draw_TexID, Intro_X, Intro_Y, Intro_Width, Intro_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Alpha });
			Alpha += Menu_Alpha_Scale;
			if (Alpha > A_Origin)
				Alpha = A_Origin;
		}
		else
		{
			Sprite_Draw(Intro_Text_Draw_TexID, Intro_X, Intro_Y, Intro_Width, Intro_Height, A_Zero);
			Intro_Alpha_Done = true;
		}	
		break;
	case UI_STATE::GLOW:
		if (Is_Intro_Glow)
			Sprite_Draw(Intro_Text_Draw_TexID, Intro_X, Intro_Y, Intro_Width, Intro_Height, A_Zero);
		else
			Sprite_Draw(Intro_Text_Draw_TexID, Intro_X, Intro_Y, Intro_Width, Intro_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
		break;
	case UI_STATE::MENU_UI:
		if (Get_Main_Select_Buffer() == MENU_BUFFER::START)
			Sprite_Draw(Menu_Start_TexID, Start_X, UI_Y, UI_Width, UI_Height, A_Zero);
		else
			Sprite_Draw(Menu_Start_TexID, Start_X, UI_Y, UI_Width, UI_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });

		if (Get_Main_Select_Buffer() == MENU_BUFFER::SETTINGS)
			Sprite_Draw(Menu_Settings_TexID, Settings_X, UI_Y, UI_Width, UI_Height, A_Zero);
		else
			Sprite_Draw(Menu_Settings_TexID, Settings_X, UI_Y, UI_Width, UI_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });

		if (Get_Main_Select_Buffer() == MENU_BUFFER::EXIT)
			Sprite_Draw(Menu_Exit_TexID, Exit_X, UI_Y, UI_Width, UI_Height, A_Zero);
		else
			Sprite_Draw(Menu_Exit_TexID, Exit_X, UI_Y, UI_Width, UI_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
		break;
	}
}

void Update_Main_Buffer(UI_STATE UI_Buffer)
{
	State = UI_Buffer;

	if (State == UI_STATE::MENU_UI)
		Update_Main_Select_Buffer(MENU_BUFFER::NONE);
}

UI_STATE Get_Main_Draw_State()
{
	return State;
}

void Update_Main_Select_Buffer(MENU_BUFFER Main_Buffer)
{
	Select_Buffer = Main_Buffer;
}

MENU_BUFFER Get_Main_Select_Buffer()
{
	return Select_Buffer;
}

void Set_Wait_For_Release(bool Release_State)
{
	Wait_For_Release = Release_State;
}

bool Get_Wait_For_Release()
{
	return Wait_For_Release;
}

bool IF_IS_Game_Done()
{
	return EXIT_STATE;
}
