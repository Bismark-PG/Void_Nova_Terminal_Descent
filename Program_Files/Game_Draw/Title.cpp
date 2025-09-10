/*==============================================================================

	Game Title [Title.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Title.h"
#include "Update_Screen.h"
#include "Setting.h"

static int W_Pixel_TexID = -1;
static int B_Pixel_TexID = -1;
static int BG_Black_TexID = -1;
static int Logo_TexID = -1;
static int Name_TexID = -1;

static int Title_Text_TexID = -1;
static int Title_TexID = -1;
static int Title_Background_TexID = -1;

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
	W_Pixel_TexID = Texture_Load(L"Resource/Texture/Other/W_Pixel.png");
	B_Pixel_TexID = Texture_Load(L"Resource/Texture/Other/B_Pixel.png");
	BG_Black_TexID = Texture_Load(L"Resource/Texture/BG/RGBCMYK/Black.png");
	Logo_TexID = Texture_Load(L"Resource/Texture/Other/Bismark_Black.jpg");
	Name_TexID = Texture_Load(L"Resource/Texture/Other/Name_W.png");

	Title_Text_TexID = Texture_Load(L"Resource/Texture/BG/Title_Text_No_Line.png");
	Title_TexID = Texture_Load(L"Resource/Texture/BG/Main_Title_Line_Fixed.png");
	Title_Background_TexID = Texture_Load(L"Resource/Texture/BG/Main_BG_Fixed.png");

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

		int Current_Volume = static_cast<int>(SOUND_MAX * (BGM_Fade_In_Timer / BGM_FADE_IN_TIME));

		if (Current_Volume > SOUND_MAX)
			Current_Volume = SOUND_MAX;

		SM->Set_BGM_Volume(Current_Volume);
	}

	if (GM.Get_Current_Main_Screen() == Main_Screen::M_WAIT)
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
	
	if (GM.Get_Current_Main_Screen() == Main_Screen::MAIN)
	{
		if (TITLE_DONE_STATE)
		{
			if (!Is_BGM_Playing)
			{
				SM->Play_BGM("Title", true);
				SM->Set_BGM_Volume(0);
				Is_BGM_Playing = true;
				BGM_Fade_In_Timer = A_Zero;
			}
			Update_Main_Screen(Main_Screen::MENU_SELECT);
		}
	}
}

void Logo_Draw()
{
	Sprite_Draw(BG_Black_TexID, A_Zero, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);

	Sprite_Draw(Logo_TexID, Logo_X, Logo_Y, Logo_Width, Logo_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
	Sprite_Draw(Name_TexID, Name_X, Name_Y, Name_Width, Name_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
	Sprite_Draw(B_Pixel_TexID, Name_X + Pixel_Move, Name_Y, Name_Width, Name_Height, A_Zero);

	if (LOGO_STATE)
	{
		if (Pixel_Height < Name_Height)
		{
			if (!Is_Logo_SFX_Playing)
			{
				SM->Play_SFX("Logo_Draw");
				Is_Logo_SFX_Playing = true;
			}
			
			Sprite_Draw(W_Pixel_TexID, Pixel_X, Pixel_Y, Pixel_Width, Pixel_Height, A_Zero);
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

				Sprite_Draw(W_Pixel_TexID, Pixel_X, Pixel_Y, Pixel_Width, Pixel_Height, A_Zero);
				Pixel_X += A_Double_And_Half * Game_Scale;
				Pixel_Move += A_Double_And_Half * Game_Scale;
				if (Pixel_X > Pixel_X_Goal)
					Pixel_X = Pixel_X_Goal;
			}
			else
			{
				if (Alpha > A_Zero)
				{
					Sprite_Draw(W_Pixel_TexID, Pixel_X, Pixel_Y, Pixel_Width, Pixel_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Alpha });
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
	Sprite_Draw(BG_Black_TexID, A_Zero, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);
	if (Alpha < A_Origin)
	{
		Sprite_Draw(Title_Background_TexID, A_Zero, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero, { A_Origin, A_Origin, A_Origin, Alpha });
		Alpha += Title_BG_Alpha_Increase;
		if (Alpha > A_Origin)
			Alpha = A_Origin;
	}
	else
	{
		Sprite_Draw(Title_Background_TexID, A_Zero, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);
		TITLE_ALPHA_STATE = true;
	}
}

void Title_Logo_Draw()
{
	if (TITLE_ALPHA_STATE)
	{
		if (T_Alpha < A_Origin)
		{
			Sprite_Draw(Title_Text_TexID, Title_X, Title_Y, Title_Width, Title_Height, A_Zero, { A_Origin, A_Origin, A_Origin, T_Alpha });
			T_Alpha += Title_Text_Alpha_Increase;
			if (T_Alpha > A_Origin)
				T_Alpha = A_Origin;
		}
		else
		{
			Sprite_Draw(Title_Text_TexID, Title_X, Title_Y, Title_Width, Title_Height, A_Zero);

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
		Sprite_Draw(Title_TexID, Title_X, Title_Y, Title_Width, Title_Height, A_Zero);

}

void Title_Draw()
{
	Title_BG_Blur();
	Title_Logo_Draw();
}


void Menu_BG()
{
	BG_X += (A_Origin * Game_Scale);
	if (BG_X >= SCREEN_WIDTH)
		BG_X = A_Zero;

	Sprite_Draw(Title_Background_TexID, BG_X, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);
	Sprite_Draw(Title_Background_TexID, BG_X - BG_WIDTH, A_Zero, BG_WIDTH, BG_HEIGHT, A_Zero);
}

void Menu_Select_Draw()
{
	Menu_BG();
	Sprite_Draw(Title_TexID, Title_X, Title_Y, Title_Width, Title_Height, A_Zero);
}