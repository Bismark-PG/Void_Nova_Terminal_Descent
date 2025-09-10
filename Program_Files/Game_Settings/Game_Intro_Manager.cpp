/*==============================================================================

	Manage Game Intro [Game_Intro_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Game_Intro_Manager.h"
#include "Game_Select.h"

GAME_INTRO_STATE Game_Intro_State;

static bool IS_Intro = false;
static bool IS_Back = false;

void Game_Intro_Update()
{
	switch (Game_Intro_State)
	{
	case GAME_INTRO_STATE::NONE:
		break;

	case GAME_INTRO_STATE::G_INTRO_WAIT:
		if (Get_Game_Select_Buffer() == SELECT_GAME::INTRO_WAIT)
			Set_Game_Intro_State(GAME_INTRO_STATE::G_INTRO_PLAY);
		else if (Get_Game_Select_Buffer() == SELECT_GAME::BACK_WAIT)
			Set_Game_Intro_State(GAME_INTRO_STATE::G_INTRO_BACK);
		break;

	case GAME_INTRO_STATE::G_INTRO_PLAY:
		if (!IS_Intro)
		{
			Fade_Start(0.5, true);
			IS_Intro = true;
		}

		if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
		{
			Update_Main_Screen(Main_Screen::SELECT_GAME);
			Update_Game_Select_Buffer(SELECT_GAME::SELECT_WAIT);

			Fade_Start(0.5, false);

			Set_Game_Intro_State(GAME_INTRO_STATE::G_INTRO_DONE);
		}
		break;

	case GAME_INTRO_STATE::G_INTRO_BACK:
		if (!IS_Back)
		{
			Fade_Start(0.5, true);
			IS_Back = true;
		}

		if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
		{
			Update_Main_Buffer(UI_STATE::MENU_UI);
			Update_Main_Select_Buffer(MENU_BUFFER::START);

			Update_Main_Screen(Main_Screen::MENU_SELECT);
			Update_Game_Select_Buffer(SELECT_GAME::DONE);

			Update_Game_Select_Screen(Game_Select_Screen::G_DONE);

			Fade_Start(0.5, false);

			Set_Game_Intro_State(GAME_INTRO_STATE::G_INTRO_DONE);
		}
		break;

	case GAME_INTRO_STATE::G_INTRO_DONE:
		if (Fade_GetState() != FADE_STATE::FINISHED_IN)
		{
			IS_Intro = false;
			IS_Back = false;
			Set_Game_Intro_State(GAME_INTRO_STATE::NONE);
		}
		break;
	}
}

void Set_Game_Intro_State(GAME_INTRO_STATE state)
{
	Game_Intro_State = state;
}

GAME_INTRO_STATE Get_Game_Intro_State()
{
	return Game_Intro_State;
}
