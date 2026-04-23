#pragma once
#include "Application/Object/BaseObject.h"

class PlayerBullet;

class Player : public BaseObject
{
public:

	Player() { Init(); }
	~Player() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

	void Shoot();

private:

	KdTexture m_tex;
	Math::Vector2 m_pos;
	float m_speed = 3.0f;

	static const int MAX_BULLETS = 100;
	PlayerBullet* m_bullets[MAX_BULLETS] = { nullptr };
	KdTexture m_bulletTex;

	float m_shootCooldown = 0.5f;
	float m_shootTimer = 0.0f;
};