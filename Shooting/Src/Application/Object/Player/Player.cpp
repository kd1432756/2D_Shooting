#include "Player.h"
#include "Bullet/PlayerBullet.h"
#include "Application/Scene.h"

void Player::Init()
{
	m_isActive = true;
	m_isAlive = true;

	m_animState = AnimState::Idle;

	m_tex.Load("Texture/GameScene/player.png");
	m_playerBulletTex.Load("Texture/GameScene/player_bullet.png");

	m_pos = { -500, 0 };
	m_vec = {};
	m_speed = 5.0f;
	m_animSpeed = 0.1f;
	m_cooldownTimer = 0.0f;
	m_shootCooldown = 0.5f;

	for (auto& bullet : m_specialBullets)
	{
		if (!bullet)
		{
			bullet = new PlayerBullet();
			bullet->SetTexture(&m_playerBulletTex);
		}
	}

	switch (rand() % 3)
	{
	case 0:		m_specialBulletType = SpecialBulletType::TypeA;	break;
	case 1:		m_specialBulletType = SpecialBulletType::TypeB;	break;
	case 2:		m_specialBulletType = SpecialBulletType::TypeC;	break;
	}
	m_specialBulletType = SpecialBulletType::TypeC;
}

void Player::Update()
{
	m_vec = {};

	if (m_isAlive)
	{
		switch (m_state)
		{
		case State::Normal:		

			if (GetAsyncKeyState('X') & 0x8000)
			{
				if (GetTensionPercent() >= 1)
				{
					ChangeState(State::ReadyToShoot);
					break;
				}
			}
			
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				m_vec.x -= 1;
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				m_vec.x += 1;
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				m_vec.y -= 1;
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				m_vec.y += 1;
			}

			m_vec.Normalize();

			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				m_vec *= 0.3f;
			}

			if (m_vec != Math::Vector2{})
			{
				if (m_animState == AnimState::Idle) ChangeAnimState(AnimState::Run);
			}
			else
			{
				if (m_animState == AnimState::Run) ChangeAnimState(AnimState::Idle);
			}

			if (GetAsyncKeyState('Z') & 0x8000)
			{
				if (m_shootTimer <= 0.0f /* && m_animState != AnimState::Attack*/)
				{
					//m_isAnimShotDone = false;
					m_isShotRequested = true;
					m_shootTimer = m_shootCooldown;
					//ChangeAnimState(AnimState::Attack);
				}
			}

			if (m_shootTimer > 0.0f)
			{
				m_shootTimer -= 1.0f / 60.0f;
			}

			//if (m_animState != AnimState::Attack && m_shootTimer <= 0.0f)
			{
				m_pos += m_vec * m_speed;
			}

			if (m_pos.x >= SCENE.GetWindowHalfSize().x - 32)m_pos.x = SCENE.GetWindowHalfSize().x - 32;
			if (m_pos.x <= -SCENE.GetWindowHalfSize().x + 32)m_pos.x = -SCENE.GetWindowHalfSize().x + 32;
			if (m_pos.y >= 125)m_pos.y = 125;
			if (m_pos.y <= -290)m_pos.y = -290;

			break;

		case State::ReadyToShoot:
			if (!(GetAsyncKeyState('X') & 0x8000))
			{
				ChangeState(State::Cooldown);
				ChangeAnimState(AnimState::Idle);
				break;
			}
			else if (GetAsyncKeyState('Z') & 0x8000)
			{
				ChangeState(State::Firing);
				m_cutInTimer = 0.0f;    // タイマー開始
				//m_isAnimShotDone = false;
				m_isSpecialReserved = true; // 必殺技を予約！
				switch (m_specialBulletType)
				{
				case SpecialBulletType::TypeA:
					ChangeAnimState(AnimState::LowAttack);
					break;
				case SpecialBulletType::TypeB:
					ChangeAnimState(AnimState::LowAttack);
					break;
				case SpecialBulletType::TypeC:
					ChangeAnimState(AnimState::HighAttack);
					break;
				}
			}

			break;

		case State::Firing:
			//必殺技の処理
			m_cutInTimer += 1.0f / 60.0f; // タイマーを進める
			if (!m_isSpecialReserved)
			{
				if (IsFireingFinished())
				{
					m_isMissionSuccess = false;
					m_isLMissionSuccess = false;
					m_isRMissionSuccess = false;

					ChangeState(State::Cooldown);
				}
			}			

			break;

		case State::Cooldown:
			if (m_cooldownTimer <= 0.0f)
			{
				ChangeState(State::Normal);
			}
			else
			{
				m_cooldownTimer -= 1.0f / 60.0f + (m_hp * 0.01f);
			}

			break;
		}
	}

	AnimUpdate();

	for (auto& bullet : m_specialBullets)
	{
		if (bullet->IsActive())
		{
			bullet->Update(m_pos);
		}
	}

	if (GetAsyncKeyState('9') & 0x8000)
	{
		AddTension(1.0f);
	}
}

