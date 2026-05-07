#include "SkullWolf.h"
#include "Application/Scene.h"

void SkullWolf::Init()
{
	m_isActive = false;
	m_isAlive = false;

	m_speed = 3.0f;

	m_animTimer = 0.0f;
	m_animIndex = 0;
	m_animSpeed = 0.15f;
	m_maxFrames = 5;
	m_moveCount = 0;
	m_alpha = 1.0f;
}

void SkullWolf::Update()
{
	if (m_isAlive)
	{
		if (m_animIndex >= 1 && m_animIndex <= 3)
		{
			m_pos.x -= m_speed;
		}

		if (m_animIndex == 2 && m_animTimer <= 0.0f)
		{
			m_moveCount++;

			if (m_moveCount >= 3)
			{
				float angle = DirectX::XMConvertToRadians(180.0f);
				float spreadAngle = DirectX::XMConvertToRadians(15.0f);
				Math::Vector2 pos = m_pos + Math::Vector2{ 0,-24 };
				ShotBullet(pos, angle - spreadAngle, 6.0f, 0.4f);
				ShotBullet(pos, angle, 6.0f, 0.4f);
				ShotBullet(pos, angle + spreadAngle, 6.0f, 0.4f);

				m_moveCount = 0;
			}
		}


		if (m_pos.x <= -SCENE.GetWindowHalfSize().x - 64)
		{
			m_isActive = false;
		}
	}
	else
	{
		if (m_maxFrames == 6)m_maxFrames = 7;
	}

	AnimUpdate();
}

void SkullWolf::Draw()
{
	int skullWolfY = 1 * 64;
	if (!m_isAlive)skullWolfY = 3 * 64;
	Math::Matrix mat =
		Math::Matrix::CreateScale(1.5f) *
		Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ m_animIndex * 64, skullWolfY, 64, 64 }, m_alpha);
}

void SkullWolf::Release()
{
}
