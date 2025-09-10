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

using namespace PALETTE;
using namespace DirectX;

NOW_PLAYING_STAGE Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
static STAGE_FLOW_STATE	Stage_Flow  = STAGE_FLOW_STATE::IDLE;

static double Prepare_Game_Timer = 0.0;
static double State_Timer = 0.0;

static bool Is_Battle_Start_Sound_Played = false;

static bool Is_Player_Moving_By_Script = false;
static XMFLOAT2 Player_Move_Target;
static float Player_Move_Speed;

static int Current_Volume = 0;
static double BGM_Fade_Timer = 0.0;
constexpr double BGM_FADE_TIME = 3.0;
static bool Is_BGM_Fading_In = false;
static bool Is_Fading = false;

static bool Is_Boss_Story = false;
static bool Is_Boss_Spawned = false;

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

	Current_Volume = 0;
	BGM_Fade_Timer = 0.0;
	Is_BGM_Fading_In = false;
	Is_Fading = false;
	Is_Boss_Spawned = false;
}

void Stage_Finalize()
{
	Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
	Stage_Flow = STAGE_FLOW_STATE::IDLE;

	Player_Spawn_X = 0;
	Player_Spawn_Y = 0;
	Player_Spawn_Speed = 150.f;

	Current_Volume = 0;
	BGM_Fade_Timer = 0.0;

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
		Current_Volume = static_cast<int>(SOUND_MAX * (BGM_Fade_Timer / BGM_FADE_TIME));

		if (Current_Volume > SOUND_MAX)
			Current_Volume = SOUND_MAX;

		SM->Set_BGM_Volume(Current_Volume);

		if (BGM_Fade_Timer >= BGM_FADE_TIME)
			Is_BGM_Fading_In = false;
	}

	if (Now_Playing_Stage == NOW_PLAYING_STAGE::STAGE_NONE) return;

	Status_Update(elapsed_time);
	Stage_Update_Player_Movement(elapsed_time);

	GameMode Now_Mode = Get_Game_Mode();

	switch (Stage_Flow)
	{
	case STAGE_FLOW_STATE::IDLE:
		// Do nothing, Wait for stage selection
		break;

	case STAGE_FLOW_STATE::STORY_PLAYING:
		if (Fade_GetState() == FADE_STATE::FINISHED_IN)
		{
			Story_Update(elapsed_time);

			if (Story_Is_Finished())
			{
				switch (Now_Playing_Stage)
				{
				case NOW_PLAYING_STAGE::STAGE_FOUR:
					Stage_Command_Player_Move(Stage_4_Boss_Start_Pos, Player_Spawn_Speed * A_Double);
					Set_Now_Stage_Flow(STAGE_FLOW_STATE::BOSS_ENTERING);
					Is_Boss_Spawned = false;
					break;
				case NOW_PLAYING_STAGE::STAGE_FIVE:
					Stage_Command_Player_Move(Stage_5_Boss_Start_Pos, Player_Spawn_Speed * A_Double);
					Set_Now_Stage_Flow(STAGE_FLOW_STATE::BOSS_ENTERING);
					Is_Boss_Spawned = false;
					break;

				default:
					Stage_Ready_Reset();
					Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
					Prepare_Game_Timer = 0.0;
					break;
				}
			}
		}
		break;

	case STAGE_FLOW_STATE::BOSS_ENTERING:
		if (!(Stage_Is_Player_Moving_By_Script()))
			Game_Logic_Playing_Story_Update(elapsed_time);

		if (!Is_Boss_Spawned)
		{
			int Boss_Index = 0;

			switch (Now_Playing_Stage)
			{
			case NOW_PLAYING_STAGE::STAGE_THREE:
			{
				XMFLOAT2 Boss_Target_POS = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (Enemy_Get_Width(Enemy_Type_Special_Boss_Stage_3, true) * A_Half)),
							Game_Offset.y + (Enemy_Get_Width(Enemy_Type_Special_Boss_Stage_3, false) * A_Quarter) };

				Boss_Index = Enemy_Create(Enemy_Type_Special_Boss_Stage_3, { Boss_Target_POS.x, -Enemy_Get_Width(Enemy_Type_Special_Boss_Stage_3, false) },
					Enemy_Move_Pattern::NONE, {}, Boss_Target_POS);
				break;
			}
			case NOW_PLAYING_STAGE::STAGE_FOUR:
			{
				XMFLOAT2 Boss_Target_POS = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (Enemy_Get_Width(Enemy_Type_Middle_Boss_Stage_4, true) * A_Half)),
							Game_Offset.y + (Enemy_Get_Width(Enemy_Type_Middle_Boss_Stage_4, false) * A_Point_Fifteen) };
					
				Boss_Index = Enemy_Create(Enemy_Type_Middle_Boss_Stage_4, { Boss_Target_POS.x, -Enemy_Get_Width(Enemy_Type_Middle_Boss_Stage_4, false) },
					Enemy_Move_Pattern::NONE, {}, Boss_Target_POS);
				break;
			}
			case NOW_PLAYING_STAGE::STAGE_FIVE:
				XMFLOAT2 Boss_Target_POS = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (Enemy_Get_Width(Enemy_Type_Final_Phase_1_Boss, true) * A_Half)),
							Game_Offset.y + (Enemy_Get_Width(Enemy_Type_Final_Phase_1_Boss, false) * A_One_Tenth) };

				Boss_Index = Enemy_Create(Enemy_Type_Final_Phase_1_Boss, { Boss_Target_POS.x, -Enemy_Get_Width(Enemy_Type_Final_Phase_1_Boss, false) },
					Enemy_Move_Pattern::NONE, {}, Boss_Target_POS);
				break;
			}


			if (Boss_Index != -1)
			{
				Set_Active_Boss_Index(Boss_Index);
				Boss_Activate(Now_Playing_Stage);

				SM->Stop_BGM();

				switch (Now_Playing_Stage)
				{
				case NOW_PLAYING_STAGE::STAGE_THREE:
					SM->Play_BGM("Stage_3_Boss_BGM");
					break;

				case NOW_PLAYING_STAGE::STAGE_FOUR:
					SM->Play_BGM("Stage_4_Boss_BGM");
					break;

				case NOW_PLAYING_STAGE::STAGE_FIVE:
					SM->Play_BGM("Stage_5_BGM");
					break;
				}

				SM->Set_BGM_Volume(0);
				BGM_Fade_Timer = 0.0;
				Is_BGM_Fading_In = true;

				Is_Boss_Story = true;
				Set_Story_State(Story_Manager_State::IN_ACTIVE);
				Story_Start(Now_Playing_Stage);
			}
			Is_Boss_Spawned = true;
		}

		Story_Update(elapsed_time);
		Boss_Update(elapsed_time);

		if (Story_Is_Finished())
		{
			Set_Boss_State(BOSS_STATE::BATTLE);
			Stage_Ready_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Prepare_Game_Timer = 0.0;
		}
		break;

	case STAGE_FLOW_STATE::PREPARE_GAME:
		if (Get_Boss_State() != BOSS_STATE::INACTIVE)
			Boss_Update(elapsed_time);

		Is_Player_Moving_By_Script = false;

		if (!Is_Battle_Start_Sound_Played)
		{
			SM->Play_SFX("Stage_Battle_Start");
			Is_Battle_Start_Sound_Played = true;
		}

		Prepare_Game_Timer += elapsed_time;

		if (Prepare_Game_Timer > 1.0)
		{
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAMEPLAY_ACTIVE);
			Stage_Timer_Reset();
			BGM_Fade_Timer = 0.0;
			Current_Volume = 0;
		}
		break;

	case STAGE_FLOW_STATE::GAMEPLAY_ACTIVE:
		Is_Stage_Paused();

		Game_Logic_Update(elapsed_time);

		if (Get_Boss_State() != BOSS_STATE::INACTIVE)
			Boss_Update(elapsed_time);

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

		if (Is_Boss_Defeated())
		{
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::BOSS_DEAD);
			State_Timer = 0.0;
			break;
		}

		if (Is_Ready_To_Logic_Draw())
		{
			switch (Now_Playing_Stage)
			{
			case NOW_PLAYING_STAGE::STAGE_NONE:
				break;

			case NOW_PLAYING_STAGE::STAGE_ONE:
				Stage_One_Update(elapsed_time);
				break;

			case NOW_PLAYING_STAGE::STAGE_TWO:
				Stage_Two_Update(elapsed_time);
				break;

			case NOW_PLAYING_STAGE::STAGE_THREE:
				if (Get_Boss_State() == BOSS_STATE::INACTIVE)
					Stage_Three_Update(elapsed_time);
				break;
			}
		}
		break;

	case STAGE_FLOW_STATE::BOSS_DEAD:
		Game_Logic_Update(elapsed_time);
		BGM_Fade_Timer = 0.0;

		State_Timer += elapsed_time;
		if (State_Timer > 3.0f)
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
		break;

	case STAGE_FLOW_STATE::PLAYER_DEAD:
		Is_Stage_Paused();

		Game_Logic_Player_Respawn_Update(elapsed_time);
		Status_Lose_Life();
		State_Timer += elapsed_time;
		if (State_Timer > 1.5)
		{
			Player_Set_State(PLAYER_STATE::NONE);
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PLAYER_RESPAWNING);
			State_Timer = 0.0;
			Player_Spawn({ Player_Spawn_X, static_cast<float>(SCREEN_HEIGHT) });
			Stage_Command_Player_Move({ Player_Spawn_X, Player_Spawn_Y }, Player_Spawn_Speed);
		}
		break;

	case STAGE_FLOW_STATE::PLAYER_RESPAWNING:
		Is_Stage_Paused();

		Game_Logic_Player_Respawn_Update(elapsed_time);

		if (!Stage_Is_Player_Moving_By_Script())
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAMEPLAY_ACTIVE);
		break;

	case STAGE_FLOW_STATE::STAGE_DONE:
		Is_Stage_Paused();

		Game_Logic_Player_Respawn_Update(elapsed_time);
		Player_Set_State(PLAYER_STATE::NONE);
		Player_Set_Avoid_State(false);
		
		State_Timer += elapsed_time;
		if (State_Timer > 3.0f)
		{
			if (Are_All_Enemies_Cleared() || Status_Is_Game_Over())
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::CHECK_REMAINING);
		}
		break;

	case STAGE_FLOW_STATE::CHECK_REMAINING:
		Game_Logic_Player_Respawn_Update(elapsed_time);

		BGM_Fade_Timer += elapsed_time;

		if (BGM_Fade_Timer > BGM_FADE_TIME)
		{
			SM->Stop_BGM();
			SM->Set_BGM_Volume(SOUND_MAX);

			if (Player_Is_Alive())
			{
				GameMode Now_Mode = Get_Game_Mode();
				if (Now_Mode == GameMode::NEW_GAME || Now_Mode == GameMode::CONTINUE)
					Save_Data_Update_Stage(static_cast<int>(Now_Playing_Stage));
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_CLEAR);
			}
			else
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_FAIL);

			State_Timer = 0.0;
		}

		Current_Volume = static_cast<int>(SOUND_MAX * (A_Origin - (BGM_Fade_Timer / BGM_FADE_TIME)));

		if (Current_Volume < 0)
			Current_Volume = 0;

		SM->Set_BGM_Volume(Current_Volume);

		break;

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
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PLAYER_EXIT);
		break;

	case STAGE_FLOW_STATE::STAGE_FAIL:
		Game_Logic_Player_Respawn_Update(elapsed_time);

		Set_Now_Stage_Flow(STAGE_FLOW_STATE::GAME_OVER);
		Set_Game_Over_Input(GAME_OVER_MENU_BUFFER::PLAYER_DEAD);
		break;
	
	case STAGE_FLOW_STATE::GAME_OVER:
			Game_Logic_Player_Respawn_Update(elapsed_time);
	case STAGE_FLOW_STATE::GAME_PAUSED:
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
				SM->Stop_BGM();

				Stage_Reset_For_Retry();
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
					Stage_Reset_For_Retry();
					Score_Reset_Current_Stage_Score();

					Update_Main_Screen(Main_Screen::SELECT_GAME);
					Update_Sub_Screen(Sub_Screen::S_DONE);
					Update_Game_Select_Screen(Game_Select_Screen::GAME_MENU_SELECT);
					Update_Game_Select_Buffer(SELECT_GAME::SELECT_WAIT);

					SM->Stop_BGM();
					SM->Play_BGM("Title");
					SM->Set_BGM_Volume(SOUND_MAX);

					Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE; 
					Set_Now_Stage_Flow(STAGE_FLOW_STATE::IDLE);
		
					Fade_Start(0.5, false);
					Is_Fading = false;
				}
			}
		}
		break;

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
			if (Now_Mode == GameMode::STAGE_SELECT)
			{
				Stage_Reset_For_Retry();

				Update_Main_Screen(Main_Screen::SELECT_GAME);
				Update_Sub_Screen(Sub_Screen::S_DONE);
				Update_Game_Select_Screen(Game_Select_Screen::GAME_MENU_SELECT);
				Update_Game_Select_Buffer(SELECT_GAME::SELECT_WAIT);

				SM->Play_BGM("Title");
				SM->Set_BGM_Volume(SOUND_MAX);

				Now_Playing_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::IDLE);

				Fade_Start(1.5, false);
				Is_Fading = false;
				break;
			}

			Is_Player_Moving_By_Script = false;
			Player_Set_Enable(false);

			int Next_Stage_Num = static_cast<int>(Now_Playing_Stage) + 1;

			if (Now_Playing_Stage == NOW_PLAYING_STAGE::STAGE_FIVE)
			{
				Stage_Reset_For_Retry();
				Update_Game_Select_Screen(Game_Select_Screen::GAME_ENDING);
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_PLAYING_DONE);
			}
			else if (Next_Stage_Num == static_cast<int>(NOW_PLAYING_STAGE::STAGE_MAX)) // Defencive Code
			{
				Stage_Reset_For_Retry();
				Update_Game_Select_Screen(Game_Select_Screen::GAME_ENDING);
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
		Stage_5_Background_Draw();
		break;
	}
	STAGE_FLOW_STATE Stage_Flow = Get_Now_Stage_Flow();

	if (Stage_Flow == STAGE_FLOW_STATE::STORY_PLAYING)
	{
		Player_Draw();
		Story_Draw();
	}
	else if (Stage_Flow == STAGE_FLOW_STATE::BOSS_ENTERING)
	{
		Game_Logic_Draw();
		Story_Draw();
	}
	else if (Stage_Flow != STAGE_FLOW_STATE::IDLE)
		Game_Logic_Draw();

	if (Stage_Flow == STAGE_FLOW_STATE::PREPARE_GAME || Stage_Flow == STAGE_FLOW_STATE::GAMEPLAY_ACTIVE)
		Stage_Ready_Draw();

	if (!(Get_Stage_5_State() == STAGE_5_STATE::BOSS_PHASE_2_BATTLE))
		Game_UI_And_Logo_Draw();

	if (Stage_Flow == STAGE_FLOW_STATE::STAGE_CLEAR_ANIME)
		Stage_Outro_Draw();

	if (Stage_Flow == STAGE_FLOW_STATE::GAME_OVER || Stage_Flow == STAGE_FLOW_STATE::GAME_PAUSED)
		Game_Over_Draw();
}

