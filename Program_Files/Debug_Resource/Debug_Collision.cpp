/*==============================================================================

	Debug Draw Collision [Debug_Collision.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Debug_Collision.h"
#include "Texture.h"
#include "Sprite.h"

using namespace DirectX;

static int Circle_TexID = -1;

void Debug_Collision_Initialize()
{
	Circle_TexID = Texture_Load(L"Resource/Texture/Other/Debug.png");
}

void Debug_Collision_Finalize()
{
}

void Debug_Collision_Draw(const Circle& circle, const XMFLOAT4& color)
{
	if (Circle_TexID < 0) return;

	if (circle.Radius <= 0.0f) return;

	float Diameter = circle.Radius * 2.0f;

	float Draw_x = circle.Center.x - circle.Radius;
	float Draw_y = circle.Center.y - circle.Radius;

	Sprite_Draw(Circle_TexID, Draw_x, Draw_y, Diameter, Diameter, 0.0f, color);
}
