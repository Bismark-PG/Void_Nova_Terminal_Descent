/*==============================================================================

	Make Enemy's Bullet [Enemy_Bullet.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Enemy_Bullet.h"
#include "sprite.h"
#include "texture.h"
#include "Enemy.h"

using namespace DirectX;
using namespace PALETTE;

static Enemy_Bullet Enemy_Bullets[ENEMY_BULLET_MAX]{};
static const std::vector<Circle> Empty_Collisions;

static int Enemy_Bullet_Normal_TexID = -1;
static int Enemy_Bullet_Long_TexID = -1;
static int Enemy_Bullet_Spire_TexID = -1;

void Enemy_Bullet_Initialize()
{
	for (Enemy_Bullet& bullet : Enemy_Bullets)
	{
		bullet.isEnable = false;
	}

	Enemy_Bullet_Normal_TexID = Texture_Load(L"Resource/Texture/Enemy/Bullet_Orange.png");
	Enemy_Bullet_Long_TexID = Texture_Load(L"Resource/Texture/Enemy/Bullet_Straight.png");
	Enemy_Bullet_Spire_TexID = Texture_Load(L"Resource/Texture/Enemy/Bullet_Spire.png");

	Enemy_Bullet_Normal_Width  = ENEMY_BULLET_NORMAL_WIDTH  * Game_Scale;
	Enemy_Bullet_Normal_Height = ENEMY_BULLET_NORMAL_HEIGHT * Game_Scale;
	Enemy_Bullet_Normal_Radius = (Enemy_Bullet_Normal_Width + Enemy_Bullet_Normal_Height) * A_Quarter;
	Enemy_Bullet_Normal_Speed = ENEMY_BULLET_NORMAL_SPEED   * Game_Scale;

	Enemy_Bullet_Long_Width  = ENEMY_BULLET_LONG_WIDTH  * Game_Scale;
	Enemy_Bullet_Long_Height = ENEMY_BULLET_LONG_HEIGHT * Game_Scale;
	Enemy_Bullet_Long_Speed  = ENEMY_BULLET_LONG_SPEED  * Game_Scale;

	Enemy_Bullet_Spire_Width = ENEMY_BULLET_SPIRE_WIDTH * Game_Scale;
	Enemy_Bullet_Spire_Height = ENEMY_BULLET_SPIRE_HEIGHT * Game_Scale;
	Enemy_Bullet_Spire_Speed = ENEMY_BULLET_SPIRE_SPEED * Game_Scale;
}

void Enemy_Bullet_Finalize()
{
	Enemy_Bullet_Normal_Width  = 0;
	Enemy_Bullet_Normal_Height = 0;
	Enemy_Bullet_Normal_Radius = 0;

	Enemy_Bullet_Long_Width  = 0;
	Enemy_Bullet_Long_Height = 0;
}

void Enemy_Bullet_Update(double elapsed_time)
{
	for (Enemy_Bullet& bullet : Enemy_Bullets)
	{
		if (!bullet.isEnable) continue;


		XMVECTOR POS = XMLoadFloat2(&bullet.position);
		XMVECTOR VEL = XMLoadFloat2(&bullet.velocity);
		POS += VEL * static_cast<float>(elapsed_time);
		XMStoreFloat2(&bullet.position, POS);

		if (!bullet.Collisions.empty())
		{
			float B_Center_X = bullet.position.x + bullet.size.x * A_Half;
			float B_Center_Y = bullet.position.y + bullet.size.y * A_Half;

			float Sin = sinf(bullet.Angle);
			float Cos = cosf(bullet.Angle);
			if (bullet.Collisions.size() > 1)
			{
				float Texture_Height = bullet.size.y;

				for (int j = 0; j < bullet.Collisions.size(); ++j)
				{
					float Next_offset;

					// Logic for Long Spire
					if (bullet.Collisions.size() == SPIRE_BULLET_COLLISION_COUNT)
					{
						if (j == 0)
							Next_offset = -Texture_Height * A_Quarter;
						else if (j == 1)
							Next_offset = A_Zero;
						else
							Next_offset = Texture_Height * A_Quarter;
					}
					else // Logic for Long bullet
						Next_offset = -Texture_Height * A_Half + (Texture_Height / (LONG_BULLET_COLLISION_COUNT + 1)) * (j + 1);

					float Rotation_X = -Next_offset * Sin;
					float Rotation_Y = Next_offset * Cos;

					bullet.Collisions[j].Center.x = B_Center_X + Rotation_X;
					bullet.Collisions[j].Center.y = B_Center_Y + Rotation_Y;
				}
			}
			else
			{
				bullet.Collisions[0].Center.x = B_Center_X;
				bullet.Collisions[0].Center.y = B_Center_Y;
			}
		}
	}
}

void Enemy_Bullet_Normal_Draw()
{
	for (Enemy_Bullet& bullet : Enemy_Bullets)
	{
		if (!bullet.isEnable || bullet.Type != Enemy_Bullet_Type::NORMAL)
			continue;

		Sprite_Draw(Enemy_Bullet_Normal_TexID, bullet.position.x, bullet.position.y,
			bullet.size.x, bullet.size.y, 0.f, { A_Origin, A_Origin, A_Origin, A_F_Fifths });
	}
}

void Enemy_Bullet_Long_Draw()
{
	for (Enemy_Bullet& bullet : Enemy_Bullets)
	{
		if (!bullet.isEnable || bullet.Type != Enemy_Bullet_Type::LONG)
			continue;

		Sprite_Draw(Enemy_Bullet_Long_TexID, bullet.position.x, bullet.position.y,
			bullet.size.x, bullet.size.y, bullet.Angle, { A_Origin, A_Origin, A_Origin, A_F_Fifths });
	}
}

void Enemy_Bullet_Spire_Draw()
{
	for (Enemy_Bullet& bullet : Enemy_Bullets)
	{
		if (!bullet.isEnable || bullet.Type != Enemy_Bullet_Type::SPIRE)
			continue;

		Sprite_Draw(Enemy_Bullet_Spire_TexID, bullet.position.x, bullet.position.y,
			bullet.size.x, bullet.size.y, bullet.Angle, { A_Origin, A_Origin, A_Origin, A_F_Fifths });
	}
}

void Enemy_Bullet_Normal_creat(const XMFLOAT2& position, const XMFLOAT2& velocity)
{
	for (int i = 0; i < ENEMY_BULLET_MAX; i++)
	{
		if (Enemy_Bullets[i].isEnable) continue;

		Enemy_Bullet& bullet = Enemy_Bullets[i];
		bullet.isEnable = true;
		bullet.Type = Enemy_Bullet_Type::NORMAL;
		bullet.position = position;
		bullet.size = { Enemy_Bullet_Normal_Width, Enemy_Bullet_Normal_Height };
		bullet.velocity = velocity;
		bullet.Angle = 0.0f;

		bullet.Collisions.clear();
		bullet.Collisions.reserve(1);

		Circle Collisions = {};
		Collisions.Center = { position.x + Enemy_Bullet_Normal_Width * A_Half, position.y + Enemy_Bullet_Normal_Height * A_Half };
		Collisions.Radius = Enemy_Bullet_Normal_Radius;
		bullet.Collisions.push_back(Collisions);

		break;
	}
}

void Enemy_Bullet_Long_creat(const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& velocity, float Angle)
{
	for (int i = 0; i < ENEMY_BULLET_MAX; i++)
	{
		if (Enemy_Bullets[i].isEnable) continue;

		Enemy_Bullet& bullet = Enemy_Bullets[i];

		bullet.isEnable = true;
		bullet.position = position;
		bullet.Type = Enemy_Bullet_Type::LONG;
		bullet.size = { Enemy_Bullet_Long_Width, Enemy_Bullet_Long_Height };
		bullet.velocity = velocity;
		bullet.Angle = Angle;

		bullet.Collisions.clear();
		bullet.Collisions.reserve(LONG_BULLET_COLLISION_COUNT);
		float radius = bullet.size.x * A_Half;

		for (int j = 0; j < LONG_BULLET_COLLISION_COUNT; j++)
		{
			Circle Collisions = {};
			Collisions.Radius = radius;
			bullet.Collisions.push_back(Collisions);
		}

		break;
	}
}

void Enemy_Bullet_Spire_creat(const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& velocity, float Angle)
{
	for (int i = 0; i < ENEMY_BULLET_MAX; i++)
	{
		if (Enemy_Bullets[i].isEnable) continue;

		Enemy_Bullet& bullet = Enemy_Bullets[i];

		bullet.isEnable = true;
		bullet.position = position;
		bullet.Type = Enemy_Bullet_Type::SPIRE;
		bullet.size = { Enemy_Bullet_Spire_Width, Enemy_Bullet_Spire_Height };
		bullet.velocity = velocity;
		bullet.Angle = Angle;

		bullet.Collisions.clear();
		bullet.Collisions.reserve(SPIRE_BULLET_COLLISION_COUNT);

		Circle C1 = {};
		C1.Radius = bullet.size.x * A_Half;
		bullet.Collisions.push_back(C1);

		Circle C2 = {};
		C2.Radius = bullet.size.x * A_Quarter;
		bullet.Collisions.push_back(C2);

		Circle C3 = {};
		C3.Radius = bullet.size.x * A_Quarter * A_Half;
		bullet.Collisions.push_back(C3);

		break;
	}
}


void Enemy_Bullet_Destroy(int Index)
{
	if (Index >= 0 && Index < ENEMY_BULLET_MAX)
		Enemy_Bullets[Index].isEnable = false;
}


void Enemy_Bullet_Destroy_All()
{
	for (Enemy_Bullet& bullet : Enemy_Bullets)
		bullet.isEnable = false;
}

bool Enemy_Bullet_IsEnable(int Index)
{
	return Enemy_Bullets[Index].isEnable;
}

bool Enemy_Bullet_CheckCollision(int Index, const Circle& target_collision)
{
	if (!Enemy_Bullets[Index].isEnable || Enemy_Bullets[Index].Collisions.empty())
		return false;

	for (const auto& B_Circle : Enemy_Bullets[Index].Collisions)
	{
		if (Collision_IsOverlapCircle(B_Circle, target_collision))
			return true;
	}
	return false;
}

const std::vector<Circle>& Enemy_Bullet_GetCollisions(int Index)
{
	if (Index >= 0 && Index < ENEMY_BULLET_MAX && Enemy_Bullets[Index].isEnable)
		return Enemy_Bullets[Index].Collisions;

	return Empty_Collisions;
}

float Enemy_Get_Bullet_Speed(Enemy_Bullet_Type Type)
{
	float Speed = 0.0f;

	switch (Type)
	{
	case Enemy_Bullet_Type::NORMAL:
		Speed = Enemy_Bullet_Normal_Speed;
		break;

	case Enemy_Bullet_Type::LONG:
		Speed = Enemy_Bullet_Long_Speed;
		break;

	case Enemy_Bullet_Type::SPIRE:
		Speed = Enemy_Bullet_Spire_Speed;
		break;
	}

	return Speed;
}
