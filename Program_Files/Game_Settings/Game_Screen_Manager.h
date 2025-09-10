/*==============================================================================

	Manage Game Screen [Game_Screen_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_SCREEN_MANAGER_H
#define GAME_SCREEN_MANAGER_H

#include "Game_State.h"

class Game_Manager {
public:
    void Change_Screen(Main_Screen M_Screen, Sub_Screen S_Screen, Game_Select_Screen G_Screen);
    Main_Screen Get_Current_Main_Screen() const;
    Sub_Screen Get_Current_Sub_Screen() const;
	Game_Select_Screen Get_Current_Game_Select_Screen() const;

private:
    Main_Screen Current_Main_Screen = Main_Screen::M_WAIT;
    Sub_Screen Current_Sub_Screen = Sub_Screen::S_WAIT;
    Game_Select_Screen Current_Game_Screen = Game_Select_Screen::G_WAIT;
};

#endif // GAME_SCREEN_MANAGER_H