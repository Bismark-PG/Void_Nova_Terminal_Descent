
/*==============================================================================

	Game Stage Select Menu [Game_Select.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Game_Select.h"
#include "Game_Window.h"
#include "Game_Intro_Manager.h"
#include "Game_Mode_Manager.h" 
#include "Save_Data.h"

//-------------------UI Texture-------------------//
static int BG_Select = -1;

static int UI_NEW = -1;
static int UI_CONTINUE = -1;
static int UI_STAGE = -1;
static int UI_SELECT = -1;
static int UI_BACK = -1;
static int UI_BUTTON = -1;

static int Main_TexID = -1;
static int Main_Anime_ID = -1;
static int Play_Main_TexID = -1;

//-------------------Num Texture-------------------//
static int Stage_1 = -1;
static int Stage_2 = -1;
static int Stage_3 = -1;
static int Stage_4 = -1;
static int Stage_5 = -1;

static int Now_Stage_TexID = -1;

SELECT_GAME Game_Select_Buffer;
SELECT_STAGE_STATE Stage_Select_Buffer;
int Stage_Select_Buffer_State;

static bool Is_Stage_Selected = false;
static bool Is_Play_Selected  = false;
static bool Is_Fading_Started = false;

void Game_Select_Initialize()
{
	Update_Game_Select_Screen(Game_Select_Screen::G_WAIT);

	Game_Select_Texture();

	if (Get_Mode_Buffer() == Get_Mode_Pre_Buffer())
	{
		Stage_Select_Buffer_State = STAGE_NUM::STAGE_1;
		Game_Select_Buffer  = SELECT_GAME::NONE;
		Stage_Select_Buffer = SELECT_STAGE_STATE::NONE;
		Now_Stage_TexID = Stage_1;
	}

	S_TITLE_WIDTH *= S_TITLE_SCALE;

	S_Title_Width  = S_TITLE_WIDTH * Game_Scale;
	S_Title_Height = S_TITLE_WIDTH * Game_Scale;
	S_Title_X = (BG_WIDTH  * A_Half) - (S_Title_Width  * A_Half);
	S_Title_Y = (BG_HEIGHT * A_Half) - (S_Title_Height * A_Half);

	S_UI_WIDTH  *= S_UI_SCALE;
	S_UI_HEIGHT *= S_UI_SCALE;

	S_UI_X = BG_WIDTH - (BG_WIDTH * 0.85f);

	S_UI_Width  = S_UI_WIDTH  * Game_Scale;
	S_UI_Height = S_UI_HEIGHT * Game_Scale;

	UI_New_Game_Y = (BG_HEIGHT * 0.3f)  - (S_UI_Height * A_Half);
	UI_Continue_Y = (BG_HEIGHT * 0.45f) - (S_UI_Height * A_Half);
	UI_Stage_Y	  = (BG_HEIGHT * 0.6f)  - (S_UI_Height * A_Half);
	UI_Back_Y	  = (BG_HEIGHT * 0.8f)  - (S_UI_Height * A_Half);

	UI_New_Game_Width = S_UI_Width * UI_New_Game_Scale;
	UI_Continue_Width = S_UI_Width * UI_Continue_Scale;
	UI_Stage_Width	  = S_UI_Width * UI_Stage_Scale;
	UI_Back_Width	  = S_UI_Width * UI_Back_Scale;

	UI_Select_X = S_UI_X + (UI_Stage_Width * 1.2f);

	Player_Intro_Width  = PLAYER_INTRO_WIDTH * Player_Intro_Scale;
	Player_Intro_Height = PLAYER_INTRO_HEIGHT * Player_Intro_Scale;
	Player_Intro_Width  *= Game_Scale;
	Player_Intro_Height *= Game_Scale;
	Player_Intro_X = BG_WIDTH  - (Player_Intro_Width  * A_F_Fifths);
	Player_Intro_Y = BG_HEIGHT - (Player_Intro_Height * A_F_Fifths);

	float Screen_Diagonal = sqrtf(powf(BG_WIDTH, 2.0f) + powf(BG_HEIGHT, 2.0f));
	S_BG_Width  = Screen_Diagonal;
	S_BG_Height = Screen_Diagonal;

	S_BG_X = -((S_BG_Width	- BG_WIDTH)  * A_Half);
	S_BG_Y = -((S_BG_Height - BG_HEIGHT) * A_Half);

	STAGE_NUM_WIDTH  *= STAGE_NUM_SCALE;
	STAGE_NUM_HEIGHT *= STAGE_NUM_SCALE;

	Stage_Num_Width  = STAGE_NUM_WIDTH  * Game_Scale;
	Stage_Num_Height = STAGE_NUM_HEIGHT * Game_Scale;

	Stage_Num_X = S_UI_X + (UI_Stage_Width * A_One_And_Half) - (Stage_Num_Width * A_Half);
	Stage_Num_Y = (BG_HEIGHT * A_Six_Tenths) - (Stage_Num_Height * A_Half);

	Stage_Up_Y	 = Stage_Num_Y - (Stage_Num_Height * A_One_A_Quarter);
	Stage_Down_Y = Stage_Num_Y + (Stage_Num_Height * A_One_A_Quarter);
}

void Game_Select_Finalize()
{
	S_TITLE_WIDTH  = 16;
	S_TITLE_HEIGHT = 9;

	S_UI_WIDTH = 5;
	S_UI_HEIGHT = 1;

	STAGE_NUM_WIDTH = 1;
	STAGE_NUM_HEIGHT = 2;

	PLAYER_INTRO_WIDTH = 5;
	PLAYER_INTRO_HEIGHT = 4;

	Player_Intro_Width  = 0;
	Player_Intro_Height = 0;
	Player_Intro_X = 0;
	Player_Intro_Y = 0;

	S_Title_X = 0;
	S_Title_Y = 0;
	S_Title_Width  = 0;
	S_Title_Height = 0;

	Expend_Scale = 0;
	BG_Angle = 0;

	S_BG_X = 0;
	S_BG_Y = 0;
	S_BG_Width  = 0;
	S_BG_Height = 0;

	S_UI_X = 0;
	S_UI_Width  = 0;
	S_UI_Height = 0;

	UI_Select_X = 0;

	UI_New_Game_Y = 0;
	UI_Continue_Y = 0;
	UI_Stage_Y = 0;
	UI_Back_Y  = 0;

	UI_New_Game_Width = 0;
	UI_Continue_Width = 0;
	UI_Stage_Width = 0;
	UI_Back_Width  = 0;

	Stage_Num_X  = 0;
	Stage_Num_Y  = 0;
	Stage_Up_Y   = 0;
	Stage_Down_Y = 0;
	Stage_Num_Width  = 0;
	Stage_Num_Height = 0;
}

void Game_Select_Update()
{
	if (Is_Stage_Selected)
	{
		switch (Stage_Select_Buffer)
		{
		case SELECT_STAGE_STATE::NONE:
			break;

		case SELECT_STAGE_STATE::STAGE_WAIT:
			if (!Is_Play_Selected && KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
			{
				int Highest_stage = Save_Data_Get_High_Stage();

#if defined(DEBUG) || defined(_DEBUG)
				Highest_stage = 5; // Debug Code
#endif

				if (Stage_Select_Buffer_State <= Highest_stage)
				{
					Set_Game_Mode(GameMode::STAGE_SELECT);
					Update_Game_Select_Buffer(SELECT_GAME::FADING_TO_STAGE);
					Update_Select_Stage_State(SELECT_STAGE_STATE::STAGE_SELECT);
					Sound_M->Play_SFX("Buffer_Select");
					Sound_M->Stop_BGM("Title");

					Is_Play_Selected = true;
				}
				else
					Sound_M->Play_SFX("Buffer_Back");

			}

			if (KeyLogger_IsTrigger(KK_BACK) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_B))
			{
				Update_Game_Select_Buffer(SELECT_GAME::SELECT_STAGE);
				Update_Select_Stage_State(SELECT_STAGE_STATE::DONE);
				Sound_M->Play_SFX("Buffer_Back");

				Is_Stage_Selected = false;    
			}

			if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
			{
				if (Stage_Select_Buffer_State <= STAGE_NUM::STAGE_1)
					Stage_Select_Buffer_State = STAGE_NUM::STAGE_1;
				else
				{
					Sound_M->Play_SFX("Buffer_Move");
					Stage_Select_Buffer_State--;
				}
			}
			else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				if (Stage_Select_Buffer_State >= STAGE_NUM::STAGE_5)
					Stage_Select_Buffer_State = STAGE_NUM::STAGE_5;
				else
				{
					Stage_Select_Buffer_State++;
					Sound_M->Play_SFX("Buffer_Move");
				}
			}

			Stage_TexID(Stage_Select_Buffer_State);
			break;

		case SELECT_STAGE_STATE::STAGE_SELECT:
		case SELECT_STAGE_STATE::DONE:
			Is_Play_Selected = false;
			break;
		}
	}

	switch (Game_Select_Buffer)
	{
	case SELECT_GAME::NONE:
		if (Game_M.Get_Current_Main_Screen() == Main_Screen::SELECT_GAME)
			Update_Game_Select_Buffer(SELECT_GAME::INTRO_WAIT);
		break;

	case SELECT_GAME::INTRO_WAIT:
	case SELECT_GAME::BACK_WAIT:
		break;

	case SELECT_GAME::SELECT_WAIT:
		if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_BACK);
			Sound_M->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_NEW_GAME);
			Sound_M->Play_SFX("Buffer_Move");
		}
		break;

	case SELECT_GAME::SELECT_NEW_GAME:
		if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Set_Game_Mode(GameMode::NEW_GAME);
			Update_Game_Select_Buffer(SELECT_GAME::FADING_TO_STAGE);
			Update_Select_Stage_State(SELECT_STAGE_STATE::STAGE_SELECT);

			Update_Stage_Select_Buffer_State(STAGE_NUM::STAGE_1);

			Sound_M->Play_SFX("Buffer_Select");
			Sound_M->Stop_BGM("Title");
		}

		if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_CONTINUE);
			Sound_M->Play_SFX("Buffer_Move");
		}
		break;

	case SELECT_GAME::SELECT_CONTINUE:
		if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Set_Game_Mode(GameMode::CONTINUE);
			Update_Game_Select_Buffer(SELECT_GAME::FADING_TO_STAGE);
			Update_Select_Stage_State(SELECT_STAGE_STATE::STAGE_SELECT);

			int Highest_stage = Save_Data_Get_High_Stage();

#if defined(DEBUG) || defined(_DEBUG)
			Highest_stage = 5; // Debug Code
#endif

			int Start_To_Continue = std::min(Highest_stage, static_cast<int>(STAGE_NUM::STAGE_5));

			Update_Stage_Select_Buffer_State(static_cast<STAGE_NUM>(Start_To_Continue));

			Sound_M->Play_SFX("Buffer_Select");
			Sound_M->Stop_BGM("Title");
		}

		if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_NEW_GAME);
			Sound_M->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_STAGE);
			Sound_M->Play_SFX("Buffer_Move");
		}
		break;

	case SELECT_GAME::SELECT_STAGE:
		if (!Is_Stage_Selected && KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Update_Game_Select_Buffer(SELECT_GAME::STAGE_WAIT);
			Update_Select_Stage_State(SELECT_STAGE_STATE::STAGE_WAIT);

			Is_Stage_Selected = true;

			Sound_M->Play_SFX("Buffer_Select");
		}

		if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_CONTINUE);
			Sound_M->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_BACK);
			Sound_M->Play_SFX("Buffer_Move");
		}
		break;

	case SELECT_GAME::STAGE_WAIT:
		break;

	case SELECT_GAME::FADING_TO_STAGE:
		if (!Is_Fading_Started)
		{
			Fade_Start(0.5, true);
			Is_Fading_Started = true;
		}

		if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
		{
			Update_Main_Screen(Main_Screen::M_DONE);
			Update_Sub_Screen(Sub_Screen::S_DONE);
			Update_Game_Select_Screen(Game_Select_Screen::GAME_PLAYING);

			switch (static_cast<STAGE_NUM>(Stage_Select_Buffer_State))
			{
			case STAGE_1:
				Set_Now_Playing_Stage(NOW_PLAYING_STAGE::STAGE_ONE);
				break;

			case STAGE_2:
				Set_Now_Playing_Stage(NOW_PLAYING_STAGE::STAGE_TWO);
				break;

			case STAGE_3:
				Set_Now_Playing_Stage(NOW_PLAYING_STAGE::STAGE_THREE);
				break;

			case STAGE_4:
				Set_Now_Playing_Stage(NOW_PLAYING_STAGE::STAGE_FOUR);
				break;

			case STAGE_5:
				Set_Now_Playing_Stage(NOW_PLAYING_STAGE::STAGE_FIVE);
				break;
			}

			Update_Select_Stage_State(SELECT_STAGE_STATE::DONE);
			Update_Game_Select_Buffer(SELECT_GAME::DONE);

			Fade_Start(0.5, false);

			Is_Fading_Started = false;
			Is_Stage_Selected = false;
		}
		break;

	case SELECT_GAME::SELECT_BACK:
		if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Update_Game_Select_Buffer(SELECT_GAME::BACK_WAIT);
			Set_Game_Intro_State(GAME_INTRO_STATE::G_INTRO_WAIT);
			Sound_M->Play_SFX("Buffer_Back");
		}

		if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_STAGE);
			Sound_M->Play_SFX("Buffer_Move");
		}
		break;

	case SELECT_GAME::DONE:
		Is_Fading_Started = false;
		Is_Stage_Selected = false;
		break;
	}
}

void Game_Select_Draw()
{
	Game_Select_BG_Draw();
	Game_Select_Player_Draw();
	Game_Select_UI_Draw();
}

void Game_Select_BG_Draw()
{
	if (BG_Angle >= R_Two_Pie)
		BG_Angle = A_Zero;

	Sprite_Draw(BG_Select, S_BG_X, S_BG_Y, S_BG_Width, S_BG_Height, BG_Angle);
	BG_Angle += 0.0025f;
}

void Game_Select_Player_Draw()
{
	SpriteAni_Draw(Play_Main_TexID, Player_Intro_X, Player_Intro_Y, Player_Intro_Width, Player_Intro_Height);
}

void Game_Select_UI_Draw()
{
	if (Is_Stage_Selected)
	{
		Sprite_Draw(UI_STAGE, S_UI_X, UI_Stage_Y, UI_Stage_Width, S_UI_Height, A_Zero);

		if (!(Get_Stage_Select_Buffer_State() == STAGE_NUM::STAGE_1))
			Sprite_Draw(UI_BUTTON, Stage_Num_X, Stage_Up_Y, Stage_Num_Width, Stage_Num_Height, Stage_Num_Up_Angle);
		if (!(Get_Stage_Select_Buffer_State() == STAGE_NUM::STAGE_5))
			Sprite_Draw(UI_BUTTON, Stage_Num_X, Stage_Down_Y, Stage_Num_Width, Stage_Num_Height, Stage_Num_Down_Angle);

		int Highest_stage = Save_Data_Get_High_Stage();

#if defined(DEBUG) || defined(_DEBUG)
		Highest_stage = 5; // Debug Code
#endif

		float Alpha = (Get_Stage_Select_Buffer_State() <= Highest_stage) ? A_Origin : A_Half;

		Sprite_Draw(Now_Stage_TexID, Stage_Num_X, Stage_Num_Y, Stage_Num_Width, Stage_Num_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Alpha });
	}

	if (Get_Game_Select_Buffer() == SELECT_GAME::SELECT_NEW_GAME)
		Sprite_Draw(UI_NEW, S_UI_X, UI_New_Game_Y, UI_New_Game_Width, S_UI_Height, A_Zero);
	else
		Sprite_Draw(UI_NEW, S_UI_X, UI_New_Game_Y, UI_New_Game_Width, S_UI_Height, A_Zero, {A_Origin, A_Origin, A_Origin, A_Half});

	if (Get_Game_Select_Buffer() == SELECT_GAME::SELECT_CONTINUE)
		Sprite_Draw(UI_CONTINUE, S_UI_X, UI_Continue_Y, UI_Continue_Width, S_UI_Height, A_Zero);
	else
		Sprite_Draw(UI_CONTINUE, S_UI_X, UI_Continue_Y, UI_Continue_Width, S_UI_Height, A_Zero, {A_Origin, A_Origin, A_Origin, A_Half});

	if (!Is_Stage_Selected)
	{
		if (Get_Game_Select_Buffer() == SELECT_GAME::SELECT_STAGE)
		{
			Sprite_Draw(UI_STAGE, S_UI_X, UI_Stage_Y, UI_Stage_Width, S_UI_Height, A_Zero);
			Sprite_Draw(UI_SELECT, UI_Select_X, UI_Stage_Y, S_UI_Width, S_UI_Height, A_Zero);
		}
		else
		{
			Sprite_Draw(UI_STAGE, S_UI_X, UI_Stage_Y, UI_Stage_Width, S_UI_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
			Sprite_Draw(UI_SELECT, UI_Select_X, UI_Stage_Y, S_UI_Width, S_UI_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
		}
	}

	if (Get_Game_Select_Buffer() == SELECT_GAME::SELECT_BACK)
		Sprite_Draw(UI_BACK, S_UI_X, UI_Back_Y, UI_Back_Width, S_UI_Height, A_Zero);
	else
		Sprite_Draw(UI_BACK, S_UI_X, UI_Back_Y, UI_Back_Width, S_UI_Height, A_Zero, {A_Origin, A_Origin, A_Origin, A_Half});
}

void Update_Game_Select_Buffer(SELECT_GAME Buffer)
{
	Game_Select_Buffer = Buffer;
}

SELECT_GAME Get_Game_Select_Buffer()
{
	return Game_Select_Buffer;
}

void Update_Select_Stage_State(SELECT_STAGE_STATE State)
{
	Stage_Select_Buffer = State;
}

SELECT_STAGE_STATE Get_Select_Stage_State()
{
	return Stage_Select_Buffer;
}

void Update_Stage_Select_Buffer_State(STAGE_NUM Stage_State)
{
	Stage_Select_Buffer_State = Stage_State;
}

STAGE_NUM Get_Stage_Select_Buffer_State()
{
	return static_cast<STAGE_NUM>(Stage_Select_Buffer_State);
}

void Stage_TexID(int Stage_State)
{
	int Return_TexID = -1;

	switch (Stage_State)
	{
	case STAGE_1:
		Return_TexID = Stage_1;
		break;

	case STAGE_2:
		Return_TexID = Stage_2;
		break;		
					
	case STAGE_3:	
		Return_TexID = Stage_3;
		break;		
					
	case STAGE_4:	
		Return_TexID = Stage_4;
		break;		 
					 
	case STAGE_5:
		Return_TexID = Stage_5;
		break;
	}

	Now_Stage_TexID = Return_TexID;
}

void Game_Select_Texture()
{
	BG_Select = Texture_M->GetID("BG_Select");
	
	UI_NEW = Texture_M->GetID("UI_New_Game");
	UI_CONTINUE = Texture_M->GetID("UI_Continue");
	UI_STAGE = Texture_M->GetID("UI_Stage");
	UI_SELECT = Texture_M->GetID("UI_Select");
	UI_BACK = Texture_M->GetID("UI_Back");
	UI_BUTTON = Texture_M->GetID("UI_Num_Button_L");

	Main_TexID = Texture_M->GetID("Player_Main_BG");
	Main_Anime_ID = SpriteAni_Get_Pattern_Info(Main_TexID, 16, 4, 0.25, { 1000, 800 }, { 1000 * 0, 800 * 0 }, true);
	Play_Main_TexID = SpriteAni_CreatePlayer(Main_Anime_ID);

	Stage_1 = Texture_M->GetID("UI_Num_1");
	Stage_2 = Texture_M->GetID("UI_Num_2");
	Stage_3 = Texture_M->GetID("UI_Num_3");
	Stage_4 = Texture_M->GetID("UI_Num_4");
	Stage_5 = Texture_M->GetID("UI_Num_5");
}
