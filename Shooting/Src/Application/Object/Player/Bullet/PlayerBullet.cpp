#include "PlayerBullet.h"
#include <Application/Scene.h>

void PlayerBullet::Init()
{
	m_isActive = false;
}

void PlayerBullet::Update(Math::Vector2& playerPos)
{
	if (m_bulletType == BulletType::Parabola)
	{
		{
			prevPos = m_pos;
			m_distance += m_speed * 0.5f;
			float t = m_distance / TARGET_DIST;
			if (t >= 1.0f)t = 1.0f;

			m_pos.x = playerPos.x + t * TARGET_DIST;
			m_pos.y = playerPos.y + 4.0f * t * (1.0f - t) * MAX_HEIGHT;

			float dx = m_pos.x - prevPos.x;
			float dy = m_pos.y - prevPos.y;

			m_angleRad = atan2f(dy, dx);

			if (t >= 1.0f)
			{
				m_isActive = false;
				m_distance = 0;
			}
		}
	}
	else
	{
		m_pos.x += cosf(m_angleRad) * m_speed;
		m_pos.y += sinf(m_angleRad) * m_speed;

		if (m_pos.x > SCENE.GetWindowHalfSize().x + m_rectSize.x / 2
			|| m_pos.y > SCENE.GetWindowHalfSize().y + m_rectSize.y / 2
			|| m_pos.y < -SCENE.GetWindowHalfSize().y - m_rectSize.y / 2)
		{
			m_isActive = false;
		}
	}
}

void PlayerBullet::Draw()
{
	Math::Matrix _scale = Math::Matrix::CreateScale(m_size);
	Math::Matrix _rotate = Math::Matrix::CreateRotationZ(m_angleRad);
	SHADER.m_spriteShader.SetMatrix(_scale * _rotate * Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0));
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ 0, 0, (int)m_rectSize.x, (int)m_rectSize.y }, 1.0f);
}

void PlayerBullet::Release()
{

}

PlayerBullet::SplitSide PlayerBullet::GetMissionSuccess()
{
	switch (m_bulletType)
	{
	case PlayerBullet::BulletType::Pircing:
		if (m_killCount >= 2)
		{
			return PlayerBullet::SplitSide::Normal;
		}
		break;
	case PlayerBullet::BulletType::Split:
		if (m_killCount)
		{
			if (m_angleRad > 0)
			{
				return PlayerBullet::SplitSide::Left;
			}
			else
			{
				return PlayerBullet::SplitSide::Right;
			}
		}
		break;
	case PlayerBullet::BulletType::Parabola:
		if (m_killCount)
		{
			return PlayerBullet::SplitSide::Normal;
		}
		break;
	default:
		break;
	}

	m_killCount = 0;

	return PlayerBullet::SplitSide::None;
}
