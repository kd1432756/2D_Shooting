#include "GameScene.h"
#include "Application/Scene.h"
#include "Application/Object/Player/Player.h"
#include "Application/Object/Player/Bullet/PlayerBullet.h"
#include "Application/Object/Enemy/Slime/Slime.h"

void GameScene::Init()
{
	m_tex.Load("Texture/GameScene/bg_game.png");

	m_isDimmingActive = false;
	m_overlayAlpha = 0.0f;

	m_player = new Player();

	m_bulletTex.Load("Texture/GameScene/player_bullet.png");
	for (auto& bullet : m_playerBullets)
	{
		if (!bullet)
		{
			bullet = new PlayerBullet();
			bullet->SetTexture(&m_bulletTex);
		}
	}

	m_slimeTex.Load("Texture/GameScene/slime.png");
	for (auto& slime : m_slime)
	{
		if (!slime)
		{
			slime = new Slime();
			slime->SetTexture(&m_slimeTex);
		}
	}
}

void GameScene::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SCENE.RequestSceneChange(SceneName::Result);
	}

	if (GetAsyncKeyState('1') & 0x8000)
	{
		for (auto& slime : m_slime)
		{
			if (slime && !slime->IsActive())
			{
				slime->SetActive(true);
				Math::Vector2 pos = { 500, 0 };
				slime->SetPosition(pos);
				break;
			}
		}
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		m_player->ChangeHP(1);
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		m_player->ChangeHP(-1);
	}

	m_player->Update();

	if (m_player->GetState() == Player::State::ReadyToShoot ||
		m_player->GetState() == Player::State::Firing ||
		!m_player->IsAlive()) // 追加：死亡時も暗転フラグを立てる
	{
		m_isDimmingActive = true;
	}
	else
	{
		m_isDimmingActive = false;
	}

	if (m_player->IsAlive() && m_player->IsShotRequested())
	{
		for (auto& bullet : m_playerBullets)
		{
			if (bullet && !bullet->IsActive())
			{
				bullet->SetActive(true);
				Math::Vector2 pos = m_player->GetPos();
				bullet->SetPosition(pos);
				bullet->SetAngle(0.0f);
				break;
			}
		}
	}

	if (!m_isDimmingActive)
	{
		for (auto& bullet : m_playerBullets)
		{
			if (bullet && bullet->IsActive())
			{
				bullet->Update(m_player->GetPos());
			}
		}

		for (auto& slime : m_slime)
		{
			if (slime && slime->IsActive())
			{
				slime->Update();
			}
		}
	}
	else if (m_player->IsAlive())
	{
		// 死亡時
	}
	else
	{
		// 必殺技時
	}

	if (m_isDimmingActive) {
		// 「目標の暗さ(0.7)」と「今の暗さに少し足した値」を比べて、
		// 小さい方を採用する（＝0.7を超えないようにする）
		m_overlayAlpha = std::min(m_player->IsAlive() ? 0.7f : 1.0f, m_overlayAlpha + 5.0f * 1.0f / 60.0f);
	}
	else {
		// 「0.0」と「今の暗さから少し引いた値」を比べて、
		// 大きい方を採用する（＝0.0を下回らないようにする）
		m_overlayAlpha = std::max(0.0f, m_overlayAlpha - 5.0f * 1.0f / 60.0f);
	}
}

void GameScene::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 }, 1.0f);

	SHADER.m_spriteShader.DrawBox(0, 0, 1280, 720, &Math::Color(0, 0, 0, m_overlayAlpha));

	for (auto& slime : m_slime)
	{
		if (slime && slime->IsActive())
		{
			slime->Draw();
		}
	}

	for (auto& bullet : m_playerBullets)
	{
		if (bullet && bullet->IsActive())
		{
			bullet->Draw();
		}
	}

	m_player->Draw();
}

void GameScene::Release()
{
	m_tex.Release();
	m_slimeTex.Release();

	if(m_player)
	{
		delete m_player;
		m_player = nullptr;
	}

	for (auto& bullet : m_playerBullets)
	{
		if(bullet)
		{
			delete bullet;
			bullet = nullptr;
		}
	}

	for (auto& slime : m_slime)
	{
		if(slime)
		{
			delete slime;
			slime = nullptr;
		}
	}
}
