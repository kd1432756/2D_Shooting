#include "PlayerBullet.h"
#include <Application/Scene.h>

void PlayerBullet::Init()
{
	m_isActive = false;
}

void PlayerBullet::Update()
{
	if (m_bulletType == BulletType::Parabola)
	{
		m_pos.x += cos(DirectX::XMConvertToRadians(m_angleDeg)) * m_speed;
		m_pos.y += sin(DirectX::XMConvertToRadians(m_angleDeg)) * m_speed + 0.5f * 9.8f * (m_speed / 60.0f) * (m_speed / 60.0f);
	}
	else
	{
		float rad = DirectX::XMConvertToRadians(m_angleDeg);
		m_pos.x += cos(rad) * m_speed;
		m_pos.y += sin(rad) * m_speed;
	}

	if (m_pos.x > SCENE.GetWindowHalfSize().x + m_rectSize.x / 2
		||m_pos.y > SCENE.GetWindowHalfSize().y + m_rectSize.y / 2
		|| m_pos.y < -SCENE.GetWindowHalfSize().y - m_rectSize.y / 2)
	{
		m_isActive = false;
	}
}

void PlayerBullet::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0));
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
			if (m_angleDeg > 0)
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
