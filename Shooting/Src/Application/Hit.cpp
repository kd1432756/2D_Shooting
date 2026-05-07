#include "Hit.h"

bool Hit::CheckCircle(const Math::Vector2& pos1, float r1, const Math::Vector2& pos2, float r2)
{
    float radiusSum = r1 + r2;

    float distSq = Math::Vector2::DistanceSquared(pos1, pos2);

    return distSq <= (radiusSum * radiusSum);
}

bool Hit::CheckRect(const Math::Vector2& pos1, float w1, float h1, const Math::Vector2& pos2, float w2, float h2)
{
    if (abs(pos1.x - pos2.x) > (w1 + w2) / 2.0f) return false;

    if (abs(pos1.y - pos2.y) > (h1 + h2) / 2.0f) return false;

    return true;
}
