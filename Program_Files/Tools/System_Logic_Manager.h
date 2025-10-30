/*==============================================================================

	Manage System Logic [System_Logic_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef SYSTEM_LOGIC_MANAGER
#define SYSTEM_LOGIC_MANAGER
#include "Game_Header_Manager.h"

void System_Initialize(HWND hWnd);
void System_Finalize();

void System_Reset_For_Ending();

#endif // SYSTEM_LOGIC_MANAGER