#include "Player.h"
#include "Bullet/PlayerBullet.h"

void Player::Init()
{
	m_isActive = true;

	m_tex.Load("Texture/GameScene/player.png");
	m_bulletTex.Load("Texture/GameScene/player_bullet.png");

	m_pos = { -500, 0 };
	m_speed = 3.0f;
	m_cooldownTimer = 0.0f;

	for (auto& bullet : m_specialBullets)
	{
		if (!bullet)
		{
			bullet = new PlayerBullet();
			bullet->SetTexture(&m_bulletTex);
		}
	}
}

void Player::Update()
{
	switch (m_state)
	{
	case State::Normal:

		if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			ChangeState(State::ReadyToShoot);
			break;
		}

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
				m_isShotRequested = true;
				m_shootTimer = m_shootCooldown;
			}
		}

		if (m_shootTimer > 0.0f)
		{
			m_shootTimer -= 1.0f / 60.0f;
		}

		break;

	case State::ReadyToShoot:
		if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
		{
			ChangeState(State::Cooldown);
			break;
		}
		else if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			m_specialBulletType = SpecialBulletType::TypeA;

			ChangeState(State::Firing);
			switch(m_specialBulletType)
			{
			case SpecialBulletType::TypeA:
				ShootSpecial(0,PlayerBullet::BulletType::Pircing);
				break;
			case SpecialBulletType::TypeB:
				ShootSpecial(SPREAD_ANGLE, PlayerBullet::BulletType::Split);
				ShootSpecial(-SPREAD_ANGLE, PlayerBullet::BulletType::Split);
				break;
			case SpecialBulletType::TypeC:
				ShootSpecial(0, PlayerBullet::BulletType::Parabola);
				break;
			}
		}

		break;

	case State::Firing:
		//必殺技の処理
		if(IsFireingFinished())
		{
			m_isMissionSuccess = false;
			m_isLMissionSuccess = false;
			m_isRMissionSuccess = false;

			ChangeState(State::Cooldown);
		}

		break;

		case State::Cooldown:
		if (m_cooldownTimer <= 0.0f)
		{
			ChangeState(State::Normal);
		}
		else
		{
			m_cooldownTimer -= 1.0f / 60.0f;
		}

		break;
	}

	for (auto& bullet : m_specialBullets)
	{
		if (bullet->IsActive())
		{
			bullet->Update();
		}
	}
}

void Player::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0));
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0, 0, 64, 128 }, 1.0f);

	for (auto& bullet : m_specialBullets)
	{
		if(bullet->IsActive())
		{
			bullet->Draw();
		}
	}
}

void Player::Release()
{
	m_tex.Release();

	for (auto& bullet : m_specialBullets)
	{
		if(bullet)
		{
			delete bullet;
			bullet = nullptr;
		}
	}
}

bool Player::CheckMissionSuccess()
{
	return (m_isMissionSuccess || (m_isLMissionSuccess && m_isRMissionSuccess));
}

void Player::ShootSpecial(float angle, PlayerBullet::BulletType type)
{
	for (auto& bullet : m_specialBullets)
	{
		if (bullet && !bullet->IsActive())
		{
			bullet->SetActive(true);
			bullet->SetPosition(m_pos);
			bullet->SetAngle(angle);
			bullet->SetBulletType(type);
			break;
		}
	}
}

bool Player::IsFireingFinished()
{
	int aliveCount = 0;
	for (const auto& bullet : m_specialBullets) {
		// 必殺技専用の弾タイプかつ、まだ有効なもの
		if (bullet->IsActive()) {
			aliveCount++;
		}
	}

	if (aliveCount == 0)
	{
		for (auto& bullet : m_specialBullets)
		{
			switch(bullet->GetMissionSuccess())
			{
				case PlayerBullet::SplitSide::Normal:
					m_isMissionSuccess = true;
					break;
				case PlayerBullet::SplitSide::Left:
					m_isLMissionSuccess = true;
					break;
				case PlayerBullet::SplitSide::Right:
					m_isRMissionSuccess = true;
					break;
				default :
					break;
			}
		}

		if (CheckMissionSuccess())
		{
			// ミッション成功の処理
		}

		return true;
	}
	return false;
}

void Player::ChangeState(State newState) 
{
	State prevState = m_state; // 前の状態を覚えておく
	m_state = newState;

	if (newState == State::Cooldown) 
	{
		if (prevState == State::Firing) 
		{
			m_cooldownTimer = 2.0f; // 撃った後は長い硬直
		}
		else 
		{
			m_cooldownTimer = 0.5f; // キャンセル時は短い硬直
		}
	}
}