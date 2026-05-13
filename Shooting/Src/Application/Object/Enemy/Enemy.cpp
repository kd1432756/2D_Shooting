#include "Enemy.h"
#include "EnemyBullet.h"

void Enemy::Init()
{
	m_enemyBulletTex.Load("Texture/GameScene/enemy_bullet.png");
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}

void Enemy::Release()
{
	m_enemyBulletTex.Release();
}

void Enemy::AnimUpdate()
{
	m_animTimer += 1.0f / 60.0f;

	if (m_animTimer >= m_animSpeed)
	{
		m_animTimer = 0.0f;
		m_animIndex++; // 次のコマへ
	}

	if (m_animIndex >= m_maxFrames)
	{
		if (!m_isAlive)
		{
			m_animIndex = m_maxFrames - 1;
			m_isActive = false;
			return;
		}

		m_animIndex = 0;
	}

	if (!m_isAlive) m_alpha -= 0.01f;
}

void Enemy::ShotBullet(Math::Vector2& pos, float angleRad, float speed, float size)
{
	if (!m_pEnemyBullets) return; // 登録されていなければ何もしない

	for (int i = 0; i < m_enemyBulletMax; ++i)
	{
		auto* bullet = m_pEnemyBullets[i];
		if (bullet && !bullet->IsActive())
		{
			bullet->SetActive(true);
			bullet->SetPosition(pos);
			bullet->SetAngle(angleRad);
			bullet->SetSpeed(speed);
			bullet->SetSize(size);
			bullet->SetTexture(&m_enemyBulletTex);
			break;
		}
	}
}

void Enemy::ShotBullet(Math::Vector2& pos, float angleRad, float speed, float size, int wait)
{
	if (!m_pEnemyBullets) return; // 登録されていなければ何もしない

	for (int i = 0; i < m_enemyBulletMax; ++i)
	{
		auto* bullet = m_pEnemyBullets[i];
		if (bullet && !bullet->IsActive())
		{
			bullet->SetActive(true);
			bullet->SetPosition(pos);
			bullet->SetAngle(angleRad);
			bullet->SetSpeed(speed);
			bullet->SetSize(size);
			bullet->SetTexture(&m_enemyBulletTex);
			bullet->SetWait(wait);
			break;
		}
	}
}

