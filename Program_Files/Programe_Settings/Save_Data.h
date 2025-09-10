/*==============================================================================

	Save/Load Game Progress [Save_Data.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef SAVE_DATA_H
#define SAVE_DATA_H

struct Game_Progress
{
	int High_Stage;

	unsigned int Hi_Score_Stage_1;
	unsigned int Hi_Score_Stage_2;
	unsigned int Hi_Score_Stage_3;
	unsigned int Hi_Score_Stage_4;
	unsigned int Hi_Score_Stage_5;
};

void Save_Data_Initialize();

void Save_Data_Save();
void Save_Data_Load();

void Save_Data_Update_Stage(int cleared_stage_number);
int Save_Data_Get_High_Stage();

void Save_Data_Update_High_Score(int stage_index, unsigned int score);
unsigned int Save_Data_Get_High_Score(int stage_index);


#endif // SAVE_DATA_H