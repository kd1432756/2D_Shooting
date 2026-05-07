#include "Slime.h"
#include "Application/Scene.h"

void Slime::Init()
{
	m_isActive = false;
	m_isAlive = false;

	m_speed = 1.0f;

	m_animTimer = 0.0f;
	m_animIndex = 0;
	m_animSpeed = 0.15f;
	m_maxFrames = 4;
	m_alpha = 1.0f;
}

void Slime::Update()
{
	if (m_isAlive) 
	{
		m_pos.x -= m_speed;

		if (m_pos.x <= -SCENE.GetWindowHalfSize().x - 64)
		{
			m_isActive = false;
		}
	}

	AnimUpdate();
}

void Slime::Draw()
{
	int slimeY = 0 * 25;
	if (!m_isAlive)slimeY = 1 * 25;
	Math::Matrix mat =
		Math::Matrix::CreateScale(2.0f) *
		Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ m_animIndex * 32, slimeY, 32, 25 }, m_alpha);
}

void Slime::Release()
{
}