void Player::Draw()
{
	if (m_state == State::ReadyToShoot || m_state == State::Firing)
	{
		Math::Color color(1, 0, 0, 0.5f);
		if (m_state == State::Firing) color = { 1, 0, 0, 0.2f };

		switch (m_specialBulletType)
		{
		case SpecialBulletType::TypeA:
		{
			float x = (SCENE.GetWindowHalfSize().x + m_pos.x) * 0.5f;
			float w = (SCENE.GetWindowHalfSize().x - m_pos.x) * 0.5f;

			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			SHADER.m_spriteShader.DrawBox(x, m_pos.y, w, 8, &color, true);
		}
			break;
		case SpecialBulletType::TypeB:
		{
			float angleRad = DirectX::XMConvertToRadians(SPREAD_ANGLE);
			float distanceX = SCENE.GetWindowHalfSize().x * 2 - m_pos.x;
			float fullLength = distanceX / cosf(angleRad);
			float extentX = fullLength * 0.5f;

			Math::Matrix matUpper =
				Math::Matrix::CreateTranslation(-m_pos.x, -m_pos.y, 0) * // (1) 発射地点を0,0へ
				Math::Matrix::CreateRotationZ(angleRad) * // (2) 回転
				Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);     // (3) 元の位置に戻す
			SHADER.m_spriteShader.SetMatrix(matUpper);
			SHADER.m_spriteShader.DrawBox(m_pos.x + extentX, m_pos.y, (int)extentX, 8, &color, true);

			Math::Matrix matLower =
				Math::Matrix::CreateTranslation(-m_pos.x, -m_pos.y, 0) *
				Math::Matrix::CreateRotationZ(-angleRad) *
				Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
			SHADER.m_spriteShader.SetMatrix(matLower);
			SHADER.m_spriteShader.DrawBox(m_pos.x + extentX, m_pos.y, (int)extentX, 8, &color, true);
		}
			break;
		case SpecialBulletType::TypeC:

			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			SHADER.m_spriteShader.DrawCircle(m_pos.x + 700.0f, m_pos.y, 32, &color, true);

			break;
		}
	}

	if (m_state == State::Firing && m_cutInTimer < CUTIN_DURATION) 
	{
		float t = m_cutInTimer / CUTIN_DURATION;
		float cutInX = 600.0f - (t * t * (3 - 2 * t) * 400.0f);
		float cutInY = 0.0f;

		float alpha = 0.6f;
		if (t > 0.8f) alpha = 0.6f * (1.0f - (t - 0.8f) / 0.2f);

		Math::Matrix mat =
			Math::Matrix::CreateScale(16.0f) *
			Math::Matrix::CreateTranslation(cutInX, cutInY, 0);

		int playerY;

		if (m_specialBulletType != SpecialBulletType::TypeC)
		{
			playerY = 4 * 64;
		}
		else
		{
			playerY = 3 * 64;
		}

		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0, playerY, 64, 64 }, alpha);
	}
	{
		int srcX;
		srcX = m_animIndex * 64;
		int srcY = 0;
		switch (m_animState) 
		{
		case AnimState::Idle:			srcY = 0 * 64; break;
		case AnimState::Run:			srcY = 1 * 64; break;
		/*case AnimState::Attack:			//srcY = 1 * 64; break;
			if (m_vec != Math::Vector2{})	srcY = 1 * 64;
			else							srcY = 0 * 64;
											break;*/
		case AnimState::HighAttack:		srcY = 3 * 64; break;
		case AnimState::LowAttack:		srcY = 4 * 64; break;
		case AnimState::Death:			srcY = 5 * 64; break;
		}

		Math::Matrix mat =
			Math::Matrix::CreateScale(2.0f) *
			Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ srcX, srcY, 64, 64 }, 1.0f);
	}

	for (auto& bullet : m_specialBullets)
	{
		if(bullet->IsActive())
		{
			bullet->Draw();
		}
	}

	{
		
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

void Player::AnimUpdate()
{
	if (m_state == State::ReadyToShoot)return;
	if (m_state == State::Firing && m_cutInTimer < CUTIN_DURATION)return;

	m_animTimer += 1.0f / 60.0f;

	if (m_animTimer >= m_animSpeed)
	{
		m_animTimer = 0.0f;
		m_animIndex++; // 次のコマへ
	}

	if (!m_isAnimShotDone) {

		// ① 通常攻撃のアニメ コマ目
		/*if (m_animState == AnimState::Attack && m_animIndex == 4 && m_animTimer >= m_animSpeed * 0.2f) {
			m_isShotRequested = true; // 通常弾発射
			m_isAnimShotDone = true;
		}*/

		// ② 必殺技のアニメ コマ目（High / Low 共通）
		if ((m_animState == AnimState::HighAttack || m_animState == AnimState::LowAttack)
			&& m_animIndex == 6) {

			if (m_isSpecialReserved) 
			{
				switch (m_specialBulletType)
				{
				case SpecialBulletType::TypeA:
					ShootSpecial(0, PlayerBullet::BulletType::Pircing);
					break;
				case SpecialBulletType::TypeB:
					ShootSpecial(SPREAD_ANGLE, PlayerBullet::BulletType::Split);
					ShootSpecial(-SPREAD_ANGLE, PlayerBullet::BulletType::Split);
					break;
				case SpecialBulletType::TypeC:
					ShootSpecial(0, PlayerBullet::BulletType::Parabola);
					break;
				}
				m_isSpecialReserved = false; // 予約消化
				//m_isAnimShotDone = true;
			}
		}
	}


	if (m_animIndex >= m_maxFrames)
	{		
		if (m_animState == AnimState::Death)
		{
			m_animIndex = m_maxFrames - 1;
			return;
		}

		int aliveCount = 0;
		for (const auto& bullet : m_specialBullets)
		{
			if (bullet->IsActive())
			{
				aliveCount++;
			}
		}

		if (aliveCount != 0)
		{
			m_animIndex = m_maxFrames - 1;
			return;
		}

		m_animIndex = 0;
		//m_isAnimShotDone = false; // 次のアニメのためにリセット
		if (m_animState != AnimState::Idle && m_animState != AnimState::Run) ChangeAnimState(AnimState::Idle);
	}
}

void Player::ChangeAnimState(AnimState animState)
{
	if (m_animState == animState && animState != AnimState::Attack) return;

	m_animIndex = 0;

	switch (animState)
	{
	case AnimState::Idle:
		m_animState = AnimState::Idle;
		m_maxFrames = IDLE_FRAMES;
		break;
	case AnimState::Run:
		m_animState = AnimState::Run;
		m_maxFrames = RUN_FRAMES;
		break;
	case AnimState::Attack:
		m_animState = AnimState::Attack;
		m_maxFrames = ATTACK_FRAMES;
		break;
	case AnimState::HighAttack:
		m_animState = AnimState::HighAttack;
		m_maxFrames = HIGH_ATTACK_FRAMES;
		break;
	case AnimState::LowAttack:
		m_animState = AnimState::LowAttack;
		m_maxFrames = LOW_ATTACK_FRAMES;
		break;
	case AnimState::Death:
		m_animState = AnimState::Death;
		m_maxFrames = DEATH_FRAMES;
		break;
	}
}

void Player::ShootSpecial(float angle, PlayerBullet::BulletType type)
{
	for (auto& bullet : m_specialBullets)
	{
		if (bullet && !bullet->IsActive())
		{
			bullet->SetActive(true);
			bullet->SetPosition(m_pos);
			bullet->SetAngle(DirectX::XMConvertToRadians(angle));
			bullet->SetBulletType(type);
			bullet->SetKillCount(0);
			break;
		}
	}
}

bool Player::IsFireingFinished()
{
	int aliveCount = 0;
	for (const auto& bullet : m_specialBullets) 
	{
		// 必殺技専用の弾タイプかつ、まだ有効なもの
		if (bullet->IsActive()) 
		{
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

		if (m_isMissionSuccess || (m_isLMissionSuccess && m_isRMissionSuccess))
		{
			ChangeHP(1);
			m_tensionGauge = 0.0f;
			switch (rand() % 3)
			{
			case 0:		m_specialBulletType = SpecialBulletType::TypeA;	break;
			case 1:		m_specialBulletType = SpecialBulletType::TypeB;	break;
			case 2:		m_specialBulletType = SpecialBulletType::TypeC;	break;
			}
		}
		else
		{
			m_tensionGauge = 50.0f;
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
			m_cooldownTimer = 1.0f; // 撃った後は長い硬直
		}
		else 
		{
			m_cooldownTimer = 0.5f; // キャンセル時は短い硬直
		}
	}
}

// HPを変更する共通関数
void Player::ChangeHP(int amount) {
	// 現在のHPに加算（amountがマイナスなら引き算になる）
	m_hp += amount;

	// --- 最小値・最大値のガード ---
	if (m_hp <= 0) {
		m_hp = 0;
		m_isAlive = false;
		ChangeAnimState(AnimState::Death);
	}

	// 最大HP（テンションMAX）を5とした場合
	const int MAX_HP = 9;
	if (m_hp > MAX_HP) {
		m_hp = MAX_HP;
	}

	// --- HP（テンション）が変わったことによる副次的な処理 ---
	// ここに速度更新などを書いておけば、増減どちらでも自動で反映される
	m_shootCooldown = 0.5f - 0.05f * m_hp;
	if (m_shootCooldown < 0.1f) m_shootCooldown = 0.1f;
	m_speed = 5.0f + (m_hp * 0.4f) * 2.0f;
	m_animSpeed = 0.1f - (m_hp * 0.01f);
	if (m_animSpeed < 0.03f) m_animSpeed = 0.03f; // 速すぎ防止
}