void Set_Now_Playing_Stage(NOW_PLAYING_STAGE Stage)
{
	Stage_Reset_For_Retry();

	GameMode Now_Mode = Get_Game_Mode();
	Now_Playing_Stage = Stage;

	Player_Reset_For_Story();
	Set_BGM_Current_Stage(Now_Playing_Stage);

	Is_Boss_Story = false;

	Set_Power_Current_Stage(Now_Playing_Stage);

	if (Now_Playing_Stage == NOW_PLAYING_STAGE::STAGE_THREE)
		Status_Add_Power(A_Origin); // Give Power 1.0


	if (Now_Mode == GameMode::NEW_GAME || Now_Mode == GameMode::CONTINUE)
	{
		// If New Game OR Continue Mode, Play Story
		Set_Now_Stage_Flow(STAGE_FLOW_STATE::STORY_PLAYING);
		Set_Story_State(Story_Manager_State::IN_ACTIVE);
		Story_Start(Now_Playing_Stage);
	}
	else // If STAGE_SELECT Mode
	{
		// Skip Story
		Score_Reset_Current_Stage_Score();
		Player_Spawn({ Player_Spawn_X, Player_Spawn_Y });

		switch (Now_Playing_Stage)
		{
		case NOW_PLAYING_STAGE::STAGE_FOUR:
		case NOW_PLAYING_STAGE::STAGE_FIVE:
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::BOSS_ENTERING);
			break;
		default:
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			break;
		}
		Prepare_Game_Timer = 0.0;
		Status_Start_Respawn_Invincibility();
	}
}

