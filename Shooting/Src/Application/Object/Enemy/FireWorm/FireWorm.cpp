#include "FireWorm.h"
#include "Application/Scene.h"

void FireWorm::Init()
{
	m_isActive = false;
	m_isAlive = false;

	m_speed = 1.0f;

	m_animTimer = 0.0f;
	m_animIndex = 0;
	m_animSpeed = 0.1f;
	m_maxFrames = 9;
	m_alpha = 1.0f;

	m_moveCount = 0;

	m_animType = AnimType::Walk;
}

void FireWorm::Update(Math::Vector2& playerPos)
{
	if (m_isAlive)
	{
		switch (m_animType)
		{
		case AnimType::Walk:
			if (m_pos.x >= 500)
			{
				m_pos.x -= m_speed;
			}
			else
			{
				/*m_animType = AnimType::Idle;
				m_maxFrames = 9;
				m_moveCount++;*/
				m_animType = AnimType::Shout;
				m_maxFrames = 16;
				m_animSpeed = 0.15f;
			}

			break;
		case AnimType::Shout:
			if (m_animIndex == 0 && m_animTimer <= 0.0f)
			{
				m_animType = AnimType::Idle;
				m_maxFrames = 9;
				m_animSpeed = 0.1f;
			}
			break;
		case AnimType::Idle:
			if (m_animIndex == 0 && m_animTimer <= 0.0f)
			{
				m_moveCount++;

				if (m_moveCount >= 4)
				{
					m_moveCount = 0;
					m_animType = AnimType::Attack;
					switch (rand() % 3)
					{
					case 0:
						m_attackType = AttackType::Attack1;
						m_maxFrames = 9;
						break;
					case 1:
						m_attackType = AttackType::Attack2;
						m_maxFrames = 9;
						break;
					case 2:
						m_attackType = AttackType::Attack3;
						m_maxFrames = 9;
						break;
					}
				}
			}

			break;
		case AnimType::Attack:
			switch (m_attackType)
			{
			case AttackType::Attack1:
				if ((m_animIndex != 0 && m_animTimer <= 0.0f) || m_animTimer == 0.5f)
				{
					Math::Vector2 pos = m_pos;
					float angle = DirectX::XMConvertToRadians(180.0f);
					angle += DirectX::XMConvertToRadians(45.0f * m_animIndex);
					pos = m_pos;
					pos += { cosf(angle) * 100, sinf(angle) * 100};
					int wait = (m_maxFrames - m_animIndex) * 6;
					float dx = playerPos.x - (m_pos.x + pos.x);
					float dy = playerPos.y - (m_pos.y + pos.y);
					float angleToPlayer = std::atan2(dy, dx);
					ShotBullet(pos, angleToPlayer, 12.0f, 0.4f, wait);
				}

				if (m_animIndex == 0 && m_animTimer <= 0.0f)
				{
					m_attackCount++;
					if (m_attackCount >= 4)
					{
						m_attackCount = 0;
						m_animType = AnimType::Idle;
						m_maxFrames = 9;
					}
				}
				break;
			case AttackType::Attack2:
				if (m_animTimer <= 0.0f)
				{
					Math::Vector2 pos = m_pos;
					float dx = playerPos.x - m_pos.x;
					float dy = playerPos.y - m_pos.y;
					float angleToPlayer = std::atan2(dy, dx);
					switch (m_animIndex % 3)
					{
					case 0:
						angleToPlayer += DirectX::XMConvertToRadians(-20.0f);
						ShotBullet(pos, angleToPlayer, 8.0f, 0.4f);
						angleToPlayer += DirectX::XMConvertToRadians(30.0f);
						ShotBullet(pos, angleToPlayer, 9.0f, 0.4f);
						angleToPlayer += DirectX::XMConvertToRadians(30.0f);
						ShotBullet(pos, angleToPlayer, 6.0f, 0.4f);
						break;
					case 1:
						angleToPlayer += DirectX::XMConvertToRadians(-30.0f);
						ShotBullet(pos, angleToPlayer, 7.0f, 0.4f);
						angleToPlayer += DirectX::XMConvertToRadians(30.0f);
						ShotBullet(pos, angleToPlayer, 10.0f, 0.4f);
						angleToPlayer += DirectX::XMConvertToRadians(30.0f);
						ShotBullet(pos, angleToPlayer, 7.0f, 0.4f);
						break;
					case 2:
						angleToPlayer += DirectX::XMConvertToRadians(-40.0f);
						ShotBullet(pos, angleToPlayer, 6.0f, 0.4f);
						angleToPlayer += DirectX::XMConvertToRadians(30.0f);
						ShotBullet(pos, angleToPlayer, 9.0f, 0.4f);
						angleToPlayer += DirectX::XMConvertToRadians(30.0f);
						ShotBullet(pos, angleToPlayer, 8.0f, 0.4f);
						break;
					}
				}

				if (m_animIndex == 0 && m_animTimer <= 0.0f)
				{
					m_attackCount++;
					if (m_attackCount >= 5)
					{
						m_attackCount = 0;
						m_animType = AnimType::Idle;
						m_maxFrames = 9;
					}
				}
				break;
			case AttackType::Attack3:

				if (m_animIndex == 0 && m_animTimer <= 0.0f)
				{
					m_attackCount++;
					if (m_attackCount >= 5)
					{
						m_attackCount = 0;
						m_animType = AnimType::Idle;
						m_maxFrames = 9;
					}
				}

				if (m_animTimer <= 0.0f)
				{
					Math::Vector2 pos = m_pos;
					float angle = DirectX::XMConvertToRadians(180.0f);
					angle += (m_animIndex + m_attackCount * m_maxFrames) * DirectX::XMConvertToRadians(6.0f);
					for (int i = 0; i < 16; ++i)
					{
						angle += i * DirectX::XMConvertToRadians(22.5f);
						ShotBullet(pos, angle, 8.0f, 0.4f);
					}
				}

				break;
			}

			break;
		}
	}
	else
	{
		if (m_animType != AnimType::Death)
		{
			m_animType = AnimType::Death;
			m_maxFrames = 8;
			m_animSpeed = 0.5f;
			SCENE.AddScore(200);
		}

		m_alpha += 0.007f;
	}

	AnimUpdate();
}

void FireWorm::Draw()
{
	int fireWormY = 0 * 90;
	switch (m_animType)
	{
	case FireWorm::AnimType::Walk:
		fireWormY = 0 * 90;
		break;
	case FireWorm::AnimType::Idle:
		fireWormY = 1 * 90;
		break;
	case FireWorm::AnimType::Attack:
		fireWormY = 1 * 90;
		break;
	case FireWorm::AnimType::Death:
		fireWormY = 3 * 90;
		break;
	case AnimType::Shout:
		fireWormY = 2 * 90;
		break;
	default:
		break;
	}
	Math::Matrix mat =
		Math::Matrix::CreateScale(-5.0f, 5.0f, 1) *
		Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ m_animIndex * 90, fireWormY, 90, 90 }, m_alpha);
}

void FireWorm::Release()
{
}
