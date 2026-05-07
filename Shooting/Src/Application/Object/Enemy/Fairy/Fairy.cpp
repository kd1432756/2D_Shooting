#include "Fairy.h"
#include "Application/Scene.h"

void Fairy::Init()
{
	m_isActive = false;
	m_isAlive = false;

	m_speed = 1.0f;

	m_animTimer = 0.0f;
	m_animIndex = 0;
	m_animSpeed = 0.15f;
	m_maxFrames = 8;
	m_alpha = 1.3f;

	m_isShotReady = false;
	m_hasShoted = false;
}

void Fairy::Update(Math::Vector2 playerPos)
{
	if (m_isAlive) 
	{
        if (m_isShotReady)
        {
            if (m_animIndex == 7 && m_animTimer <= 0.0f)
            {
                float dx = playerPos.x - m_pos.x;
                float dy = playerPos.y - m_pos.y;
                float angleToPlayer = std::atan2(dy, dx);

                ShotBullet(m_pos, angleToPlayer, 15.0f, 0.4f);

                m_isShotReady = false;
                m_hasShoted = true;
            }
        }
        else
        {
            m_pos.x -= m_speed;

            if (!m_hasShoted && m_pos.x <= 0.0f)
            {
                m_isShotReady = true;
                m_pos.x = 0.0f;

                m_animTimer = 0.0f;
                m_animIndex = 0;
            }
        }

		if (m_pos.x <= -SCENE.GetWindowHalfSize().x - 64)
		{
			m_isActive = false;
		}
	}

	AnimUpdate();
}

void Fairy::Draw()
{
	int fairyY = 0 * 25;
	Math::Matrix mat =
		Math::Matrix::CreateScale(-2.5f, 2.5f, 1) *
		Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ m_animIndex * 32, fairyY, 32, 32 }, m_alpha);
}

void Fairy::Release()
{
}
