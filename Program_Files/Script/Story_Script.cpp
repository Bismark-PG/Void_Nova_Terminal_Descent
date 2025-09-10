/*==============================================================================

	Story Script Manager [Story_Script.h]

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

static int Story_Commander_TexID = -1;
static int Anime_Commander_TexID = -1;
static int Play_Commander_TexID = -1;
static int BG_Commander_TexID = -1;
static int Intro_Commander_TexID = -1;

static int Story_Button_TexID = -1;
static int Anime_Button_TexID = -1;
static int Play_Button_TexID = -1;

static int Story_BG_TexID = -1;
static int Story_Enemy_BG_TexID = -1;

static int Script_Example = -1;

static SCRIPT_ANIMATION_STATE Animation_State = SCRIPT_ANIMATION_STATE::IN_ACTIVE;
static SCRIPT_ANIMATION_STATE Boss_Animation_State = SCRIPT_ANIMATION_STATE::IN_ACTIVE;
static SCRIPT_ANIMATION_STATE Ending_Animation_State = SCRIPT_ANIMATION_STATE::IN_ACTIVE;

static bool Is_Script_Active = false;

static int Last_Commanded_Page = -1;

XMFLOAT2 Stage_1_2_3_Start_Pos = { 0.0f, 0.0f };
XMFLOAT2 Stage_3_Boss_Start_Pos = { 0.0f, 0.0f };
XMFLOAT2 Stage_4_Start_Pos = { 0.0f, 0.0f };
XMFLOAT2 Stage_4_Boss_Start_Pos = { 0.0f, 0.0f };
XMFLOAT2 Stage_5_Start_Pos = { 0.0f, 0.0f };
XMFLOAT2 Stage_5_Boss_Start_Pos = { 0.0f, 0.0f };

void Script_Initialize()
{
	Story_Commander_TexID = Texture_Load(L"Resource/Texture/Story/Story_Man_Animation_Extend_Fixed.png");
	Anime_Commander_TexID = SpriteAni_Get_Pattern_Info(Story_Commander_TexID, 32, 8, 0.1, { 512, 512 }, { 512 * 0, 512 * 0 }, true);
	Play_Commander_TexID  = SpriteAni_CreatePlayer(Anime_Commander_TexID);
	BG_Commander_TexID	  = Texture_Load(L"Resource/Texture/Story/Story_Man_BG.png");
	Intro_Commander_TexID = Texture_Load(L"Resource/Texture/Story/Story_Man_Fixed.png");

	Story_Button_TexID = Texture_Load(L"Resource/Texture/Story/Button_Anime_Fixed.png");
	Anime_Button_TexID = SpriteAni_Get_Pattern_Info(Story_Button_TexID, 16, 4, 0.1, { 512, 512 }, { 512 * 0, 512 * 0 }, true);
	Play_Button_TexID = SpriteAni_CreatePlayer(Anime_Button_TexID);

	Story_BG_TexID = Texture_Load(L"Resource/Texture/Story/Story_BG_Extend.png");
	Story_Enemy_BG_TexID = Texture_Load(L"Resource/Texture/Story/Story_BG_Enemy.png");

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

	Script_Set_Player_POS_Enum();

	Script_Draw_Initialize();
	Set_Position(Story_X, Story_Y, Story_Enemy_Y, Story_Width, Story_Height);
	Ending_Set_Position(Story_X, Story_Y, Story_Width, Story_Height);
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

bool Script_Update(double elapsed_time)
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
			Story_Direction_Y = Story_Y;
			Set_Script_State(SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL);
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
			Set_Script_State(SCRIPT_ANIMATION_STATE::COMMANDER_BOX_INTRO);
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
		Page_Input_Cooldown_Timer -= elapsed_time;

		if (Page_Input_Cooldown_Timer <= 0.0 && Is_Current_Script_Done() &&
			(KeyLogger_IsTrigger(KK_ENTER) || KeyLogger_IsTrigger(KK_SPACE) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A)))
		{
			SM->Play_SFX("Stage_Story_Enter");
			Is_Page_Complete();
			return true;
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

bool Script_Boss_Update(double elapsed_time)
{
	if (!Is_Script_Active) return false;

	float Time = static_cast<float>(elapsed_time);

	switch (Boss_Animation_State)
	{
	case SCRIPT_ANIMATION_STATE::BG_INTRO_VERTICAL:
	{
		float Change = Story_Height * Intro_Direction_Speed * Game_Scale * Time;

		Story_Direction_Height += Change;
		Story_Direction_Y -= Change * Story_Direction_Limit;
		if (Story_Direction_Height >= Story_Height)
		{
			Story_Direction_Height = Story_Height;
			Story_Direction_Y = Story_Enemy_Y;
			Set_Script_State(SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL, true);
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
			Set_Script_State(SCRIPT_ANIMATION_STATE::TEXT_ACTIVE, true);
		}
	}
	break;

	case SCRIPT_ANIMATION_STATE::TEXT_ACTIVE:
		Boss_Page_Auto_Timer -= elapsed_time;

		if (Boss_Page_Auto_Timer <= 0.0 && Is_Current_Script_Done())
		{
			SM->Play_SFX("Stage_Story_Enter");
			Is_Page_Complete();
			Story_Force_Next_Page();
			Boss_Page_Auto_Timer = BOSS_PAGE_AUTO_DELAY;
		}
		break;

	case SCRIPT_ANIMATION_STATE::BG_OUTRO:
		Story_Alpha -= Commander_Alpha_Direction_Speed * 0.5f * Time;

		if (Story_Alpha <= 0.0f)
		{
			Story_Alpha = 0.0f;
			Set_Script_State(SCRIPT_ANIMATION_STATE::FINISHED, true);
		}
		break;
	}

	return false;
}

void Ending_Commander_Start()
{
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
			Sprite_Draw(Story_BG_TexID, Story_X, Story_Y, Story_Width, Story_Height, 0.f);
		return;
	}

	if (Ending_Animation_State <= SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL)
		Sprite_Draw(Story_BG_TexID, Story_Direction_X, Story_Direction_Y, Story_Direction_Width, Story_Direction_Height, 0.f);
	else
		Sprite_Draw(Story_BG_TexID, Story_X, Story_Y, Story_Width, Story_Height, 0.f);
}

void Script_Animation_Draw()
{
	if (Animation_State == SCRIPT_ANIMATION_STATE::IN_ACTIVE || Animation_State == SCRIPT_ANIMATION_STATE::FINISHED)
		return;

	if (Get_Now_Playing_Stage() == NOW_PLAYING_STAGE::STAGE_FIVE && !Is_Boss_Story_Time())
		return;

	if (Animation_State == SCRIPT_ANIMATION_STATE::BG_OUTRO)
	{
		Sprite_Draw(Story_BG_TexID, Story_X, Story_Y, Story_Width, Story_Height, 0.f, { A_Origin, A_Origin, A_Origin, Story_Alpha });
		return;
	}

	if (Animation_State <= SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL)
		Sprite_Draw(Story_BG_TexID, Story_Direction_X, Story_Direction_Y, Story_Direction_Width, Story_Direction_Height, 0.f);
	else
		Sprite_Draw(Story_BG_TexID, Story_X, Story_Y, Story_Width, Story_Height, 0.f);

	if (Animation_State == SCRIPT_ANIMATION_STATE::COMMANDER_BOX_INTRO || Animation_State == SCRIPT_ANIMATION_STATE::COMMANDER_BOX_OUTRO)
		Sprite_Draw(BG_Commander_TexID, Commander_Direction_X, Commander_Direction_Y, Commander_Direction_Size, Commander_Direction_Size, 0.f);
	else if (Animation_State > SCRIPT_ANIMATION_STATE::COMMANDER_BOX_INTRO && Animation_State < SCRIPT_ANIMATION_STATE::COMMANDER_BOX_OUTRO)
		Sprite_Draw(BG_Commander_TexID, Commander_X, Commander_Y, Commander_Size, Commander_Size, 0.f);

	if (Animation_State == SCRIPT_ANIMATION_STATE::COMMANDER_FADE_IN)
		Sprite_Draw(Intro_Commander_TexID, Commander_X, Commander_Y, Commander_Size, Commander_Size, 0.f, { A_Origin, A_Origin, A_Origin, Commander_Alpha });
	else if (Animation_State == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE)
		SpriteAni_Draw(Play_Commander_TexID, Commander_X, Commander_Y, Commander_Size, Commander_Size);
}

void Script_Boss_Animation_Draw()
{
	if (Boss_Animation_State == SCRIPT_ANIMATION_STATE::IN_ACTIVE || Boss_Animation_State == SCRIPT_ANIMATION_STATE::FINISHED)
		return;

	if (Boss_Animation_State == SCRIPT_ANIMATION_STATE::BG_OUTRO)
	{
		Sprite_Draw(Story_Enemy_BG_TexID, Story_X, Story_Enemy_Y, Story_Width, Story_Height, 0.f, { A_Origin, A_Origin, A_Origin, Story_Alpha });
		return;
	}

	float Current_Y = (Boss_Animation_State == SCRIPT_ANIMATION_STATE::BG_INTRO_VERTICAL) ? Story_Enemy_Y + (Story_Height - Story_Direction_Height) * 0.5f : Story_Enemy_Y;

	if (Boss_Animation_State <= SCRIPT_ANIMATION_STATE::BG_INTRO_HORIZONTAL)
		Sprite_Draw(Story_Enemy_BG_TexID, Story_Direction_X, Current_Y, Story_Direction_Width, Story_Direction_Height, 0.f);
	else
		Sprite_Draw(Story_Enemy_BG_TexID, Story_X, Story_Enemy_Y, Story_Width, Story_Height, 0.f);
}

void Script_Text_Draw(NOW_PLAYING_STAGE Now_Playing_Stage, int Now_Playing_Page, bool Is_Boss_Story)
{
	if (Now_Playing_Page != Last_Commanded_Page)
	{
		if (!Is_Boss_Story)
		{
			switch (Now_Playing_Stage)
			{
			case NOW_PLAYING_STAGE::STAGE_ONE:
				if (Now_Playing_Page == 0)
					Stage_Command_Player_Move(Stage_1_2_3_Start_Pos, (Stage_1_Start_Speed * Game_Scale));
				break;

			case NOW_PLAYING_STAGE::STAGE_TWO:
				if (Now_Playing_Page == 0)
					Stage_Command_Player_Move(Stage_1_2_3_Start_Pos, (Stage_2_Start_Speed * Game_Scale));
				break;

			case NOW_PLAYING_STAGE::STAGE_THREE:
				if (Now_Playing_Page == 0)
					Stage_Command_Player_Move(Stage_1_2_3_Start_Pos, (Stage_2_Start_Speed * Game_Scale));
				break;

			case NOW_PLAYING_STAGE::STAGE_FOUR:
				if (Now_Playing_Page == 0)
					Stage_Command_Player_Move(Stage_4_Start_Pos, (Stage_2_Start_Speed * Game_Scale));
				break;

			case NOW_PLAYING_STAGE::STAGE_FIVE:
				if (Now_Playing_Page == 0)
					Stage_Command_Player_Move(Stage_1_2_3_Start_Pos, (Stage_2_Start_Speed * Game_Scale));
				break;
			}
		}
		Last_Commanded_Page = Now_Playing_Page;
	}

	bool Draw_Text = (!Is_Boss_Story && Animation_State == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE) ||
		(Is_Boss_Story && Boss_Animation_State == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE);

	if (Is_Script_Active && Draw_Text)
	{
		switch (Now_Playing_Stage)
		{
		case NOW_PLAYING_STAGE::STAGE_ONE:
			Script_Stage_1_Draw(Now_Playing_Page);
			break;

		case NOW_PLAYING_STAGE::STAGE_TWO:
			Script_Stage_2_Draw(Now_Playing_Page);
			break;

		case NOW_PLAYING_STAGE::STAGE_THREE:
			Script_Stage_3_Draw(Now_Playing_Page, Is_Boss_Story);
			break;

		case NOW_PLAYING_STAGE::STAGE_FOUR:
			Script_Stage_4_Draw(Now_Playing_Page, Is_Boss_Story);
			break;

		case NOW_PLAYING_STAGE::STAGE_FIVE:
			Script_Stage_5_Draw(Now_Playing_Page, Is_Boss_Story);
			break;
		}

		if (Is_Current_Script_Done())
			Script_Next_Button_Draw();
	}
}

void Script_Next_Button_Draw()
{
	if (Get_Now_Playing_Stage() == NOW_PLAYING_STAGE::STAGE_FIVE && !Is_Boss_Story_Time())
		return;

	float POS_Y = A_Zero;

	if (Is_Boss_Story_Time())
		POS_Y = Story_Enemy_Button_Y;
	else
		POS_Y = Story_Button_Y;

	SpriteAni_Draw(Play_Button_TexID, Story_Button_X, POS_Y, Story_Button_Size, Story_Button_Size);
}


void Script_Start()
{
	if (Animation_State != SCRIPT_ANIMATION_STATE::IN_ACTIVE && Animation_State != SCRIPT_ANIMATION_STATE::FINISHED)
		return;

	Script_Reset();
	Is_Script_Active = true;
	Set_Script_State(SCRIPT_ANIMATION_STATE::BG_INTRO_VERTICAL);
	SM->Play_SFX("Stage_Story_BG");
}

void Script_Start_Outro()
{
	if (Animation_State == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE)
		Set_Script_State(SCRIPT_ANIMATION_STATE::COMMANDER_BOX_OUTRO);
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

void Script_Boss_Start()
{
	if (Boss_Animation_State != SCRIPT_ANIMATION_STATE::IN_ACTIVE && Boss_Animation_State != SCRIPT_ANIMATION_STATE::FINISHED)
		return;

	Script_Boss_Reset();
	Is_Script_Active = true;
	Set_Script_State(SCRIPT_ANIMATION_STATE::BG_INTRO_VERTICAL, true);
	SM->Play_SFX("Stage_Story_BG");
}

void Script_Boss_Start_Outro()
{
	if (Boss_Animation_State == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE)
		Set_Script_State(SCRIPT_ANIMATION_STATE::BG_OUTRO, true);
}

void Script_Boss_Reset()
{
	Is_Script_Active = false;

	Story_Direction_Width = Story_Width * A_Zero_Five;
	Story_Direction_Height = 0;
	Story_Direction_X = Story_X + (Story_Width * Story_Direction_Limit);
	Story_Direction_Y = Story_Enemy_Y + (Story_Height * Story_Direction_Limit);
	Story_Alpha = 1.0f;

	Set_Script_State(SCRIPT_ANIMATION_STATE::IN_ACTIVE, true);
}

void Set_Script_State(SCRIPT_ANIMATION_STATE State, bool Is_Boss)
{
	if (Is_Boss)
		Boss_Animation_State = State;
	else
		Animation_State = State;

	if (State == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE)
	{
		if (Is_Boss)
			Boss_Page_Auto_Timer = BOSS_PAGE_AUTO_DELAY;
		else
			Page_Input_Cooldown_Timer = PAGE_INPUT_DELAY;
	}
}

SCRIPT_ANIMATION_STATE Get_Script_State()
{
	return Animation_State;
}

SCRIPT_ANIMATION_STATE Get_Boss_Script_State()
{
	return Boss_Animation_State;
}


bool Is_Script_Outro_Finished(bool Is_Boss)
{
	if (Is_Boss)
		return Get_Boss_Script_State() == SCRIPT_ANIMATION_STATE::FINISHED;
	else
		return Get_Script_State() == SCRIPT_ANIMATION_STATE::FINISHED;
}

void Script_Set_Player_POS_Enum()
{
	Stage_1_2_3_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) * A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_T_Quarters) - (PLAYER_HEIGHT * A_Half) };

	Stage_3_Boss_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) *A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_F_Fifths) - (PLAYER_HEIGHT * A_Half) };

	Stage_4_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) *A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_Half) - (PLAYER_HEIGHT * A_Half) };

	Stage_4_Boss_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) *A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_T_Quarters) - (PLAYER_HEIGHT * A_Half) };

	Stage_5_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) *A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_Six_Five) - (PLAYER_HEIGHT * A_Half) };

	Stage_5_Boss_Start_Pos = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH) *A_Half),
		static_cast<float>(SCREEN_HEIGHT * A_T_Quarters) - (PLAYER_HEIGHT * A_Half) };
}

void Script_Stage_1_Draw(int Now_Page)
{
	Script_Stage_1(Now_Page);
}

void Script_Stage_2_Draw(int Now_Page)
{
	Script_Stage_2(Now_Page);
}

void Script_Stage_3_Draw(int Now_Page, bool is_boss_story)
{
	if (is_boss_story)
		Script_Stage_3_BOSS(Now_Page);
	else
		Script_Stage_3(Now_Page);
}

void Script_Stage_4_Draw(int Now_Page, bool is_boss_story)
{
	if (is_boss_story)
		Script_Stage_4_BOSS(Now_Page);
	else
		Script_Stage_4(Now_Page);
}

void Script_Stage_5_Draw(int Now_Page, bool is_boss_story)
{
	if (is_boss_story)
	{
		if (Get_Stage_5_State() == STAGE_5_STATE::BOSS_APPEAR_STORY)
			Script_Stage_5_BOSS_Phase_1(Now_Page);
		else if (Get_Stage_5_State() == STAGE_5_STATE::PHASE_CHANGE_STORY)
			Script_Stage_5_BOSS_Phase_2(Now_Page);
	}
	else
		Script_Stage_5(Now_Page);
}