void Set_BGM_Current_Stage(NOW_PLAYING_STAGE Stage)
{
	SM->Play_SFX("Stage_Start");

	switch (Stage)
	{
	case NOW_PLAYING_STAGE::STAGE_ONE:
		SM->Play_BGM("Stage_1_BGM");
		break;

	case NOW_PLAYING_STAGE::STAGE_TWO:
		SM->Play_BGM("Stage_2_BGM");
		break;

	case NOW_PLAYING_STAGE::STAGE_THREE:
		SM->Play_BGM("Stage_3_BGM");
		break;

	case NOW_PLAYING_STAGE::STAGE_FOUR:
		SM->Play_BGM("Stage_4_BGM");
		break;

	case NOW_PLAYING_STAGE::STAGE_FIVE:
		SM->Play_BGM("Stage_5_BGM");
		break;
	}

	SM->Set_BGM_Volume(0);
	BGM_Fade_Timer = 0.0;
	Is_BGM_Fading_In = true;
}

void Set_Power_Current_Stage(NOW_PLAYING_STAGE Stage)
{
	switch (Stage)
	{
	case NOW_PLAYING_STAGE::STAGE_ONE:
	case NOW_PLAYING_STAGE::STAGE_TWO:
		Status_Add_Power(A_T_Quarters);
		break;
	case NOW_PLAYING_STAGE::STAGE_THREE:
		Status_Add_Power(A_One_A_T_Quarters);
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
	if (State == STAGE_FLOW_STATE::STORY_PLAYING && Get_Boss_State() == BOSS_STATE::STORY)
	{
		Is_Boss_Story = true;
		Set_Story_State(Story_Manager_State::IN_ACTIVE);
		Story_Start(Get_Now_Playing_Stage());
	}
	else
		Is_Boss_Story = false;

	Stage_Flow = State;
}

STAGE_FLOW_STATE Get_Now_Stage_Flow()
{
	return Stage_Flow;
}

bool Is_Boss_Story_Time()
{
	return Is_Boss_Story;
}

void Stage_Command_Player_Move(const DirectX::XMFLOAT2& targetPos, float speed)
{
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


void Stage_Reset_For_Retry()
{
	Enemy_Spawner_Reset();

	Boss_Initialize();

	Status_Initialize();

	Game_Logic_Finalize();
	Game_Logic_Initialize();

	Stage_Ready_Reset();
	Stage_Over_Draw_Reset();

	Prepare_Game_Timer = 0.0;
	State_Timer = 0.0;

	Stage_Timer_Reset();
	Stage_Update_Reset();

	Is_Battle_Start_Sound_Played = false;
	Is_Fading = false;
	BGM_Fade_Timer = 0.0;
	Is_BGM_Fading_In = false;
	Is_Boss_Spawned = false;

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
