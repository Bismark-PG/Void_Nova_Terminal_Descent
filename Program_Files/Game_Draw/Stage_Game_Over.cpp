/*==============================================================================

	Game Over Draw [Stage_Game_Over.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Stage_Game_Over.h"

#include "Stage_Select.h"
#include "Game_Window.h"
#include "Sprite_Animation.h"
#include "KeyLogger.h"
#include "Palette.h"
#include "Audio_Manager.h"
#include "Score.h"

using namespace PALETTE;

static int Game_Over_BG_TexID = -1;

static int Operation_TexID = -1;
static int Failed_TexID = -1;
static int Game_Over_JP_TexID = -1;

static int Retry_TexID = -1;
static int Retry_JP_TexID = -1;

static int Go_To_Main_TexID = -1;
static int Go_To_Main_JP_TexID = -1;

static int Complete_TexID = -1;
static int Complete_JP_TexID = -1;

static float BG_Alpha = 0.0f;
static float UI_Alpha = 0.0f;
static float Game_Over_Menu_Alpha = 0.0f;
static float Stage_Clear_Alpha = 1.0f;
static float Stage_Score_Alpha = 0.0f;
static float Fade_Out_Wait_Timer = 0.0f;

static bool Is_Failed_Sound_Played = false;
static bool Is_BG_Fade_In_Done = false;
static bool Is_Title_Anime_Done = false;
static bool Is_Game_Over_Sound_Played = false;
static bool Is_Paused = false;

STAGE_OUTRO_STATE		Outro_State = STAGE_OUTRO_STATE::IN_ACTIVE;
GAME_OVER_MENU_BUFFER   Game_Over_Buffer_Input = GAME_OVER_MENU_BUFFER::NONE;
GAME_OVER_MENU_SELCETED Game_Over_Selected = GAME_OVER_MENU_SELCETED::NONE;

void Game_Over_Initialize()
{
	Game_Over_BG_TexID	= Texture_Load(L"Resource/Texture/Other/B_Pixel.png");

	Operation_TexID		= Texture_Load(L"Resource/Texture/UI/UI_Operation.png");
	Failed_TexID		= Texture_Load(L"Resource/Texture/UI/UI_Failed.png");
	Game_Over_JP_TexID	= Texture_Load(L"Resource/Texture/UI/UI_Failed_JP.png");

	Retry_TexID			= Texture_Load(L"Resource/Texture/UI/Menu_Return.png");
	Retry_JP_TexID		= Texture_Load(L"Resource/Texture/UI/Menu_Return_JP.png");

	Go_To_Main_TexID	= Texture_Load(L"Resource/Texture/UI/Menu_Abort.png");
	Go_To_Main_JP_TexID = Texture_Load(L"Resource/Texture/UI/Menu_Abort_JP.png");

	Complete_TexID	    = Texture_Load(L"Resource/Texture/UI/UI_Complete.png");
	Complete_JP_TexID   = Texture_Load(L"Resource/Texture/UI/UI_Complete_JP.png");

	Stage_Over_Draw_Reset();

	BG_A_Width  = static_cast<float>(SCREEN_WIDTH);
	BG_A_Height = static_cast<float>(SCREEN_HEIGHT);

	Game_Over_Title_Width  = Game_Screen_Width  * A_Half;
	Game_Over_Title_Height = Game_Screen_Height * A_Z_T_Quarters;
	Game_Over_Title_X = Game_Offset.x + (Game_Screen_Width * A_Half) - (Game_Over_Title_Width * A_Half);
	Game_Over_Title_Y = (Game_Screen_Height * A_Quarter) - (Game_Over_Title_Height * A_Half);

	Game_Over_Status_Width  = Game_Over_Title_Width * A_F_Fifths;
	Game_Over_Status_Height = Game_Screen_Height * A_Zero_Five;
	Game_Over_Status_X = Game_Offset.x + (Game_Screen_Width * A_Half) - (Game_Over_Status_Width * A_Half);
	Game_Over_Status_Y = Game_Over_Title_Y + Game_Over_Title_Height;

	Game_Over_JP_Width  = Game_Over_Status_Width * A_Half;
	Game_Over_JP_Height = Game_Over_Status_Height * A_F_Fifths;
	Game_Over_JP_X = Game_Offset.x + (Game_Screen_Width * A_Half) - (Game_Over_JP_Width * A_Half);
	Game_Over_JP_Y = Game_Over_Title_Y - Game_Over_JP_Height;

	Game_Over_UI_Width  = Game_Over_Status_Width * A_Half;
	Game_Over_UI_Height = Game_Over_Title_Height * A_Half;
	Game_Over_UI_X = Game_Offset.x + (Game_Screen_Width * A_Half) - (Game_Over_UI_Width * A_Half);
	Game_Over_UI_JP_X = Game_Over_UI_X + (Game_Over_UI_Width * A_Half);

	Retry_Y			= (Game_Screen_Height * A_Six_Tenths) - (Game_Over_UI_Height * A_Half);
	Retry_JP_Y		= Retry_Y - (Game_Over_UI_Height * A_Half);
	Go_To_Main_Y	= (Game_Screen_Height * A_F_Fifths) - (Game_Over_UI_Height * A_Half);
	Go_To_Main_JP_Y = Go_To_Main_Y - (Game_Over_UI_Height * A_Half);


	Clear_Score_Width  = (Game_Screen_Width * A_Six_Tenths) * A_One_Tenth;
	Clear_Score_Height = Clear_Score_Width * A_One_And_Half;
	float Total_Score_Width = Clear_Score_Width * SCORE_DIGITS;

	Clear_Score_X = Game_Offset.x + (Game_Screen_Width * A_Half) - (Total_Score_Width * A_Half);
	Clear_Score_Y = (Game_Screen_Height * A_Half) - (Clear_Score_Height * A_Half);

	Is_Paused = false;
}

void Game_Over_Finalize()
{
	Stage_Over_Draw_Reset();

	BG_A_Width  = 0;
	BG_A_Height = 0;

	Game_Over_Title_X = 0;
	Game_Over_Title_Y = 0;
	Game_Over_Title_Width  = 0;
	Game_Over_Title_Height = 0;

	Game_Over_Status_X = 0;
	Game_Over_Status_Y = 0;
	Game_Over_Status_Width  = 0;
	Game_Over_Status_Height = 0;

	Game_Over_JP_X = 0;
	Game_Over_JP_Y = 0;
	Game_Over_JP_Width  = 0;
	Game_Over_JP_Height = 0;

	Game_Over_UI_X = 0;
	Game_Over_UI_JP_X = 0;
	Game_Over_UI_Width  = 0;
	Game_Over_UI_Height = 0;

	Retry_Y = 0;
	Retry_JP_Y = 0;
	Go_To_Main_Y = 0;
	Go_To_Main_JP_Y = 0;

	Clear_Score_X = 0;
	Clear_Score_Y = 0;
	Clear_Score_Width  = 0;
	Clear_Score_Height = 0;
}

void Game_Over_Update()
{
	if (Is_Failed_Sound_Played)
	{
		SM->Play_SFX("Stage_Failed");
		Is_Failed_Sound_Played = false;
	}

	if (Is_Title_Anime_Done || Get_Now_Stage_Flow() == STAGE_FLOW_STATE::GAME_PAUSED)
	{
		switch (Get_Game_Over_Input())
		{
		case GAME_OVER_MENU_BUFFER::PLAYER_DEAD:
			if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
			{
				Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::GO_MAIN);
				SM->Play_SFX("Buffer_Move");
			}
			else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::RETRY);
				SM->Play_SFX("Buffer_Move");
			}
			break;

		case GAME_OVER_MENU_BUFFER::RETRY:
			if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
			{
				Set_Game_Over_Menu_Selected(GAME_OVER_MENU_SELCETED::RE_START);
				Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::DONE);
				SM->Play_SFX("Buffer_Select");
			}

			if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::GO_MAIN);
				SM->Play_SFX("Buffer_Move");
			}
			break;

		case GAME_OVER_MENU_BUFFER::GO_MAIN:
			if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
			{
				Set_Game_Over_Menu_Selected(GAME_OVER_MENU_SELCETED::GO_TO_MAIN);
				Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::DONE);
				SM->Play_SFX("Buffer_Back");
			}

			if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
			{
				Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::RETRY);
				SM->Play_SFX("Buffer_Move");
			}
			break;

		case GAME_OVER_MENU_BUFFER::DONE:
			break;
		}
	}
}

void Game_Over_Draw()
{
	if (Is_Paused)
		Game_Paused_Draw();
	else
	{
		if (!Is_Game_Over_Sound_Played)
			Is_Game_Over_Sound_Played = true;

		Stage_Done_BG_Draw();

		if (Is_BG_Fade_In_Done)
			Stage_Failed_UI_Draw();

		if (Is_BG_Fade_In_Done && Is_Title_Anime_Done)
		{
			if (Game_Over_Menu_Alpha < A_Half)
			{
				Sprite_Draw(Retry_JP_TexID, Game_Over_UI_JP_X, Retry_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, Game_Over_Menu_Alpha });
				Sprite_Draw(Go_To_Main_JP_TexID, Game_Over_UI_JP_X, Go_To_Main_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, Game_Over_Menu_Alpha });
				Sprite_Draw(Retry_TexID, Game_Over_UI_X, Retry_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, Game_Over_Menu_Alpha });
				Sprite_Draw(Go_To_Main_TexID, Game_Over_UI_X, Go_To_Main_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, Game_Over_Menu_Alpha });

				Game_Over_Menu_Alpha += Menu_Alpha_Increase;

				if (Game_Over_Menu_Alpha >= A_Half)
					Game_Over_Menu_Alpha = A_Half;
			}
			else
			{
				if (Get_Game_Over_Input() == GAME_OVER_MENU_BUFFER::RETRY)
				{
					Sprite_Draw(Retry_JP_TexID, Game_Over_UI_JP_X, Retry_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f);
					Sprite_Draw(Retry_TexID, Game_Over_UI_X, Retry_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f);
				}
				else
				{
					Sprite_Draw(Retry_JP_TexID, Game_Over_UI_JP_X, Retry_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
					Sprite_Draw(Retry_TexID, Game_Over_UI_X, Retry_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
				}

				if (Get_Game_Over_Input() == GAME_OVER_MENU_BUFFER::GO_MAIN)
				{
					Sprite_Draw(Go_To_Main_JP_TexID, Game_Over_UI_JP_X, Go_To_Main_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f);
					Sprite_Draw(Go_To_Main_TexID, Game_Over_UI_X, Go_To_Main_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f);
				}
				else
				{
					Sprite_Draw(Go_To_Main_JP_TexID, Game_Over_UI_JP_X, Go_To_Main_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
					Sprite_Draw(Go_To_Main_TexID, Game_Over_UI_X, Go_To_Main_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

				}
			}
		}
	}
}

void Stage_Over_Draw_Reset()
{
	UI_Alpha = A_Zero;
	BG_Alpha = A_Zero;
	Game_Over_Menu_Alpha = A_Zero;

	Is_BG_Fade_In_Done = false;
	Is_Title_Anime_Done = false;
	Is_Failed_Sound_Played = false;

	Is_Game_Over_Sound_Played = false;

	Game_Over_Buffer_Input = GAME_OVER_MENU_BUFFER::NONE;
	Game_Over_Selected = GAME_OVER_MENU_SELCETED::NONE;
}

void Game_Paused_Draw()
{
	Sprite_Draw(Game_Over_BG_TexID, 0.f, 0.f, BG_A_Width, BG_A_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

	if (Get_Game_Over_Input() == GAME_OVER_MENU_BUFFER::RETRY)
	{
		Sprite_Draw(Retry_JP_TexID, Game_Over_UI_JP_X, Retry_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f);
		Sprite_Draw(Retry_TexID, Game_Over_UI_X, Retry_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f);
	}
	else
	{
		Sprite_Draw(Retry_JP_TexID, Game_Over_UI_JP_X, Retry_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
		Sprite_Draw(Retry_TexID, Game_Over_UI_X, Retry_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
	}

	if (Get_Game_Over_Input() == GAME_OVER_MENU_BUFFER::GO_MAIN)
	{
		Sprite_Draw(Go_To_Main_JP_TexID, Game_Over_UI_JP_X, Go_To_Main_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f);
		Sprite_Draw(Go_To_Main_TexID, Game_Over_UI_X, Go_To_Main_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f);
	}
	else
	{
		Sprite_Draw(Go_To_Main_JP_TexID, Game_Over_UI_JP_X, Go_To_Main_JP_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
		Sprite_Draw(Go_To_Main_TexID, Game_Over_UI_X, Go_To_Main_Y, Game_Over_UI_Width, Game_Over_UI_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

	}
}

void Set_Is_Paused_Menu(bool is_paused)
{
	Is_Paused = is_paused;
}

bool Is_Paused_Menu()
{
	return Is_Paused;
}

void Stage_Outro_Start()
{
	if (Outro_State != STAGE_OUTRO_STATE::IN_ACTIVE && Outro_State != STAGE_OUTRO_STATE::FINISHED) return;

	BG_Alpha = A_Zero;
	UI_Alpha = A_Zero;
	Stage_Clear_Alpha = A_Origin;
	Stage_Score_Alpha = A_Zero;
	Fade_Out_Wait_Timer = A_Zero;
	Score_Outro_Reset();

	Outro_State = STAGE_OUTRO_STATE::BG_FADE_IN;
}

void Stage_Outro_Update(double elapsed_time)
{
	if (Outro_State == STAGE_OUTRO_STATE::IN_ACTIVE || Outro_State == STAGE_OUTRO_STATE::FINISHED) return;

	float Time = static_cast<float>(elapsed_time);

	switch (Outro_State)
	{
	case STAGE_OUTRO_STATE::BG_FADE_IN:
		BG_Alpha += BG_Alpha_Increase;

		if (BG_Alpha >= A_Half)
		{
			BG_Alpha = A_Half;
			SM->Play_SFX("Stage_Show_Score");
			Outro_State = STAGE_OUTRO_STATE::UI_FADE_IN;
		}
		break;

	case STAGE_OUTRO_STATE::UI_FADE_IN:
		UI_Alpha += UI_Alpha_Increase;

		if (UI_Alpha >= A_Origin)
		{
			UI_Alpha = A_Origin;
			SM->Play_SFX("Stage_Score_Up");
			Outro_State = STAGE_OUTRO_STATE::SCORE_COUNT;
		}
		break;

	case STAGE_OUTRO_STATE::SCORE_COUNT:
		Score_Outro_Update(elapsed_time);

		if (Score_Is_Increase_Done())
			Outro_State = STAGE_OUTRO_STATE::WAIT_BEFORE_FADE;
		break;

	case STAGE_OUTRO_STATE::WAIT_BEFORE_FADE:
		Fade_Out_Wait_Timer += Time;

		if (Fade_Out_Wait_Timer > 2.0f)
			Outro_State = STAGE_OUTRO_STATE::ALL_FADE_OUT;
		break;

	case STAGE_OUTRO_STATE::ALL_FADE_OUT:
		Stage_Clear_Alpha -= Clear_Alpha_Increase;
		Stage_Score_Alpha -= Score_Alpha_Increase;
		BG_Alpha -= BG_Alpha_Increase;

		if (Stage_Clear_Alpha <= A_Zero)
		{
			Stage_Clear_Alpha = A_Zero;
			Stage_Score_Alpha = A_Zero;
			BG_Alpha = A_Zero;
			Outro_State = STAGE_OUTRO_STATE::FINISHED;
		}
		break;

	case STAGE_OUTRO_STATE::IN_ACTIVE:
	case STAGE_OUTRO_STATE::FINISHED:
		break;
	}
}

void Stage_Outro_Draw()
{
	if (Outro_State == STAGE_OUTRO_STATE::IN_ACTIVE) return;

	Sprite_Draw(Game_Over_BG_TexID, 0.f, 0.f, BG_A_Width, BG_A_Height, 0.f, { A_Origin, A_Origin, A_Origin, BG_Alpha });

	if (Outro_State >= STAGE_OUTRO_STATE::UI_FADE_IN)
	{
		float Current_UI_Alpha = (Outro_State == STAGE_OUTRO_STATE::ALL_FADE_OUT) ? Stage_Clear_Alpha : UI_Alpha;

		Sprite_Draw(Operation_TexID, Game_Over_Title_X, Game_Over_Title_Y, Game_Over_Title_Width, Game_Over_Title_Height, 0.f, { A_Origin, A_Origin, A_Origin, Current_UI_Alpha });
		Sprite_Draw(Complete_TexID, Game_Over_Status_X, Game_Over_Status_Y, Game_Over_Status_Width, Game_Over_Status_Height, 0.f, { A_Origin, A_Origin, A_Origin, Current_UI_Alpha });
		Sprite_Draw(Complete_JP_TexID, Game_Over_JP_X, Game_Over_JP_Y, Game_Over_JP_Width, Game_Over_JP_Height, 0.f, { A_Origin, A_Origin, A_Origin, Current_UI_Alpha });
	}

	if (Outro_State >= STAGE_OUTRO_STATE::SCORE_COUNT)
	{
		if (Outro_State == STAGE_OUTRO_STATE::SCORE_COUNT && Stage_Score_Alpha < A_T_Quarters)
		{
			Stage_Score_Alpha += Score_Alpha_Increase;

			if (Stage_Score_Alpha > A_T_Quarters)
				Stage_Score_Alpha = A_T_Quarters;
		}

		float Current_Score_Alpha = (Outro_State == STAGE_OUTRO_STATE::ALL_FADE_OUT) ? Stage_Clear_Alpha : Stage_Score_Alpha;
		Score_End_Score_Draw(Clear_Score_X, Clear_Score_Y, Clear_Score_Width, Clear_Score_Height, Current_Score_Alpha);
	}
}

bool Is_Stage_Outro_Finished()
{
	return Outro_State == STAGE_OUTRO_STATE::FINISHED;
}

void Stage_Done_BG_Draw()
{
	if (BG_Alpha < A_Half)
	{
		Sprite_Draw(Game_Over_BG_TexID, 0.f, 0.f, BG_A_Width, BG_A_Height, 0.f, { A_Origin, A_Origin, A_Origin, BG_Alpha });
		BG_Alpha += BG_Alpha_Increase;

		if (BG_Alpha >= A_Half)
			BG_Alpha = A_Half;
	}
	else
	{
		Sprite_Draw(Game_Over_BG_TexID, 0.f, 0.f, BG_A_Width, BG_A_Height, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
		Is_BG_Fade_In_Done = true;
	}
}

void Stage_Failed_UI_Draw()
{
	if (UI_Alpha < A_Origin)
	{
		Sprite_Draw(Operation_TexID, Game_Over_Title_X, Game_Over_Title_Y, Game_Over_Title_Width, Game_Over_Title_Height, 0.f, { A_Origin, A_Origin, A_Origin, UI_Alpha });
		Sprite_Draw(Failed_TexID, Game_Over_Status_X, Game_Over_Status_Y, Game_Over_Status_Width, Game_Over_Status_Height, 0.f, { A_Origin, A_Origin, A_Origin, UI_Alpha });
		Sprite_Draw(Game_Over_JP_TexID, Game_Over_JP_X, Game_Over_JP_Y, Game_Over_JP_Width, Game_Over_JP_Height, 0.f, { A_Origin, A_Origin, A_Origin, UI_Alpha });
		UI_Alpha += UI_Alpha_Increase;

		if (UI_Alpha >= A_Origin)
		{
			UI_Alpha = A_Origin;
			Is_Failed_Sound_Played = true;
		}
	}
	else
	{
		Sprite_Draw(Operation_TexID, Game_Over_Title_X, Game_Over_Title_Y, Game_Over_Title_Width, Game_Over_Title_Height, 0.f);
		Sprite_Draw(Failed_TexID, Game_Over_Status_X, Game_Over_Status_Y, Game_Over_Status_Width, Game_Over_Status_Height, 0.f);
		Sprite_Draw(Game_Over_JP_TexID, Game_Over_JP_X, Game_Over_JP_Y, Game_Over_JP_Width, Game_Over_JP_Height, 0.f);
		Is_Title_Anime_Done = true;
	}
}

void Stage_Clear_UI_Draw()
{
	if (UI_Alpha < A_Origin)
	{
		Sprite_Draw(Operation_TexID, Game_Over_Title_X, Game_Over_Title_Y, Game_Over_Title_Width, Game_Over_Title_Height, 0.f, { A_Origin, A_Origin, A_Origin, UI_Alpha });
		Sprite_Draw(Complete_TexID, Game_Over_Status_X, Game_Over_Status_Y, Game_Over_Status_Width, Game_Over_Status_Height, 0.f, { A_Origin, A_Origin, A_Origin, UI_Alpha });
		Sprite_Draw(Complete_JP_TexID, Game_Over_JP_X, Game_Over_JP_Y, Game_Over_JP_Width, Game_Over_JP_Height, 0.f, { A_Origin, A_Origin, A_Origin, UI_Alpha });
		UI_Alpha += UI_Alpha_Increase;

		if (UI_Alpha >= A_Origin)
			UI_Alpha = A_Origin;
	}
	else
	{
		Sprite_Draw(Operation_TexID, Game_Over_Title_X, Game_Over_Title_Y, Game_Over_Title_Width, Game_Over_Title_Height, 0.f);
		Sprite_Draw(Complete_TexID, Game_Over_Status_X, Game_Over_Status_Y, Game_Over_Status_Width, Game_Over_Status_Height, 0.f);
		Sprite_Draw(Complete_JP_TexID, Game_Over_JP_X, Game_Over_JP_Y, Game_Over_JP_Width, Game_Over_JP_Height, 0.f);
		Is_Title_Anime_Done = true;
	}
}

void Set_Game_Over_Input(GAME_OVER_MENU_BUFFER Input)
{
	Game_Over_Buffer_Input = Input;
}

GAME_OVER_MENU_BUFFER Get_Game_Over_Input()
{
	return Game_Over_Buffer_Input;
}

void Set_Game_Over_Menu_Selected(GAME_OVER_MENU_SELCETED Selected)
{
	Game_Over_Selected = Selected;
}

GAME_OVER_MENU_SELCETED Get_Game_Over_Menu_Selected()
{
	return Game_Over_Selected;
}