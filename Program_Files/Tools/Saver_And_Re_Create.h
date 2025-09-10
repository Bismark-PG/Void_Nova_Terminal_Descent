/*==============================================================================

	Save or Restore Game State And Buffer [Saver_And_Re_Create.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef STATE_BUFFER_SAVER
#define STATE_BUFFER_SAVER
#include "Game_Header_Manager.h"

struct State_And_Buffer_Storage
{
	// Game Manager
	Main_Screen				Main_State = {};
	Sub_Screen				Sub_State = {};
	Game_Select_Screen		Game_State = {};

	// Title
	UI_STATE				Main_Draw_State = {};
	MENU_BUFFER				Main_Select_Buffer = {};

	// Settings & Sound Scale
	SETTING_BUFFER			Setting_Select_Buffer = {};
	SOUND_SETTING_STATE		Sound_Setting_Buffer = {};
	SOUND_SCALE_BUFFER		BGM_Scale_Buffer = {};
	SOUND_SCALE_BUFFER		SFX_Scale_Buffer = {};

	// Save BGM Info
	std::string				Last_Playing_BGM;
};

extern State_And_Buffer_Storage Saver;

void Save_State_And_Buffer();
void Restore_State_And_Buffer();

#endif // STATE_BUFFER_SAVER