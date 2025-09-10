/*==============================================================================

	Game Title [Title.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef TITLE_H
#define TITLE_H

#include "Game_Window.h"
#include "system_timer.h"
#include "Fade.h"

static int TITLE_WIDTH = 16;
static int TITLE_HEIGHT = 9;
static constexpr int TITLE_SCALE = 40;

static int LOGO_WIDTH = 6;
static int LOGO_HEIGHT = 6;
static constexpr int LOGO_SCALE = 30;

static int NAME_WIDTH = 5;
static int NAME_HEIGHT = 1;
static constexpr int NAME_SCALE = 50;

static int PIXEl_WIDTH = 8;
static int PIXEl_HEIGHT = 8;

static float BG_X;
extern float BG_WIDTH;
extern float BG_HEIGHT;

static float Title_X;
static float Title_Y;
static float Title_Width;
static float Title_Height;

static float Logo_X;
static float Logo_Y;
static float Logo_Width;
static float Logo_Height;

static float Name_X;
static float Name_Y;
static float Name_Width;
static float Name_Height;

static float Pixel_X;
static float Pixel_X_Goal;
static float Pixel_Y;
static float Pixel_Width;
static float Pixel_Height;
static float Pixel_Y_Origin;
static float Pixel_Height_Origin;
static float Pixel_Move;
static constexpr float Pixel_Location_Limit = 0.5f;

static constexpr float Title_BG_Alpha_Increase = 0.025f;
static constexpr float Title_Text_Alpha_Increase = 0.01f;

void Title_Initialize();
void Title_Finalize();

void Title_Update();
void Logo_Draw();

void Title_BG_Blur();
void Title_Logo_Draw();

void Title_Draw();

void Menu_BG();
void Menu_Select_Draw();

#endif // TITLE_H