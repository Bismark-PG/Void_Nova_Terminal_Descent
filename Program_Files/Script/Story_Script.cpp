/*==============================================================================

	Story Script Manager [Story_Script.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Story_Script.h"
#include "Game_Window.h"
#include "Sprite_Animation.h"
#include "system_timer.h"
#include "KeyLogger.h"
#include "Script_Draw.h"
#include "Story_Manager.h"
#include "Palette.h"
#include "Player.h"
#include "Audio_Manager.h"
#include "Stage_Update.h"
#include "Ending.h"

using namespace DirectX;
using namespace PALETTE;

static int Anime_Commander = -1;
static int Play_Commander = -1;

static int Anime_Button = -1;
static int Play_Button = -1;

static int Story_BG = -1;
static int Story_Enemy_BG = -1;
static int Story_Commander_BG = -1;
static int Story_Commander_Intro = -1;

static SCRIPT_ANIMATION_STATE Animation_State = SCRIPT_ANIMATION_STATE::IN_ACTIVE;
static SCRIPT_ANIMATION_STATE Ending_Animation_State = SCRIPT_ANIMATION_STATE::IN_ACTIVE;

static bool Is_Script_Active = false;

static int Last_Commanded_Page = -1;

void Script_Initialize()
{
	Script_Texture();

	Story_Width = Game_Screen_Width * STORY_WIDTH_SACLE;
	Story_Height = Story_Width * STORY_HEIGHT_SACLE;

	Story_X = Game_Offset.x + ((Game_Screen_Width - Story_Width) * Story_Direction_Limit);
	Story_Y = Story_Height * A_Quarter;
	Story_Enemy_Y = Game_Screen_Height - Story_Height - (Story_Height * A_Quarter);
	Story_Enemy_Button_Y = Story_Enemy_Y + (Story_Height * A_Quarter);

	Story_Direction_X = Story_X + (Story_Width  * Story_Direction_Limit);
	Story_Direction_Y = Story_Y + (Story_Height * Story_Direction_Limit);

	Story_Direction_Width = Story_Width * A_Zero_Five;

	Commander_Size = Story_Width * COMMANDER_SIZE_SACLE;

	Commander_X = (Story_X + Story_Width)  - (Commander_Size * Commander_Direction_Limit);
	Commander_Y = (Story_Y + Story_Height) - (Commander_Size * Commander_Direction_Limit);

	Commander_Direction_X = Commander_X + (Commander_Size * Commander_Direction_Limit);
	Commander_Direction_Y = Commander_Y + (Commander_Size * Commander_Direction_Limit);

	Story_Button_Size = Commander_Size * BUTTON_SIZE_SACLE;

	Story_Button_X = Commander_X - (Story_Button_Size * A_One_And_Half);
	Story_Button_Y = Commander_Y + (Story_Button_Size * A_Half);

	Script_Set_Player_Target_Pos();

	Script_Draw_Initialize();
	Set_Position(Story_X, Story_Y, Story_Enemy_Y, Story_Width, Story_Height);
	//Ending_Set_Position(Story_X, Story_Y, Story_Width, Story_Height);
}

void Script_Finalize()
{
	Story_X = 0;
	Story_Y = 0;	
	Story_Width = 0;
	Story_Height = 0;

	Story_Direction_X = 0;
	Story_Direction_Y = 0;
	Story_Direction_Width  = 0;
	Story_Direction_Height = 0;

	Story_Enemy_Y = 0;
	Story_Enemy_Button_Y = 0;

	Commander_X = 0;
	Commander_Y = 0;
	Commander_Size = 0;

	Commander_Direction_X = 0;
	Commander_Direction_Y = 0;
	Commander_Direction_Size = 0;

	Commander_Alpha = 0.0f;
}

bool Script_Update(double elapsed_time, bool isAutoForward, bool Is_Boss_Story)
{
	if (!Is_Script_Active) return false;

	float Time = static_cast<float>(elapsed_time);

	switch (Animation_State)
	{
	case SCRIPT_ANIMATION_STATE::BG_INTRO_VERTICAL:
	{
		float Change = Story_Height * Intro_Direction_Speed * Game_Scale * Time;
		Story_Direction_Height += Change;
		Story_Direction_Y -= Change * Story_Direction_Limit;
		if (Story_Direction_Height >= Story_Height)
		{
			Story_Direction_Height = Story_Height;
			Story_Direction_Y = Is_Boss_Story ? Story_Enemy_Y : Story_Y; 
			Animation_State = SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL;
		}
	}
	break;

	case SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL:
	{
		float Change = Story_Width * Intro_Direction_Speed * Game_Scale * Time;
		Story_Direction_Width += Change;
		Story_Direction_X -= Change * Story_Direction_Limit;
		if (Story_Direction_Width >= Story_Width)
		{
			Story_Direction_Width = Story_Width;
			Story_Direction_X = Story_X;
			Animation_State = Is_Boss_Story ? SCRIPT_ANIMATION_STATE::TEXT_ACTIVE : SCRIPT_ANIMATION_STATE::COMMANDER_BOX_INTRO;
		}
	}
	break;

	case SCRIPT_ANIMATION_STATE::COMMANDER_BOX_INTRO:
	{
		float Change = Commander_Size * Intro_Direction_Speed * Game_Scale * Time;

		Commander_Direction_Size += Change;
		Commander_Direction_X -= Change * Commander_Direction_Limit;
		Commander_Direction_Y -= Change * Commander_Direction_Limit;

		if (Commander_Direction_Size >= Commander_Size)
		{
			Commander_Direction_Size = Commander_Size;
			Commander_Direction_X = Commander_X;
			Commander_Direction_Y = Commander_Y;
			Set_Script_State(SCRIPT_ANIMATION_STATE::COMMANDER_FADE_IN);
		}
	}
	break;

	case SCRIPT_ANIMATION_STATE::COMMANDER_FADE_IN:
		Commander_Alpha += Commander_Alpha_Direction_Speed * Time;

		if (Commander_Alpha >= 1.0f)
		{
			Commander_Alpha = 1.0f;
			Set_Script_State(SCRIPT_ANIMATION_STATE::TEXT_ACTIVE);
		}
		break;

	case SCRIPT_ANIMATION_STATE::TEXT_ACTIVE:
		if (isAutoForward)
		{
			Boss_Page_Auto_Timer -= elapsed_time;
			if (Boss_Page_Auto_Timer <= 0.0 && Is_Current_Script_Done())
			{
				Sound_M->Play_SFX("Stage_Story_Enter");
				Is_Page_Complete();
				Boss_Page_Auto_Timer = BOSS_PAGE_AUTO_DELAY;
				return true;
			}
		}
		else
		{
			Page_Input_Cooldown_Timer -= elapsed_time;
			if (Page_Input_Cooldown_Timer <= 0.0 && Is_Current_Script_Done() &&
				(KeyLogger_IsTrigger(KK_ENTER) || KeyLogger_IsTrigger(KK_SPACE) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A)))
			{
				Sound_M->Play_SFX("Stage_Story_Enter");
				Is_Page_Complete();
				return true;
			}
		}
		break;

	case SCRIPT_ANIMATION_STATE::COMMANDER_BOX_OUTRO:
	{
		float Change = Commander_Size * Outro_Direction_Speed * Game_Scale * Time;

		Commander_Direction_Size -= Change;
		Commander_Direction_X += Change * Commander_Direction_Limit;
		Commander_Direction_Y += Change * Commander_Direction_Limit;

		if (Commander_Direction_Size <= 0.0f)
		{
			Commander_Direction_Size = 0.0f;
			Set_Script_State(SCRIPT_ANIMATION_STATE::BG_OUTRO);
		}
	}
	break;

	case SCRIPT_ANIMATION_STATE::BG_OUTRO:
		Story_Alpha -= Commander_Alpha_Direction_Speed * 0.5f * Time;

		if (Story_Alpha <= 0.0f)
		{
			Story_Alpha = 0.0f;
			Set_Script_State(SCRIPT_ANIMATION_STATE::FINISHED);
		}
		break;
	}

	return false;
}

void Ending_Commander_Start()
{
	Ending_Get_Text_POS(Story_X, Story_Y, Story_Width, Story_Height);

	if (Ending_Animation_State != SCRIPT_ANIMATION_STATE::IN_ACTIVE && Ending_Animation_State != SCRIPT_ANIMATION_STATE::FINISHED)
		return;

	Story_Direction_Width = Story_Width * A_Zero_Five;
	Story_Direction_Height = 0;
	Story_Direction_X = Story_X + (Story_Width * Story_Direction_Limit);
	Story_Direction_Y = Story_Y + (Story_Height * Story_Direction_Limit);

	Ending_Animation_State = SCRIPT_ANIMATION_STATE::BG_INTRO_VERTICAL;
}

void Ending_Commander_Update(double elapsed_time)
{
	float Time = static_cast<float>(elapsed_time);

	switch (Ending_Animation_State)
	{
	case SCRIPT_ANIMATION_STATE::BG_INTRO_VERTICAL:
	{
		float Change = Story_Height * Intro_Direction_Speed * Game_Scale * Time;

		Story_Direction_Height += Change;
		Story_Direction_Y -= Change * Story_Direction_Limit;
		if (Story_Direction_Height >= Story_Height)
		{
			Story_Direction_Height = Story_Height;
			Story_Direction_Y = Story_Y;
			Ending_Animation_State = SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL;
		}
	}
	break;

	case SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL:
	{
		float Change = Story_Width * Intro_Direction_Speed * Game_Scale * Time;

		Story_Direction_Width += Change;
		Story_Direction_X -= Change * Story_Direction_Limit;
		if (Story_Direction_Width >= Story_Width)
		{
			Story_Direction_Width = Story_Width;
			Story_Direction_X = Story_X;
			Ending_Animation_State = SCRIPT_ANIMATION_STATE::FINISHED;
		}
	}
	break;
	}
}

void Ending_Commander_Draw()
{
	if (Ending_Animation_State == SCRIPT_ANIMATION_STATE::IN_ACTIVE || Ending_Animation_State == SCRIPT_ANIMATION_STATE::FINISHED)
	{
		if (Ending_Animation_State == SCRIPT_ANIMATION_STATE::FINISHED)
			Sprite_Draw(Story_BG, Story_X, Story_Y, Story_Width, Story_Height, 0.f);
		return;
	}

	if (Ending_Animation_State <= SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL)
		Sprite_Draw(Story_BG, Story_Direction_X, Story_Direction_Y, Story_Direction_Width, Story_Direction_Height, 0.f);
	else
		Sprite_Draw(Story_BG, Story_X, Story_Y, Story_Width, Story_Height, 0.f);
}

void Script_Animation_Draw(bool Is_Boss_Story)
{
	if (Animation_State == SCRIPT_ANIMATION_STATE::IN_ACTIVE || Animation_State == SCRIPT_ANIMATION_STATE::FINISHED)
		return;

	if (Get_Now_Playing_Stage() == NOW_PLAYING_STAGE::STAGE_FIVE)
	{
		if (Get_Stage_5_State() == STAGE_5_STATE::STAGE_STORY || Get_Stage_5_State() == STAGE_5_STATE::BOSS_FINAL_PHASE_ENTERING)
			return;
	}

	float Current_Y = Is_Boss_Story ? Story_Enemy_Y : Story_Y;
	int BG_Texture = Is_Boss_Story ? Story_Enemy_BG : Story_BG;

	if (Animation_State == SCRIPT_ANIMATION_STATE::BG_OUTRO)
	{
		Sprite_Draw(BG_Texture, Story_X, Current_Y, Story_Width, Story_Height, 0.f, { 1.0f, 1.0f, 1.0f, Story_Alpha });
		return;
	}

	if (Animation_State <= SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL)
	{
		float introY = Is_Boss_Story ? Story_Enemy_Y + (Story_Height - Story_Direction_Height) * 0.5f : Story_Direction_Y;
		Sprite_Draw(BG_Texture, Story_Direction_X, introY, Story_Direction_Width, Story_Direction_Height, 0.f);
	}
	else
	{
		Sprite_Draw(BG_Texture, Story_X, Current_Y, Story_Width, Story_Height, 0.f);
	}

	if (!Is_Boss_Story)
	{
		if (Animation_State == SCRIPT_ANIMATION_STATE::COMMANDER_BOX_INTRO || Animation_State == SCRIPT_ANIMATION_STATE::COMMANDER_BOX_OUTRO)
			Sprite_Draw(Story_Commander_BG, Commander_Direction_X, Commander_Direction_Y, Commander_Direction_Size, Commander_Direction_Size, 0.f);
		else if (Animation_State > SCRIPT_ANIMATION_STATE::COMMANDER_BOX_INTRO && Animation_State < SCRIPT_ANIMATION_STATE::COMMANDER_BOX_OUTRO)
			Sprite_Draw(Story_Commander_BG, Commander_X, Commander_Y, Commander_Size, Commander_Size, 0.f);

		if (Animation_State == SCRIPT_ANIMATION_STATE::COMMANDER_FADE_IN)
			Sprite_Draw(Story_Commander_Intro, Commander_X, Commander_Y, Commander_Size, Commander_Size, 0.f, { A_Origin, A_Origin, A_Origin, Commander_Alpha });
		else if (Animation_State == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE)
			SpriteAni_Draw(Play_Commander, Commander_X, Commander_Y, Commander_Size, Commander_Size);
	}
}

void Script_Text_Draw(NOW_PLAYING_STAGE Now_Playing_Stage, int Now_Playing_Page, bool Is_Boss_Story, STORY_TYPE Type)
{
	switch (Now_Playing_Stage)
	{
	case NOW_PLAYING_STAGE::STAGE_ONE:
		Script_Stage_1_Draw(Now_Playing_Page, Type);
		break;

	case NOW_PLAYING_STAGE::STAGE_TWO:
		Script_Stage_2_Draw(Now_Playing_Page, Type);
		break;

	case NOW_PLAYING_STAGE::STAGE_THREE:
		Script_Stage_3_Draw(Now_Playing_Page, Type);
		break;

	case NOW_PLAYING_STAGE::STAGE_FOUR:
		Script_Stage_4_Draw(Now_Playing_Page, Type);
		break;

	case NOW_PLAYING_STAGE::STAGE_FIVE:
		Script_Stage_5_Draw(Now_Playing_Page, Type);
		break;
	}

	if (Is_Current_Script_Done())
		Script_Next_Button_Draw(Is_Boss_Story);
}

void Script_Begin(bool isAutoForward, bool Is_Boss_Story)
{
	Script_Reset();
	Is_Script_Active = true;
	Sound_M->Play_SFX("Stage_Story_BG");

	bool Is_State_Skip = false;
	if (Get_Now_Playing_Stage() == NOW_PLAYING_STAGE::STAGE_FIVE)
	{
		if (Get_Stage_5_State() == STAGE_5_STATE::STAGE_STORY || Get_Stage_5_State() == STAGE_5_STATE::BOSS_FINAL_PHASE_ENTERING)
			Is_State_Skip = true;
	}

	if (Is_State_Skip)
		Animation_State = SCRIPT_ANIMATION_STATE::TEXT_ACTIVE;
	else
		Animation_State = SCRIPT_ANIMATION_STATE::BG_INTRO_VERTICAL;

	// Set up timers based on director's notes
	if (isAutoForward)
		Boss_Page_Auto_Timer = BOSS_PAGE_AUTO_DELAY;
	else
		Page_Input_Cooldown_Timer = PAGE_INPUT_DELAY;
}

void Script_Next_Button_Draw(bool Is_Boss_Story)
{
	if (Get_Now_Playing_Stage() == NOW_PLAYING_STAGE::STAGE_FIVE)
	{
		if (Get_Stage_5_State() == STAGE_5_STATE::STAGE_STORY || Get_Stage_5_State() == STAGE_5_STATE::BOSS_FINAL_PHASE_ENTERING)
			return;
	}

	float POS_Y = A_Zero;

	if (Is_Boss_Story)
		POS_Y = Story_Enemy_Button_Y;
	else
		POS_Y = Story_Button_Y;

	SpriteAni_Draw(Play_Button, Story_Button_X, POS_Y, Story_Button_Size, Story_Button_Size);
}

void Script_Start_Outro(bool Is_Boss_Story)
{
	if (Animation_State == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE)
	{
		Animation_State = Is_Boss_Story ? SCRIPT_ANIMATION_STATE::BG_OUTRO : SCRIPT_ANIMATION_STATE::COMMANDER_BOX_OUTRO;
	}
}

void Script_Reset()
{
	Is_Script_Active = false;
	Last_Commanded_Page = -1;
	
	Story_Direction_Width = Story_Width * A_Zero_Five;
	Story_Direction_Height = 0;
	Story_Direction_X = Story_X + (Story_Width * Story_Direction_Limit);
	Story_Direction_Y = Story_Y + (Story_Height * Story_Direction_Limit);
	Story_Alpha = 1.0f;

	Commander_Direction_Size = 0;
	Commander_Direction_X = Commander_X + (Commander_Size * Commander_Direction_Limit);
	Commander_Direction_Y = Commander_Y + (Commander_Size * Commander_Direction_Limit);
	Commander_Alpha = 0.0f;

	Set_Script_State(SCRIPT_ANIMATION_STATE::IN_ACTIVE);
}

void Set_Script_State(SCRIPT_ANIMATION_STATE State)
{
	Animation_State = State;
}

SCRIPT_ANIMATION_STATE Get_Script_State()
{
	return Animation_State;
}

bool Is_Script_Outro_Finished()
{
	return Get_Script_State() == SCRIPT_ANIMATION_STATE::FINISHED;
}

DirectX::XMFLOAT2 Script_Get_Enum(PLAYER_MOVE_POSITION POS)
{
	XMFLOAT2 ENUM_POS = { 0.0f, 0.0f };

	switch (POS)
	{
	case PLAYER_MOVE_POSITION::STAGE_START_POS:
		ENUM_POS = Stage_Start_Pos;
		break;

	case PLAYER_MOVE_POSITION::STAGE_3_BOSS_START_POS:
		ENUM_POS = Stage_3_Boss_Start_Pos;
		break;

	case PLAYER_MOVE_POSITION::STAGE_4_BOSS_START_POS:
		ENUM_POS = Stage_4_Boss_Start_Pos;
		break;

	case PLAYER_MOVE_POSITION::STAGE_5_BOSS_START_POS:
		ENUM_POS = Stage_5_Boss_Start_Pos;
		break;
	}

	return ENUM_POS;
}

void Script_Set_Player_Target_Pos()
{
	Stage_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) * A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_T_Quarters) - (PLAYER_HEIGHT * A_Half) };

	Stage_3_Boss_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) *A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_F_Fifths) - (PLAYER_HEIGHT * A_Half) };

	Stage_4_Boss_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) *A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_T_Quarters) - (PLAYER_HEIGHT * A_Half) };

	Stage_5_Boss_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) *A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_T_Quarters) - (PLAYER_HEIGHT * A_Half) };
}

void Script_Texture()
{
	int Commander = Texture_M->GetID("UI_Story_Commander_Noise");
	Anime_Commander = SpriteAni_Get_Pattern_Info(Commander, 32, 8, 0.1, { 512, 512 }, { 512 * 0, 512 * 0 }, true);
	Play_Commander = SpriteAni_CreatePlayer(Anime_Commander);

	int Button = Texture_M->GetID("UI_Story_Button");
	Anime_Button = SpriteAni_Get_Pattern_Info(Button, 16, 4, 0.1, { 512, 512 }, { 512 * 0, 512 * 0 }, true);
	Play_Button = SpriteAni_CreatePlayer(Anime_Button);

	Story_BG = Texture_M->GetID("UI_Story_BG");
	Story_Enemy_BG = Texture_M->GetID("UI_Enemy_Story_BG");
	Story_Commander_BG = Texture_M->GetID("UI_Story_Commander_BG");
	Story_Commander_Intro = Texture_M->GetID("UI_Story_Commander_Intro");
}

