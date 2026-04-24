#pragma once
#include "Application/Object/BaseObject.h"

class PlayerBullet;

class Player : public BaseObject
{
public:

	enum class State
	{
		Normal,
		ReadyToShoot,
		Firing,
		Cooldown
	};

	Player() { Init(); }
	~Player() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

	State GetState() const { return m_state; }

private:

	void Shoot();

	void ShootSpecial();

	bool IsFireingFinished();

	void ChangeState(State newState);

	bool m_isActive;
	Math::Vector2 m_pos;
	KdTexture m_tex;

	float m_speed = 3.0f;
	State m_state = State::Normal;
	float m_cooldownTimer = 0.0f;

	static const int MAX_BULLETS = 100;
	PlayerBullet* m_bullets[MAX_BULLETS] = { nullptr };

	static const int MAX_SPECIALBULLETS = 2;
	PlayerBullet* m_specialBullets[MAX_SPECIALBULLETS] = { nullptr };

	KdTexture m_bulletTex;

	float m_shootCooldown = 0.5f;
	float m_shootTimer = 0.0f;
};