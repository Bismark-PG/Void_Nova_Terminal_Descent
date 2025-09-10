/*==============================================================================

	Effect Draw [Effect.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Effect.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "Enemy.h"
#include "Texture.h"
#include "Sprite_Animation.h"

using namespace PALETTE;

struct Effect
{
	XMFLOAT2 position;
	XMFLOAT2 size;
	int Sprite_Ani_ID;
	bool isEnable;
};

constexpr int EFFECT_MAX = ENEMY_MAX;
static Effect Effects[EFFECT_MAX]{};
// If You Have Many Effect, Make Matrix
static int Player_Explosion_TexID = -1;
static int Player_Explosion_Ani_ID = -1;

static int Enemy_Explosion_TexID = -1;
static int Enemy_Explosion_Ani_ID = -1;

static int Enemy_Real_Explosion_TexID = -1;
static int Enemy_Real_Explosion_Ani_ID = -1;

void Effect_Initialize()
{
	for (Effect& Eff : Effects)
		Eff.isEnable = false;

	Player_Explosion_TexID = Texture_Load(L"Resource/Texture/Animation/Explosion_Light_1.png");
	Player_Explosion_Ani_ID = SpriteAni_Get_Pattern_Info(Player_Explosion_TexID, 16, 4, 0.01, { 256, 256 }, { 0, 0 }, false);

	Enemy_Explosion_TexID = Texture_Load(L"Resource/Texture/Animation/Explosion_Light_Big_Fixed.png");
	Enemy_Explosion_Ani_ID = SpriteAni_Get_Pattern_Info(Enemy_Explosion_TexID, 42, 6, 0.0075, { 170, 170 }, { 0, 0 }, false);

	Enemy_Real_Explosion_TexID = Texture_Load(L"Resource/Texture/Animation/Explosion.png");
	Enemy_Real_Explosion_Ani_ID = SpriteAni_Get_Pattern_Info(Enemy_Real_Explosion_TexID, 16, 4, 0.05, { 64, 64 }, { 0, 0 }, false);
}

void Effect_Finalize()
{

}

void Effect_Update(double elapsed_time)
{
	for (Effect& Eff : Effects)
	{
		if (!Eff.isEnable)	continue;

		if (SpriteAni_IsStopped(Eff.Sprite_Ani_ID))
		{
			SpriteAni_DestroyPlayer(Eff.Sprite_Ani_ID);
			Eff.isEnable = false;
		}
	}
}

void Effect_Draw()
{
	for (Effect& Eff : Effects)
	{
		if (!Eff.isEnable)	continue;

		SpriteAni_Draw(Eff.Sprite_Ani_ID, Eff.position.x, Eff.position.y, Eff.size.x, Eff.size.y);
	}
}

void Effect_Create(Effect_Type type, const XMFLOAT2& position, const XMFLOAT2& size)
{
	for (Effect& Eff : Effects)
	{
		if (Eff.isEnable) continue;

		Eff.isEnable = true;

		switch (type)
		{
		case Effect_Type::PLAYER_EXPLOSION:
			Eff.size = size;
			Eff.position = position;
			Eff.Sprite_Ani_ID = SpriteAni_CreatePlayer(Player_Explosion_Ani_ID);
			break;

		case Effect_Type::ENEMY_EXPLOSION:
			Eff.size.x = size.x * A_One_And_Half;
			Eff.size.y = size.y * A_One_And_Half;

			Eff.position.x = position.x - (Eff.size.x * A_Quarter);
			Eff.position.y = position.y - (Eff.size.y * A_Quarter);
			Eff.Sprite_Ani_ID = SpriteAni_CreatePlayer(Enemy_Explosion_Ani_ID);
			break;

		case Effect_Type::REAL_EXPLOSION:
			Eff.size = size;
			Eff.position = position;
			Eff.Sprite_Ani_ID = SpriteAni_CreatePlayer(Enemy_Real_Explosion_Ani_ID);
			break;
		}
		break;
	}
}
