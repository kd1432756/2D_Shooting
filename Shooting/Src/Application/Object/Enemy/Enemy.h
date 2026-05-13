#pragma once
#include "Application/Object/BaseObject.h"
#include "Application/Scene.h"

class EnemyBullet;

class Enemy : public BaseObject
{
public:
	Enemy() { Init(); }
	~Enemy() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

	void ChangeHP(int amount)
	{
		m_hp += amount;
		if (m_hp <= 0)
		{
			m_isAlive = false;
			m_animTimer = 0.0f;
			m_animIndex = 0;
			SCENE.AddScore(10);
		}
	}

	void SetEnemyBulletPool(EnemyBullet** bullets, int maxBullets)
	{
		m_pEnemyBullets = bullets;
		m_enemyBulletMax = maxBullets;
	}

protected:

	void AnimUpdate();

	void ShotBullet(Math::Vector2& pos, float angleRad, float speed, float size);
	void ShotBullet(Math::Vector2& pos, float angleRad, float speed, float size, int wait);

	float m_speed = 2.0f;

	int m_animIndex = 0;
	float m_animTimer = 0.0f;
	float m_animSpeed = 0.1f;
	int m_maxFrames = 2;
	float m_alpha = 1.0f;

	EnemyBullet** m_pEnemyBullets = nullptr;
	int m_enemyBulletMax = 0;
	KdTexture m_enemyBulletTex;
};