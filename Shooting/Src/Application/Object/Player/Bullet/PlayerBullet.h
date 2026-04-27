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
	void Update();
	void Draw();
	void Release();

	void SetBulletType(BulletType type) { m_bulletType = type; }

	void IncrementKillCount() { m_killCount++; }

	SplitSide GetMissionSuccess();

private:

	float m_speed = 15.0f;
	Math::Vector2 m_rectSize = { 16, 16 };

	BulletType m_bulletType = BulletType::Pircing;

	int m_killCount = 0;
};