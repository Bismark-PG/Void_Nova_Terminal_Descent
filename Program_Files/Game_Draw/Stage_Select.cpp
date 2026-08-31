/*==============================================================================

	Draw Stage [Stage_Select.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Stage_Select.h"
#include "Story_Manager.h"
#include "Game_Mode_Manager.h" 
#include "Game_Status.h" 
#include "Enemy_Bullet.h"
#include "Palette.h"
#include "Score.h"
#include "Stage_Update.h"
#include "Boss_Manager.h"
#include "Story_Script.h"
#include <Item.h>

using namespace PALETTE;
using namespace DirectX;

NOW_PLAYING_STAGE Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
static STAGE_FLOW_STATE	Stage_Flow  = STAGE_FLOW_STATE::IDLE;

static double Prepare_Game_Timer = 0.0;
static double State_Timer = 0.0;

static bool Is_Prepare_Sequence_Started = false;
static bool Is_Battle_Start_Sound_Played = false;
static bool Is_Blackout_Active = false;
static bool Is_Final_BGM_Active = false;

static bool Is_Player_Moving_By_Script = false;
static XMFLOAT2 Player_Move_Target;
static float Player_Move_Speed;

static float Current_Volume = 0.0f;
static double BGM_Fade_Timer = 0.0;
constexpr double BGM_FADE_TIME = 3.0;
static bool Is_BGM_Fading_In = false;
static bool Is_Stage_Start = false;
static bool Is_Fading = false;

void Stage_Initialize()
{
	Game_Logic_Initialize();

	Story_Initialize();
	Boss_Initialize();
	Stage_Update_Reset();
	Score_Initialize();

	Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
	Stage_Flow = STAGE_FLOW_STATE::IDLE;
	Is_Player_Moving_By_Script = false;

	Player_Spawn_X = Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH * A_Half);
	Player_Spawn_Y = static_cast<float>(SCREEN_HEIGHT) - (PLAYER_HEIGHT * A_One_And_Half);
	Player_Spawn_Speed *= Game_Scale;

	Current_Volume = 0.0f;
	BGM_Fade_Timer = 0.0;
	Is_BGM_Fading_In = false;
	Is_Stage_Start = false;
	Is_Fading = false;

	Is_Blackout_Active = false;
	Is_Final_BGM_Active = false;
}

void Stage_Finalize()
{
	Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
	Stage_Flow = STAGE_FLOW_STATE::IDLE;

	Player_Spawn_X = 0;
	Player_Spawn_Y = 0;
	Player_Spawn_Speed = 150.f;

	Current_Volume = 0.0f;
	BGM_Fade_Timer = 0.0;
	Is_BGM_Fading_In = false;
	Is_Stage_Start = false;
	Is_Fading = false;

	Is_Blackout_Active = false;
	Is_Final_BGM_Active = false;

	Score_Finalize();
	Stage_Update_Reset();
	Story_Finalize();

	Game_Logic_Finalize();
}

void Stage_Update(double elapsed_time)
{
	if (Is_BGM_Fading_In && BGM_Fade_Timer < BGM_FADE_TIME)
	{
		BGM_Fade_Timer += elapsed_time;
        float Target_Volume = Sound_M->Get_Target_BGM_Volume();
		Current_Volume = Target_Volume * static_cast<float>(BGM_Fade_Timer / BGM_FADE_TIME);

		if (Current_Volume > Target_Volume)
			Current_Volume = Target_Volume;

		Sound_M->Update_Current_BGM_Volume(Current_Volume);

		if (BGM_Fade_Timer >= BGM_FADE_TIME)
			Is_BGM_Fading_In = false;
	}

	if (Now_Playing_Stage == NOW_PLAYING_STAGE::STAGE_NONE) return;

	Status_Update(elapsed_time);
	Story_Update(elapsed_time);

	switch (Stage_Flow)
	{
	case STAGE_FLOW_STATE::IDLE:
		// Do nothing, Wait for stage selection
		break;

	case STAGE_FLOW_STATE::STAGE_SETUP:
		Stage_Update_Player_Movement(elapsed_time);
		switch (Now_Playing_Stage)
		{
		case NOW_PLAYING_STAGE::STAGE_ONE:
			Stage_One_Update(elapsed_time);
			break;
		case NOW_PLAYING_STAGE::STAGE_TWO:
			Stage_Two_Update(elapsed_time);

			break;
		case NOW_PLAYING_STAGE::STAGE_THREE:
			Stage_Three_Update(elapsed_time);

			break;
		case NOW_PLAYING_STAGE::STAGE_FOUR:
			Stage_Four_Update(elapsed_time);
			break;

		case NOW_PLAYING_STAGE::STAGE_FIVE:
			Stage_Five_Update(elapsed_time);
			break;
		}
		break;

	case STAGE_FLOW_STATE::PREPARE_GAME:
		Stage_Set_Is_Player_Moving_By_Script(false);

		if (!Is_Battle_Start_Sound_Played)
		{
			Sound_M->Play_SFX("Stage_Battle_Start");
			Is_Battle_Start_Sound_Played = true;
		}

		Prepare_Game_Timer += elapsed_time;

		if (Prepare_Game_Timer > 1.0)
		{
			if (Is_Ready_To_Logic_Draw())
			{
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAMEPLAY_ACTIVE);
				Stage_Timer_Reset();
			}
			Game_Logic_Update(elapsed_time);
		}
		break;

	case STAGE_FLOW_STATE::FAKE_BOSS_FADE_WAIT:
	{
		if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
			Set_Blackout_Overlay(true);
		BGM_Fade_Timer += elapsed_time;

		if (BGM_Fade_Timer > BGM_FADE_TIME)
		{
			Sound_M->Stop_BGM();
			Sound_M->Update_Current_BGM_Volume(Sound_M->Get_Target_BGM_Volume());

			Set_Stage_5_State(STAGE_5_STATE::BOSS_SWAP);
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::FAKE_BOSS_ENTERING);
			Set_Now_Anime_Flow(FINAL_BOSS_ANIME::FADE_START);

			State_Timer = 0.0;
		}

		float Target_Volume = Sound_M->Get_Target_BGM_Volume();
		Current_Volume = Target_Volume * (1.0f - static_cast<float>(BGM_Fade_Timer / BGM_FADE_TIME));

		if (Current_Volume < 0.0f)
			Current_Volume = 0.0f;

		Sound_M->Update_Current_BGM_Volume(Current_Volume);
	}
		break;

	case STAGE_FLOW_STATE::FAKE_BOSS_ENTERING:
		Stage_Five_Update(elapsed_time);
		Stage_BOSS_Ready_Update(elapsed_time);

		if (!Is_Final_BGM_Active)
		{
			if (Get_Now_Anime_Flow() == FINAL_BOSS_ANIME::ANIME_1_DONE)
			{
				Sound_M->Stop_BGM();
				float User_Set_Volume = Sound_M->Get_Target_BGM_Volume();
				Sound_M->Update_Current_BGM_Volume(User_Set_Volume);
				Set_BGM_For_Boss(Now_Playing_Stage);
				Is_Final_BGM_Active = true;
			}
		}

		if (Is_Ready_To_Final_Battle())
		{
			State_Timer = 0.0;
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAMEPLAY_ACTIVE);
			Stage_Timer_Reset();
		}
		break;

	case STAGE_FLOW_STATE::BOSS_ENTERING:
		Stage_Update_Player_Movement(elapsed_time);

		Game_Logic_Playing_Story_Update(elapsed_time);
		Boss_Update(elapsed_time);

		switch (Now_Playing_Stage)
		{
		case NOW_PLAYING_STAGE::STAGE_THREE:
			Stage_Three_Update(elapsed_time);
			break;

		case NOW_PLAYING_STAGE::STAGE_FOUR:
			Stage_Four_Update(elapsed_time);
			break;

		case NOW_PLAYING_STAGE::STAGE_FIVE:
			Stage_Five_Update(elapsed_time);
			break;
		}
		break;

	case STAGE_FLOW_STATE::GAMEPLAY_ACTIVE:
		Is_Stage_Paused();
		Stage_Update_Player_Movement(elapsed_time);
		Game_Logic_Update(elapsed_time);
		Boss_Update(elapsed_time);

		switch (Now_Playing_Stage)
		{
		case NOW_PLAYING_STAGE::STAGE_ONE:
			Stage_One_Update(elapsed_time);
			break;

		case NOW_PLAYING_STAGE::STAGE_TWO:
			Stage_Two_Update(elapsed_time);
			break;

		case NOW_PLAYING_STAGE::STAGE_THREE:
			Stage_Three_Update(elapsed_time);

			break;
		case NOW_PLAYING_STAGE::STAGE_FOUR:
			Stage_Four_Update(elapsed_time);
			break;

		case NOW_PLAYING_STAGE::STAGE_FIVE:
			Stage_Five_Update(elapsed_time);
			break;
		}

		if (!Player_Is_Alive())
		{
			if (Status_Is_Game_Over())
			{
				Set_Is_Paused_Menu(false);
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
			}
			else
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::PLAYER_DEAD);

			State_Timer = 0.0;
			return;
		}
		break;

	case STAGE_FLOW_STATE::PLAYER_DEAD:
		Is_Stage_Paused();

		Game_Logic_Player_Respawn_Update(elapsed_time);
		Status_Lose_Life();
		State_Timer += elapsed_time;
		if (State_Timer > 1.5)
		{
			Player_Status_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PLAYER_RESPAWNING);
			State_Timer = 0.0;
			Player_Spawn({ Player_Spawn_X, static_cast<float>(SCREEN_HEIGHT) });
			Stage_Command_Player_Move({ Player_Spawn_X, Player_Spawn_Y }, Player_Spawn_Speed);
		}
		break;

	case STAGE_FLOW_STATE::PLAYER_RESPAWNING:
		Is_Stage_Paused();
		Stage_Update_Player_Movement(elapsed_time);
		Game_Logic_Player_Respawn_Update(elapsed_time);

		if (!Stage_Is_Player_Moving_By_Script())
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAMEPLAY_ACTIVE);
		break;

	case STAGE_FLOW_STATE::STAGE_DONE:
		BGM_Fade_Timer = 0.0f;
		Is_Stage_Paused();

		Game_Logic_Player_Respawn_Update(elapsed_time);
		Player_Status_Reset();
		
		State_Timer += elapsed_time;
		if (State_Timer > 3.0f)
		{
			if (Are_All_Enemies_Cleared() || Status_Is_Game_Over())
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::CHECK_REMAINING);
		}
		break;

	case STAGE_FLOW_STATE::CHECK_REMAINING:
	{
		Game_Logic_Player_Respawn_Update(elapsed_time);

		BGM_Fade_Timer += elapsed_time;

		if (BGM_Fade_Timer > BGM_FADE_TIME)
		{
			Sound_M->Stop_BGM();
			Sound_M->Update_Current_BGM_Volume(Sound_M->Get_Target_BGM_Volume());

			if (Player_Is_Alive())
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_CLEAR);
			else
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_FAIL);

			State_Timer = 0.0;
		}

		float Target_Volume = Sound_M->Get_Target_BGM_Volume();
		Current_Volume = Target_Volume * (1.0f - static_cast<float>(BGM_Fade_Timer / BGM_FADE_TIME));

		if (Current_Volume < 0.0f)
			Current_Volume = 0.0f;

		Sound_M->Update_Current_BGM_Volume(Current_Volume);

		break;
	}

	case STAGE_FLOW_STATE::STAGE_CLEAR:
		Stage_Outro_Start();

		State_Timer = 0.0;
		Score_Set_Score();
		Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_CLEAR_ANIME);
		break;

	case STAGE_FLOW_STATE::STAGE_CLEAR_ANIME:
		Game_Logic_Player_Respawn_Update(elapsed_time);
		Stage_Outro_Update(elapsed_time);

		if (Is_Stage_Outro_Finished())
		{
			Fade_Start(2.0, true);

			if (Now_Playing_Stage == NOW_PLAYING_STAGE::STAGE_FIVE && Get_Stage_5_State() == STAGE_5_STATE::FAKE_CLEAR_SEQUENCE)
			{
				BGM_Fade_Timer = 0.0f;
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::FAKE_BOSS_FADE_WAIT);
			}
			else
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::PLAYER_EXIT);
		}
		break;

	case STAGE_FLOW_STATE::STAGE_FAIL:
		Game_Logic_Player_Respawn_Update(elapsed_time);

		Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAME_OVER);
		Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::PLAYER_DEAD);
		break;
	
	case STAGE_FLOW_STATE::GAME_OVER:
		Game_Logic_Player_Respawn_Update(elapsed_time);
	case STAGE_FLOW_STATE::GAME_PAUSED:
	{
		Game_Over_Update();

		if (Stage_Flow == STAGE_FLOW_STATE::GAME_PAUSED)
		{
			if (KeyLogger_IsTrigger(KK_BACK) || KeyLogger_IsTrigger(KK_ESCAPE) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_B) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_BACK))
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAMEPLAY_ACTIVE);
		}

		if (Get_Game_Over_Input() == GAME_OVER_MENU_BUFFER::DONE)
		{
			if (Get_Game_Over_Menu_Selected() == GAME_OVER_MENU_SELCETED::RE_START)
			{
				Sound_M->Stop_BGM();

				Stage_Reset_For_Restart();
				Score_Reset_Current_Stage_Score();
				Set_BGM_Current_Stage(Now_Playing_Stage);
				Set_Power_Current_Stage(Now_Playing_Stage);

				Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			}
			else if (Get_Game_Over_Menu_Selected() == GAME_OVER_MENU_SELCETED::GO_TO_MAIN)
			{
				if (!Is_Fading)
				{
					Fade_Start(0.5, true);
					Is_Fading = true;
				}

				if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
				{
					Stage_Reset_For_ALL();
					Score_Reset_Current_Stage_Score();

					Update_Main_Screen(Main_Screen::SELECT_GAME);
					Update_Sub_Screen(Sub_Screen::S_DONE);
					Update_Game_Select_Screen(Game_Select_Screen::GAME_MENU_SELECT);
					Update_Game_Select_Buffer(SELECT_GAME::SELECT_WAIT);

					Sound_M->Stop_BGM();
					float User_Set_Volume = Sound_M->Get_Target_BGM_Volume();
					Sound_M->Update_Current_BGM_Volume(User_Set_Volume);
					Sound_M->Play_BGM("Title");

					Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
					Set_Now_Stage_Flow(STAGE_FLOW_STATE::IDLE);
					Set_Now_Anime_Flow(FINAL_BOSS_ANIME::NONE);

					Fade_Start(0.5, false);
					Is_Fading = false;
				}
			}
		}
		break;
	}

	case STAGE_FLOW_STATE::PLAYER_EXIT:
		if (Player_Is_Alive())
			Stage_Command_Player_Move({ Player_Get_Position().x, -PLAYER_HEIGHT }, Player_Spawn_Speed * 2.0f);

		if (!Is_Fading)
		{
			Fade_Start(2.0, true);
			Is_Fading = true;
		}

		if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
		{
			float User_Set_Volume = Sound_M->Get_Target_BGM_Volume();
			Sound_M->Update_Current_BGM_Volume(User_Set_Volume);

			if (Get_Game_Mode() == GameMode::STAGE_SELECT)
			{
				Stage_Reset_For_ALL();

				Update_Main_Screen(Main_Screen::SELECT_GAME);
				Update_Sub_Screen(Sub_Screen::S_DONE);
				Update_Game_Select_Screen(Game_Select_Screen::GAME_MENU_SELECT);
				Update_Game_Select_Buffer(SELECT_GAME::SELECT_WAIT);

				Sound_M->Play_BGM("Title");

				Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::IDLE);
				Set_Now_Anime_Flow(FINAL_BOSS_ANIME::NONE);
			
				Fade_Start(1.5, false);
				Is_Fading = false;
				break;
			}

			Is_Player_Moving_By_Script = false;
			Player_Set_Enable(false);

			int Next_Stage_Num = static_cast<int>(Now_Playing_Stage) + 1;

			if (Now_Playing_Stage == NOW_PLAYING_STAGE::STAGE_FIVE)
			{
				Stage_Reset_For_ALL();
				Update_Game_Select_Screen(Game_Select_Screen::GAME_ENDING);
				Set_Ending_Status(ENDING_SEQUENCE::ENDING_START);
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_PLAYING_DONE);
			}
			else if (Next_Stage_Num == static_cast<int>(NOW_PLAYING_STAGE::STAGE_MAX)) // Defencive Code
			{
				Stage_Reset_For_ALL();
				Update_Game_Select_Screen(Game_Select_Screen::GAME_ENDING);
				Set_Ending_Status(ENDING_SEQUENCE::ENDING_START);
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_PLAYING_DONE);
			}
			else
				Set_Now_Playing_Stage(static_cast<NOW_PLAYING_STAGE>(Next_Stage_Num));

			Is_Fading = false;
			Fade_Start(0.5, false);
		}
		break;

	case STAGE_FLOW_STATE::STAGE_PLAYING_DONE:
		break;
	}
}

void Stage_Draw()
{
	if (Now_Playing_Stage == NOW_PLAYING_STAGE::STAGE_NONE)
		return;

	switch (Now_Playing_Stage)
	{
	case NOW_PLAYING_STAGE::STAGE_NONE:
	case NOW_PLAYING_STAGE::STAGE_MAX:
		break;

	case NOW_PLAYING_STAGE::STAGE_ONE:
		Stage_1_Background_Draw();
		break;

	case NOW_PLAYING_STAGE::STAGE_TWO:
		Stage_2_Background_Draw();
		break;

	case NOW_PLAYING_STAGE::STAGE_THREE:
		Stage_3_Background_Draw();
		break;

	case NOW_PLAYING_STAGE::STAGE_FOUR:
		Stage_4_Background_Draw();
		break;

	case NOW_PLAYING_STAGE::STAGE_FIVE:
		if (Is_Final_Boss_Entered())
			Stage_BOSS_Background_Draw();
		else
			Stage_5_Background_Draw();
		break;
	}
	STAGE_FLOW_STATE Stage_Flow = Get_Now_Stage_Flow();
	
	if (Stage_Flow != STAGE_FLOW_STATE::IDLE)
	{
		Game_Logic_Draw();
		
		if (Is_Blackout_Active)
			Stage_5_BlackOut_Draw();

		Story_Draw();

		if (!Is_Blackout_Active)
			Game_UI_And_Logo_Draw();
	}

	if (Stage_Flow == STAGE_FLOW_STATE::PREPARE_GAME)
		Stage_Ready_Draw();
	
	if (Stage_Flow == STAGE_FLOW_STATE::FAKE_BOSS_ENTERING)
		Stage_BOSS_Ready_Draw();

	if (Stage_Flow == STAGE_FLOW_STATE::STAGE_CLEAR_ANIME)
		Stage_Outro_Draw();

	if (Stage_Flow == STAGE_FLOW_STATE::GAME_OVER || Stage_Flow == STAGE_FLOW_STATE::GAME_PAUSED)
		Game_Over_Draw();
}

void Set_Blackout_Overlay(bool Is_Blackout)
{
	Is_Blackout_Active = Is_Blackout;
}

bool Is_BlackOut_Overlay()
{
	return Is_Blackout_Active;
}

bool Is_Final_Boss_Active()
{
	return Is_Final_BGM_Active;
}

void Set_Now_Playing_Stage(NOW_PLAYING_STAGE Stage)
{
	Stage_Reset_For_ALL();

	Now_Playing_Stage = Stage;

	GameMode Now_Game_Mode = Get_Game_Mode();
	if (Now_Game_Mode == GameMode::NEW_GAME || Now_Game_Mode == GameMode::CONTINUE)
		Player_Reset_For_Story();
	else
		Player_Spawn({ Player_Spawn_X, Player_Spawn_Y });

	Set_BGM_Current_Stage(Now_Playing_Stage);
	Set_Power_Current_Stage(Now_Playing_Stage);

	Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_SETUP);
}

void Set_BGM_Current_Stage(NOW_PLAYING_STAGE Stage)
{
	Sound_M->Stop_BGM();
	Sound_M->Play_SFX("Stage_Start");

	switch (Stage)
	{
	case NOW_PLAYING_STAGE::STAGE_ONE:
		Sound_M->Play_BGM("Stage_1_BGM");
		break;

	case NOW_PLAYING_STAGE::STAGE_TWO:
		Sound_M->Play_BGM("Stage_2_BGM");
		break;

	case NOW_PLAYING_STAGE::STAGE_THREE:
		Sound_M->Play_BGM("Stage_3_BGM");
		break;

	case NOW_PLAYING_STAGE::STAGE_FOUR:
		Sound_M->Play_BGM("Stage_4_BGM");
		break;

	case NOW_PLAYING_STAGE::STAGE_FIVE:
		Sound_M->Play_BGM("Stage_5_BGM");
		break;
	}

	Sound_M->Update_Current_BGM_Volume(0.0f);
	BGM_Fade_Timer = 0.0;
	Is_BGM_Fading_In = true;
}

void Set_BGM_For_Boss(NOW_PLAYING_STAGE Stage)
{
	Sound_M->Stop_BGM();
	
	switch (Stage)
	{
	case NOW_PLAYING_STAGE::STAGE_THREE:
		Sound_M->Play_BGM("Stage_3_Boss_BGM");
		break;
		
	case NOW_PLAYING_STAGE::STAGE_FOUR:
		Sound_M->Play_BGM("Stage_4_Boss_BGM");
		break;
		
	case NOW_PLAYING_STAGE::STAGE_FIVE:
		if (Get_Stage_5_State() == STAGE_5_STATE::BOSS_APPEARANCE)
			Sound_M->Play_BGM("Stage_5_Boss_Phase_1_BGM");

		if (Stage_Flow == STAGE_FLOW_STATE::FAKE_BOSS_ENTERING)
			Sound_M->Play_BGM("Stage_5_Boss_Phase_2_BGM");
		break;
	}

	Sound_M->Update_Current_BGM_Volume(0.0f);
	BGM_Fade_Timer = 0.0;
	Is_BGM_Fading_In = true;
}

void Set_Power_Current_Stage(NOW_PLAYING_STAGE Stage)
{
	switch (Stage)
	{
	case NOW_PLAYING_STAGE::STAGE_ONE:
	case NOW_PLAYING_STAGE::STAGE_TWO:
		Status_Add_Power(A_Origin);
		break;
	case NOW_PLAYING_STAGE::STAGE_THREE:
		Status_Add_Power(A_Double);
		break;
	case NOW_PLAYING_STAGE::STAGE_FOUR:
	case NOW_PLAYING_STAGE::STAGE_FIVE:
		Status_Add_Power(PLAYER_POWER_MAX);
		break;
	}
}

NOW_PLAYING_STAGE Get_Now_Playing_Stage()
{
	return Now_Playing_Stage;
}

void Set_Now_Stage_Flow(STAGE_FLOW_STATE State)
{
	Stage_Flow = State;
}

STAGE_FLOW_STATE Get_Now_Stage_Flow()
{
	return Stage_Flow;
}

void Stage_Command_Player_Move(const DirectX::XMFLOAT2& targetPos, float speed)
{
	Player_Status_Reset();

	Is_Player_Moving_By_Script = true;
	Player_Move_Target = targetPos;
	Player_Move_Speed = speed;
}

void Stage_Update_Player_Movement(double elapsed_time)
{
	if (!Is_Player_Moving_By_Script) return;

	DirectX::XMFLOAT2 currentPos = Player_Get_Position();
	float moveAmount = Player_Move_Speed * static_cast<float>(elapsed_time);

	if (currentPos.x < Player_Move_Target.x) { currentPos.x += moveAmount;

	if (currentPos.x > Player_Move_Target.x) currentPos.x = Player_Move_Target.x; }
	else if (currentPos.x > Player_Move_Target.x) { currentPos.x -= moveAmount;

	if (currentPos.x < Player_Move_Target.x) currentPos.x = Player_Move_Target.x; }

	if (currentPos.y < Player_Move_Target.y) { currentPos.y += moveAmount;

	if (currentPos.y > Player_Move_Target.y) currentPos.y = Player_Move_Target.y; }
	else if (currentPos.y > Player_Move_Target.y) { currentPos.y -= moveAmount;

	if (currentPos.y < Player_Move_Target.y) currentPos.y = Player_Move_Target.y; }

	Player_Set_Position(currentPos);

	if (abs(currentPos.x - Player_Move_Target.x) < 1.0f && abs(currentPos.y - Player_Move_Target.y) < 1.0f)
	{
		Player_Set_Position(Player_Move_Target);
		Is_Player_Moving_By_Script = false;
	}
}

bool Stage_Is_Player_Moving_By_Script()
{
	return Is_Player_Moving_By_Script;
}

void Stage_Set_Is_Player_Moving_By_Script(bool Is_Move)
{
	Is_Player_Moving_By_Script = Is_Move;
}

bool Are_All_Enemies_Cleared()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (Enemy_IsEnable(i))
			return false;
	}
	return true; 
}

void Stage_Reset_For_ALL()
{
	Game_Logic_Finalize();
	Game_Logic_Initialize();

	Score_Reset_All_Score();
	Boss_Initialize();
	Story_Reset();
	Stage_Update_Reset();

	Stage_Ready_Reset();
	Stage_Over_Draw_Reset();

	Prepare_Game_Timer = 0.0;
	State_Timer = 0.0;
	Stage_Timer_Reset();

	Is_Battle_Start_Sound_Played = false;
	Is_Fading = false;
	BGM_Fade_Timer = 0.0;
	Is_BGM_Fading_In = false;
	Is_Stage_Start = false;

	Is_Blackout_Active = false;
	Is_Final_BGM_Active = false;

	Player_Spawn({ Player_Spawn_X, Player_Spawn_Y });
	Status_Start_Respawn_Invincibility();
}

void Stage_Reset_For_Restart()
{
	Story_Trigger_Saver();

	Status_Initialize();
	Score_Reset_Current_Stage_Score();

	Enemy_Spawner_Reset();
	Boss_Initialize();
	Item_Initialize();
	Bullet_Initialize();
	Enemy_Initialize();
	Enemy_Bullet_Initialize();
	Effect_Initialize();

	Stage_Update_Reset();
	Stage_Ready_Reset();

	Story_Trigger_Loader();

	Prepare_Game_Timer = 0.0;
	State_Timer = 0.0;
	Stage_Timer_Reset();

	Is_Battle_Start_Sound_Played = false;
	Is_Fading = false;

	Is_Blackout_Active = false;
	Is_Final_BGM_Active = false;

	Player_Status_Reset();
	Player_Spawn({ Player_Spawn_X, Player_Spawn_Y });
	Status_Start_Respawn_Invincibility();
}

double Get_In_Game_Timer()
{
	return State_Timer;
}

void Is_Stage_Paused()
{
	if (KeyLogger_IsTrigger(KK_BACK) || KeyLogger_IsTrigger(KK_ESCAPE) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_BACK))
	{
		Set_Is_Paused_Menu(true);
		Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::RETRY);
		Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAME_PAUSED);
		return;
	}
}
