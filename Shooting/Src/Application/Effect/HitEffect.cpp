#include "HitEffect.h"

void HitEffect::Init()
{
	m_isActive = false;
	m_pos = {};
	m_size = 1.0f;

	m_animTimer = 0.0f;
	m_animIndex = 0;
	m_animSpeed = 0.03f;
	m_maxFrames = 4;
}

void HitEffect::Update()
{
	AnimUpdate();
}

void HitEffect::Draw()
{
	Math::Matrix mat =
		Math::Matrix::CreateScale(m_size) *
		Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ m_animIndex * 33, 0, 33, 33 }, 1.0f);
}

void HitEffect::Release()
{
}
