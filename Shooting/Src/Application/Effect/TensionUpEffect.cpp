#include "TensionUpEffect.h"

void TensionUpEffect::Init()
{
	m_isActive = false;
	m_pos = {};
	m_size = 2.0f;

	m_animTimer = 0.0f;
	m_animIndex = 0;
	m_animSpeed = 0.02f;
	m_maxFrames = 17;
}

void TensionUpEffect::Update(Math::Vector2& pos)
{
	m_pos = pos;

	AnimUpdate();
}

void TensionUpEffect::Draw()
{
	Math::Matrix mat =
		Math::Matrix::CreateScale(m_size) *
		Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ m_animIndex * 32, 0, 32, 32 }, 0.5f);
}

void TensionUpEffect::Release()
{
}
