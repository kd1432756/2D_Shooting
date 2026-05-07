#include "Application/Scene.h"
#include "EnemyBullet.h"

void EnemyBullet::Init()
{
    m_isActive = false;
    m_pos = {};
    m_angleRad = 0.0f;
    m_speed = 0.0f;
}

void EnemyBullet::Update()
{
    if (!m_isActive) return;

    m_pos.x += cosf(m_angleRad) * m_speed;
    m_pos.y += sinf(m_angleRad) * m_speed;

    float halfWidth = SCENE.GetWindowHalfSize().x + 64.0f;
    float halfHeight = SCENE.GetWindowHalfSize().y + 64.0f;

    if (m_pos.x < -halfWidth || m_pos.x > halfWidth ||
        m_pos.y < -halfHeight || m_pos.y > halfHeight)
    {
        m_isActive = false;
    }
}

void EnemyBullet::Draw()
{
    Math::Matrix mat =
        Math::Matrix::CreateScale(m_size) *
        Math::Matrix::CreateRotationZ(m_angleRad) * 
        Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);

    SHADER.m_spriteShader.SetMatrix(mat);

    SHADER.m_spriteShader.DrawTex(m_tex, Math::Rectangle{ 0, 0, 32, 32 }, 1.0f);
}

void EnemyBullet::Release()
{
}
