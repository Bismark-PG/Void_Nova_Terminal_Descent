/*==============================================================================

	Save/Load Game Progress [Save_Data.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Save_Data.h"
#include <fstream>

static Game_Progress Current_Progress;


static const char* SAVE_FILE_NAME = "Resource/Save/Void_Nova_Save.dat";

void Save_Data_Initialize()
{
	Save_Data_Load();
}

void Save_Data_Save()
{
	std::ofstream saveFile(SAVE_FILE_NAME, std::ios::binary);

	if (saveFile.is_open())
	{
		saveFile.write(reinterpret_cast<const char*>(&Current_Progress), sizeof(Game_Progress));
		saveFile.close();
	}
}

void Save_Data_Load()
{
	std::ifstream loadFile(SAVE_FILE_NAME, std::ios::binary);

	if (loadFile.is_open())
	{
		loadFile.read(reinterpret_cast<char*>(&Current_Progress), sizeof(Game_Progress));
		loadFile.close();
	}
	else
	{
		Current_Progress.High_Stage = 1;
		Current_Progress.Hi_Score_Stage_1 = 0;
		Current_Progress.Hi_Score_Stage_2 = 0;
		Current_Progress.Hi_Score_Stage_3 = 0;
		Current_Progress.Hi_Score_Stage_4 = 0;
		Current_Progress.Hi_Score_Stage_5 = 0;
	}
}

void Save_Data_Update_Stage(int cleared_stage_number)
{
	int next_stage = cleared_stage_number + 1;

	if (next_stage > Current_Progress.High_Stage)
	{
		Current_Progress.High_Stage = next_stage;
		Save_Data_Save();
	}
}

int Save_Data_Get_High_Stage()
{
	return Current_Progress.High_Stage;
}

void Save_Data_Update_High_Score(int stage_index, unsigned int score)
{
	bool Is_Updated = false;
	switch (stage_index)
	{
	case 0: 
		if (score > Current_Progress.Hi_Score_Stage_1)
		{
			Current_Progress.Hi_Score_Stage_1 = score;
			Is_Updated = true;
		}
		break;

	case 1:
		if (score > Current_Progress.Hi_Score_Stage_2)
		{
			Current_Progress.Hi_Score_Stage_2 = score;
			Is_Updated = true;
		}
		break;

	case 2:
		if (score > Current_Progress.Hi_Score_Stage_3)
		{
			Current_Progress.Hi_Score_Stage_3 = score;
			Is_Updated = true;
		}
		break;

	case 3:
		if (score > Current_Progress.Hi_Score_Stage_4)
		{
			Current_Progress.Hi_Score_Stage_4 = score;
			Is_Updated = true;
		}
		break;

	case 4:
		if (score > Current_Progress.Hi_Score_Stage_5)
		{
			Current_Progress.Hi_Score_Stage_5 = score;
			Is_Updated = true;
		}
		break;
	}

	if (Is_Updated)
		Save_Data_Save();
}

unsigned int Save_Data_Get_High_Score(int stage_index)
{
	switch (stage_index)
	{
	case 0:
		return Current_Progress.Hi_Score_Stage_1;

	case 1:
		return Current_Progress.Hi_Score_Stage_2;

	case 2:
		return Current_Progress.Hi_Score_Stage_3;

	case 3:
		return Current_Progress.Hi_Score_Stage_4;

	case 4:
		return Current_Progress.Hi_Score_Stage_5;

	default:
		return 0;
	}
}
