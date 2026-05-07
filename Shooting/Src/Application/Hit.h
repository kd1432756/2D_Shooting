#pragma once

class Hit
{
public:

	bool CheckCircle(const Math::Vector2& pos1, float r1, const Math::Vector2& pos2, float r2);

	bool CheckRect(const Math::Vector2& pos1, float w1, float h1,
		const Math::Vector2& pos2, float w2, float h2);

private:

};