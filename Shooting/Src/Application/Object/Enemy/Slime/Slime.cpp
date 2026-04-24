#include "Slime.h"

void Slime::Init()
{
	m_isActive = false;
}

void Slime::Update()
{
}

void Slime::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0));
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ 0, 0, 128, 128 }, 1.0f);
}

void Slime::Release()
{
}
