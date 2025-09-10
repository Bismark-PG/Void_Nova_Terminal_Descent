/*==============================================================================

    AABB Collision [Collision.h]

    Author : Choi HyungJoon

==============================================================================*/
#include "Collision.h"
#include <cmath>
using namespace DirectX;

bool Collision_IsOverlapCircle(const Circle& a, const Circle& b)
{ 
    float x1 = b.Center.x - a.Center.x;
    float y1 = b.Center.y - a.Center.y;

    return pow((a.Radius + b.Radius), 2) > (pow(x1, 2) + pow(y1, 2));

    // DirectX
    /*
    XMVECTOR ac = XMLoadFloat2(&a.Center);
    XMVECTOR bc = XMLoadFloat2(&b.Center);
    XMVECTOR lsq = XMVector2LengthSq(bc - ac);

    return pow(a.Radius + b.Radius, 2) > XMVectorGetX(lsq);
    */
}

bool Collision_IsOverlapBox(const Box& a, const Box& b)
{
    float At = a.Center.y - a.Half_Height;
    float Ab = a.Center.y + a.Half_Height;
    float Al = a.Center.x - a.Half_Width;
    float Ar = a.Center.x + a.Half_Width;
    float Bt = b.Center.y - b.Half_Height;
    float Bb = b.Center.y + b.Half_Height;
    float Bl = b.Center.x - b.Half_Width;
    float Br = b.Center.x + b.Half_Width;

    return Al < Br
        && Ar > Bl
        && At < Bb
        && Ab > Bt;
}
