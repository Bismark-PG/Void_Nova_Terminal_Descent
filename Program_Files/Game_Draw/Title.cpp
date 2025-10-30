/*==============================================================================

	Game Title [Title.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Title.h"
#include "Update_Screen.h"
#include "Setting.h"

//----------------Title Background----------------//
static int BG_ID = -1;
static int BG_Title = -1;
static int Pixel_W = -1;
static int Pixel_B = -1;

//---------------Intro Logo Texture---------------//
static int B_Logo = -1;
static int B_Name = -1;
static int UI_Title_Line = -1;
static int UI_Title_No_Line = -1;

static bool STATE = false;
static bool LOGO_STATE = false;
static bool TITLE_STATE = false;
static bool TITLE_DONE_STATE = false;
static bool TITLE_ALPHA_STATE = false;

static float Alpha = A_Origin;
static float T_Alpha = A_Zero;

float BG_WIDTH;
float BG_HEIGHT;

static bool Is_BGM_Playing = false;
static double BGM_Fade_In_Timer = 0.0;
constexpr double BGM_FADE_IN_TIME = 2.0;

static bool Is_Logo_SFX_Playing = false;
static bool Is_Logo_SFX_Fading_Out = false;

void Title_Initialize()
{
	Title_Texture();

	BG_WIDTH = (float)Direct3D_GetBackBufferWidth();
	BG_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	TITLE_WIDTH	 *= TITLE_SCALE;
	TITLE_HEIGHT *= TITLE_SCALE;

	LOGO_WIDTH  *= LOGO_SCALE;
	LOGO_HEIGHT *= LOGO_SCALE;

	NAME_WIDTH  *= NAME_SCALE;
	NAME_HEIGHT *= NAME_SCALE;
	
	Title_X = (BG_WIDTH * 0.5f)  - (TITLE_WIDTH  * Game_Scale * 0.5f);
	Title_Y = (BG_HEIGHT * 0.5f) - (TITLE_HEIGHT * Game_Scale * 0.5f);
	Title_Width  = TITLE_WIDTH  * Game_Scale;
	Title_Height = TITLE_HEIGHT * Game_Scale;

	Logo_X = (BG_WIDTH * 0.5f)  - (LOGO_WIDTH  * Game_Scale * 0.5f);
	Logo_Y = (BG_HEIGHT * 0.5f) - (LOGO_HEIGHT * Game_Scale * 0.5f);
	Logo_Width  = LOGO_WIDTH  * Game_Scale;
	Logo_Height = LOGO_HEIGHT * Game_Scale;
	
	Name_X = (BG_WIDTH * 0.5f) - (NAME_WIDTH  * Game_Scale * 0.5f);
	Name_Width  = NAME_WIDTH  * Game_Scale;
	Name_Height = NAME_HEIGHT * Game_Scale;
	Name_Y = Logo_Y + Logo_Height + (Name_Height * 0.5f);

	Pixel_X = Name_X;
	Pixel_X_Goal = Name_X + Name_Width;
	Pixel_Y = Name_Y + (Name_Height * 0.5f);
	Pixel_Width = (PIXEl_WIDTH * 0.5f) * Game_Scale;
	Pixel_Y_Origin = Pixel_Y;

	Pixel_Move = A_Zero;

	BGM_Fade_In_Timer = 0.0;
	Is_BGM_Playing = false;

	Is_Logo_SFX_Playing = false;
	Is_Logo_SFX_Fading_Out = false;
}

void Title_Finalize()
{
	Update_Main_Screen(Main_Screen::M_DONE);

	TITLE_WIDTH		= 16;
	TITLE_HEIGHT	= 9;

	LOGO_WIDTH		= 6;
	LOGO_HEIGHT		= 6;

	NAME_WIDTH		= 5;
	NAME_HEIGHT		= 1;

	Title_X	= 0;
	Title_Y	= 0;
	Title_Width	 = 0;
	Title_Height = 0;

	Logo_X	= 0;
	Logo_Y	= 0;
	Logo_Width	= 0;
	Logo_Height	= 0;

	Name_X	= 0;
	Name_Y	= 0;
	Name_Width	= 0;
	Name_Height	= 0;

	Pixel_X			= 0;
	Pixel_X_Goal	= 0;
	Pixel_Y			= 0;
	Pixel_Width		= 0;
	Pixel_Y_Origin	= 0;

	BGM_Fade_In_Timer = 0.0;
	Is_BGM_Playing = false;

	Is_Logo_SFX_Playing = false;
	Is_Logo_SFX_Fading_Out = false;
}

void Title_Update()
{
	if (Is_BGM_Playing && BGM_Fade_In_Timer < BGM_FADE_IN_TIME)
	{
		BGM_Fade_In_Timer += SystemTimer_GetElapsedTime();

        float Target_Volume = Sound_M->Get_Target_BGM_Volume();
		float Current_Volume = Target_Volume * static_cast<float>(BGM_Fade_In_Timer / BGM_FADE_IN_TIME);

		if (Current_Volume > Target_Volume)
			Current_Volume = Target_Volume;

		Sound_M->Update_Current_BGM_Volume(Current_Volume);
	}

	if (Game_M.Get_Current_Main_Screen() == Main_Screen::M_WAIT)
	{
		if (Fade_GetState() == FADE_STATE::NONE)
			Fade_Start(1.5, false);
		else if (Fade_GetState() == FADE_STATE::FINISHED_IN && !LOGO_STATE)
			LOGO_STATE = true;
		else if (Fade_GetState() == FADE_STATE::FINISHED_IN && STATE)
			Fade_Start(1.5, true);
		else if (Fade_GetState() == FADE_STATE::FINISHED_OUT && STATE)
			Update_Main_Screen(Main_Screen::MAIN);
	}
	
	if (Game_M.Get_Current_Main_Screen() == Main_Screen::MAIN)
	{
		if (TITLE_DONE_STATE)
		{
			if (!Is_BGM_Playing)
			{
				Sound_M->Play_BGM("Title", true);
				Sound_M->Update_Current_BGM_Volume(0.0f);
				Is_BGM_Playing = true;
				BGM_Fade_In_Timer = A_Zero;
			}
			Update_Main_Screen(Main_Screen::MENU_SELECT);
		}
	}
}

void Logo_Draw()
{
	Sprite_Draw(BG_ID, A_Zero, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);

	Sprite_Draw(B_Logo, Logo_X, Logo_Y, Logo_Width, Logo_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
	Sprite_Draw(B_Name, Name_X, Name_Y, Name_Width, Name_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
	Sprite_Draw(Pixel_B, Name_X + Pixel_Move, Name_Y, Name_Width, Name_Height, A_Zero);

	if (LOGO_STATE)
	{
		if (Pixel_Height < Name_Height)
		{
			if (!Is_Logo_SFX_Playing)
			{
				Sound_M->Play_SFX("Logo_Draw");
				Is_Logo_SFX_Playing = true;
			}
			
			Sprite_Draw(Pixel_W, Pixel_X, Pixel_Y, Pixel_Width, Pixel_Height, A_Zero);
			Pixel_Y -= ((Name_Height * A_Zero_Five) * (float)Game_Scale) * Pixel_Location_Limit;
			Pixel_Height += (Name_Height * A_Zero_Five) * (float)Game_Scale;
			if (Pixel_Height > Name_Height)
				Pixel_Height = Name_Height;
		}
		else
		{
			if (Pixel_X < Pixel_X_Goal)
			{
				Is_Logo_SFX_Fading_Out = true;

				Sprite_Draw(Pixel_W, Pixel_X, Pixel_Y, Pixel_Width, Pixel_Height, A_Zero);
				Pixel_X += A_Double_And_Half * Game_Scale;
				Pixel_Move += A_Double_And_Half * Game_Scale;
				if (Pixel_X > Pixel_X_Goal)
					Pixel_X = Pixel_X_Goal;
			}
			else
			{
				if (Alpha > A_Zero)
				{
					Sprite_Draw(Pixel_W, Pixel_X, Pixel_Y, Pixel_Width, Pixel_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Alpha });
					Alpha -= Title_BG_Alpha_Increase;
					if (Alpha < A_Zero)
					{
						Alpha = A_Zero;
						STATE = true;
						LOGO_STATE = false;
					}
				}
			}
		}
	}
}

void Title_BG_Blur()
{
	Sprite_Draw(BG_ID, A_Zero, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);
	if (Alpha < A_Origin)
	{
		Sprite_Draw(BG_Title, A_Zero, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero, {A_Origin, A_Origin, A_Origin, Alpha});
		Alpha += Title_BG_Alpha_Increase;
		if (Alpha > A_Origin)
			Alpha = A_Origin;
	}
	else
	{
		Sprite_Draw(BG_Title, A_Zero, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);
		TITLE_ALPHA_STATE = true;
	}
}

void Title_Logo_Draw()
{
	if (TITLE_ALPHA_STATE)
	{
		if (T_Alpha < A_Origin)
		{
			Sprite_Draw(UI_Title_No_Line, Title_X, Title_Y, Title_Width, Title_Height, A_Zero, { A_Origin, A_Origin, A_Origin, T_Alpha });
			T_Alpha += Title_Text_Alpha_Increase;
			if (T_Alpha > A_Origin)
				T_Alpha = A_Origin;
		}
		else
		{
			Sprite_Draw(UI_Title_No_Line, Title_X, Title_Y, Title_Width, Title_Height, A_Zero);

			if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
			{
				TITLE_STATE = true;
				TITLE_DONE_STATE = true;
				TITLE_ALPHA_STATE = false;
				Fade_Start(0.5, false, White);
			}
		}
	}

	if (TITLE_STATE) 
		Sprite_Draw(UI_Title_Line, Title_X, Title_Y, Title_Width, Title_Height, A_Zero);
}

void Title_Draw()
{
	Title_BG_Blur();
	Title_Logo_Draw();
}

void Title_Reset_For_Ending()
{
	STATE = false;
	LOGO_STATE = false;
	TITLE_STATE = false;
	TITLE_DONE_STATE = false;
	TITLE_ALPHA_STATE = false;

	Alpha = A_Origin;
	T_Alpha = A_Zero;

	Pixel_X = Name_X;
	Pixel_Y = Pixel_Y_Origin;
	Pixel_Move = A_Zero;
	Pixel_Height = 0.0f; 

	BGM_Fade_In_Timer = 0.0;
	Is_BGM_Playing = false;
	Is_Logo_SFX_Playing = false;
	Is_Logo_SFX_Fading_Out = false;
}


void Menu_BG()
{
	BG_X += (A_Origin * Game_Scale);
	if (BG_X >= SCREEN_WIDTH)
		BG_X = A_Zero;

	Sprite_Draw(BG_Title, BG_X, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);
	Sprite_Draw(BG_Title, BG_X - BG_WIDTH, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);
}

void Menu_Select_Draw()
{
	Menu_BG();
	Sprite_Draw(UI_Title_Line, Title_X, Title_Y, Title_Width, Title_Height, A_Zero);
}

void Title_Texture()
{
	//----------------Title Background----------------//
	BG_ID = Texture_M->GetID("K");
	BG_Title = Texture_M->GetID("BG_Title");
	Pixel_W = Texture_M->GetID("Pixel_Withe");
	Pixel_B = Texture_M->GetID("Pixel_Black");

	//---------------Intro Logo Texture---------------//
	B_Logo = Texture_M->GetID("Bismark_LOGO");
	B_Name = Texture_M->GetID("Bismark_Name");
	UI_Title_Line = Texture_M->GetID("UI_Title_Line");
	UI_Title_No_Line = Texture_M->GetID("UI_Title_No_Line");
}
