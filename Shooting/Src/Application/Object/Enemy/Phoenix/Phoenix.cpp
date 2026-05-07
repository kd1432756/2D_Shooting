#include "Phoenix.h"
#include "Application/Scene.h"

void Phoenix::Init()
{
	m_isActive = false;
	m_isAlive = false;

	m_speed = 1.5f;

	m_animTimer = 0.0f;
	m_animIndex = 0;
	m_animSpeed = 0.15f;
	m_maxFrames = 4;
	m_alpha = 1.0f;
	m_moveCount = 0;
	m_isMove = true;
}

void Phoenix::Update()
{
	if (m_isAlive) 
	{
		if (m_isMove)
		{
			
			m_pos.x -= m_speed;
		}
		else if (m_animIndex == 2 && m_animTimer <= 0.0f)
		{
			if (m_moveCount == 1 || m_moveCount == 3)
			{
				float angle = DirectX::XMConvertToRadians(180.0f);
				angle += DirectX::XMConvertToRadians((m_moveCount - 1) * 0.5f * 22.5f);
				Math::Vector2 pos = m_pos;
				int num = 8;
				for (int i = 0; i < num; ++i)
				{
					ShotBullet(pos, angle, 6.0f, 0.4f);
					angle += DirectX::XMConvertToRadians(360.0f / num);
				}
			}
		}

		m_pos.y -= m_speed * 0.1f;

		if (m_animIndex == 2)
		{
			m_pos.y += m_speed * 0.4f;
		}

		if (m_animIndex == 0 && m_animTimer <= 0.0f)
		{
			m_moveCount++;
			if (m_moveCount >= 4)
			{
				m_isMove = !m_isMove;
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
		if (m_maxFrames == 4)m_maxFrames = 8;
	}

	AnimUpdate();
}

void Phoenix::Draw()
{
	int phoenixY = 1 * 64;
	if (!m_isMove)phoenixY = 0 * 64;
	if (!m_isAlive)phoenixY = 4 * 64;
	Math::Matrix mat =
		Math::Matrix::CreateScale(-2.5f, 2.5f, 1) *
		Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ m_animIndex * 64, phoenixY, 64, 64 }, m_alpha);
}

void Phoenix::Release()
{
}
