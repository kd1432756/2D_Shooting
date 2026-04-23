#include "Player.h"
#include "Bullet/PlayerBullet.h"

void Player::Init()
{
	m_tex.Load("Texture/GameScene/player.png");
	m_bulletTex.Load("Texture/GameScene/player_bullet.png");

	m_pos = { -500, 0 };
	m_speed = 3.0f;

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (!m_bullets[i])
		{
			m_bullets[i] = new PlayerBullet();
			m_bullets[i]->SetTexture(&m_bulletTex);
		}
	}
}

void Player::Update()
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pos.x -= m_speed;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pos.x += m_speed;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pos.y -= m_speed;
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pos.y += m_speed;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (m_shootTimer <= 0.0f)
		{
			Shoot();
			m_shootTimer = m_shootCooldown;
		}
	}

	if (m_shootTimer > 0.0f)
	{
		m_shootTimer -= 1.0f / 60.0f;
	}

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (m_bullets[i]->IsActive())
		{
			m_bullets[i]->Update();
		}
	}
}

void Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0));
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0, 0, 64, 128 }, 1.0f);

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if(m_bullets[i]->IsActive())
		{
			m_bullets[i]->Draw();
		}
	}
}

void Player::Release()
{
	m_tex.Release();

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if(m_bullets[i])
		{
			delete m_bullets[i];
			m_bullets[i] = nullptr;
		}
	}
}

void Player::Shoot()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (m_bullets[i] && !m_bullets[i]->IsActive())
		{
			m_bullets[i]->SetPosition(m_pos);
			m_bullets[i]->SetActive(true);
			break;
		}
	}
}
