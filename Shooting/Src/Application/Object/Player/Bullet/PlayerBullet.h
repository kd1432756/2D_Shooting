#pragma once
#include "../../BaseObject.h"

class PlayerBullet : public BaseObject
{
public:

	enum class BulletType
	{
		Pircing,
		Split,
		Parabola,
	};

	enum class SplitSide {
		Normal,
		Left,
		Right,
		None
	};

	PlayerBullet() { Init(); }
	~PlayerBullet() { Release(); }

	void Init();
	void Update(Math::Vector2& playerPos);
	void Draw();
	void Release();

	void SetBulletType(BulletType type) { m_bulletType = type; }

	void IncrementKillCount() { m_killCount++; }

	SplitSide GetMissionSuccess();

private:

	float m_speed = 30.0f;
	Math::Vector2 m_rectSize = { 16, 5 };
	float m_size = 2.0f;

	BulletType m_bulletType = BulletType::Pircing;

	int m_killCount = 0;

	const float TARGET_DIST = 700.0f;
	float m_distance = 0;
	const float MAX_HEIGHT = 350.0f;
	Math::Vector2 prevPos;
};