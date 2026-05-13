#include "BaseEffect.h"

void BaseEffect::Init()
{

}

void BaseEffect::Update()
{

}

void BaseEffect::Draw()
{

}

void BaseEffect::Release()
{

}

void BaseEffect::AnimUpdate()
{
	m_animTimer += 1.0f / 60.0f;

	if (m_animTimer >= m_animSpeed)
	{
		m_animTimer = 0.0f;
		m_animIndex++; // 次のコマへ
	}

	if (m_animIndex >= m_maxFrames)
	{
		m_animIndex = 0;
		m_isActive = false;
		return;
	}
}