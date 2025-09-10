/*==============================================================================

	Make Bullet [Bullet.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Bullet.h"
#include "Palette.h"
#include "Enemy_Bullet.h"
#include "Game_Status.h"

using namespace DirectX;
using namespace PALETTE;

static Bullet Bullets[BULLET_MAX]{};

static int Bullet_TexID{};
static int Laser_TexID{};

void Bullet_Initialize()
{
	for (Bullet& Bullet : Bullets)
	{
		Bullet.isEnable = false;
	}
	Bullet_TexID = Texture_Load(L"Resource/Texture/Player/Bullet_Power_Standard.png");
	Laser_TexID  = Texture_Load(L"Resource/Texture/Player/Bullet_Power_Max.png");
	
	Bullet_Width  = BULLET_WIDTH  * Game_Scale;
	Bullet_Height = BULLET_HEIGHT * Game_Scale;
	Bullet_Radius = (Bullet_Width + Bullet_Height) * A_Quarter;
	Bullet_Speed  = LASER_SPEED * Game_Scale;

	Laser_Width  = LASER_WIDTH * Game_Scale;
	Laser_Height = LASER_HEIGHT * Game_Scale;
	Laser_Radius = (Laser_Width + Laser_Height) * A_Quarter;
	Laser_Speed  = LASER_SPEED * Game_Scale;

	Bullet_Damage_For_Power = BULLET_DAMAGE_Lv_1;
}

void Bullet_Finalize()
{
	Bullet_Width  = 0;
	Bullet_Height = 0;
	Bullet_Radius = 0;
	Bullet_Speed  = 0;

	Laser_Width  = 0;
	Laser_Height = 0;
	Laser_Radius = 0;
	Laser_Speed  = 0;

	Bullet_Damage_For_Power = 0;
}

void Bullet_Update(double elapsed_time)
{
	for (Bullet& Bullet : Bullets)
	{
		if (!Bullet.isEnable)	continue;

		float Current_Power = Status_Get_Power();
		int Power_Level = static_cast<int>(Current_Power);

		switch (Power_Level)
		{
		case 0:
			Bullet_Damage_For_Power = BULLET_DAMAGE_Lv_1;
			break;

		case 1:
			Bullet_Damage_For_Power = BULLET_DAMAGE_Lv_2;
			break;

		case 2:
			Bullet_Damage_For_Power = BULLET_DAMAGE_Lv_3;
			break;

		case 3:
			Bullet_Damage_For_Power = BULLET_DAMAGE_LV_4;
			break;

		case 4:
			Bullet_Damage_For_Power = BULLET_DAMAGE_MAX;
			break;
		}

		if (Bullet.position.y < -Bullet.size.y || 
			Bullet.position.y > SCREEN_HEIGHT ||
			Bullet.position.x < -Bullet.size.x ||
			Bullet.position.x > SCREEN_WIDTH)
		{
			Bullet.isEnable = false;
			continue;
		}

		// ââéZópÇÃïœêîÇ…äiî[
		XMVECTOR pos = XMLoadFloat2(&Bullet.position);
		XMVECTOR vel = XMLoadFloat2(&Bullet.velocity);

		// É|ÉWÉVÉáÉìÇÇ∏ÇÁÇ∑
		pos += vel * static_cast<float>(elapsed_time);

		// å≥ÇÃïœêîÇ…ï‘Ç∑
		XMStoreFloat2(&Bullet.position, pos);
	}
}

void Bullet_Draw()
{
	for (Bullet& Bullet : Bullets)
	{
		if (!Bullet.isEnable)	continue;

		Sprite_Draw(Bullet_TexID, Bullet.position.x, Bullet.position.y, Bullet.size.x, Bullet.size.y,
			0.f, { A_Origin, A_Origin, A_Origin, A_Third });
	}
}

void Laser_Draw()
{
	for (Bullet& Bullet : Bullets)
	{
		if (!Bullet.isEnable)	continue;

		Sprite_Draw(Laser_TexID, Bullet.position.x, Bullet.position.y, Bullet.size.x, Bullet.size.y,
			0.f, { A_Origin, A_Origin, A_Origin, A_Third });
	}
}

void Bullet_Create(const XMFLOAT2& position)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (Bullets[i].isEnable) continue;

		Bullets[i].isEnable = true;
		Bullets[i].lifeTime = 0.0;
		Bullets[i].position = position;
		Bullets[i].size = { Bullet_Width, Bullet_Height };
		Bullets[i].velocity = { 0.0, -Bullet_Speed };
		Bullets[i].Collision = { { Bullet_Width * A_Half, Bullet_Height * A_Half }, Bullet_Radius };
		break;
	}
}

void Laser_Create(const DirectX::XMFLOAT2& position)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (Bullets[i].isEnable) continue;

		Bullets[i].isEnable = true;
		Bullets[i].lifeTime = 0.0;
		Bullets[i].position = position;
		Bullets[i].size = { Laser_Width, Laser_Height };
		Bullets[i].velocity = { 0.0, -Laser_Speed };
		Bullets[i].Collision = { { Laser_Width * A_Half, Laser_Height * A_Half }, Laser_Radius };
		break;
	}
}

bool Bullet_IsEnable(int Index)
{
	return Bullets[Index].isEnable;
}

Circle Bullet_GetCollision(int Index)
{
	float Cx = Bullets[Index].Collision.Center.x
		+ Bullets[Index].position.x;
	float Cy = Bullets[Index].Collision.Center.y
		+ Bullets[Index].position.y;

	return { { Cx, Cy }, Bullets[Index].Collision.Radius};
}

void Bullet_Destroy(int Index)
{
	Bullets[Index].isEnable = false;
}

void Bullet_Destroy_All_Enemy_Bullets()
{
	Enemy_Bullet_Destroy_All();
}

float Bullet_Get_Now_Damage()
{
	return Bullet_Damage_For_Power;
}
