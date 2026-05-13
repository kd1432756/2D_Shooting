#pragma once
#include"Application/Object/Enemy/Enemy.h"

class FireWorm : public Enemy
{
public:
	FireWorm() { Init(); }
	~FireWorm() { Release(); }

	void Init();
	void Update(Math::Vector2& playerPos);
	void Draw();
	void Release();

private:

	enum class AnimType
	{
		Walk,
		Idle,
		Attack,
		Death,
		Shout
	};

	enum class AttackType
	{
		Attack1,
		Attack2,
		Attack3,
	};

	AnimType m_animType;
	AttackType m_attackType;

	int m_moveCount = 0;
	int m_attackCount = 0;
};