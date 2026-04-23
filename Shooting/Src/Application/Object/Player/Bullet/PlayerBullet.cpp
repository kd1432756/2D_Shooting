#include "PlayerBullet.h"

void PlayerBullet::Init()
{
	m_isActive = false;
}

void PlayerBullet::Update()
{
	m_pos.x += m_speed;
	if (m_pos.x > 1280)
	{
		m_isActive = false;
	}
}

void PlayerBullet::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0));
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ 0, 0, 16, 16 }, 1.0f);
}

void PlayerBullet::Release()
{

}
