/*==============================================================================

	Game Logic [Score.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef SCORE_H
#define SCORE_H

static unsigned int Score = 0;
static unsigned int View_Score = 0;
static unsigned int Null_Score = 0000000000;

static int Now_Stage = -1;
static unsigned int Now_Stage_Score = 0;

static unsigned int Stage_Now_Scores[5]  = { 0 };
static unsigned int Stage_High_Scores[5] = { 0 };

void Score_Initialize();
void Score_Finalize();

void Score_Update(double elapsed_time);
void Score_Increase(int Increase);

void Score_Draw();
void Score_End_Score_Draw(float Draw_X, float Draw_Y, float Draw_W, float Draw_H, float Alpha);
void Score_Outro_Update(double elapsed_time);
bool Score_Is_Increase_Done();

void Score_Reset();
void Score_Reset_All_Score();
void Score_Outro_Reset();
void Score_Reset_Current_Stage_Score();

void Score_Set_Score();
unsigned int Score_Get_Total_Score();
unsigned int Score_Get_High_Score();

#endif // SCORE_H
