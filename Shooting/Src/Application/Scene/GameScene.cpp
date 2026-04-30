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

	m_fireTex.Load("Texture/GameScene/fire.png");

	m_numberTex.Load("Texture/GameScene/number.png");
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
				Math::Vector2 pos = m_player->GetPos() + Math::Vector2{ -24.0f,12.0f };
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

	for (auto& bullet : m_playerBullets)
	{
		if (bullet && bullet->IsActive())
		{
			bullet->Draw();
		}
	}

	SHADER.m_spriteShader.DrawBox(0, 0, 1280, 720, &Math::Color(0, 0, 0, m_overlayAlpha));

	for (auto& slime : m_slime)
	{
		if (slime && slime->IsActive())
		{
			slime->Draw();
		}
	}

	m_player->Draw();

	{
		float baseX = 580.0f;  // 右端(640)から少し左
		float baseY = -324.0f; // 下端(-360)から少し上

		float gaugeWidth = 24.0f;
		float gaugeMaxHalfHeight = 150.0f;
		{
			Math::Color baseColor(0.1f, 0.1f, 0.1f, 0.8f);
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			float bgCenterY = baseY + gaugeMaxHalfHeight;
			SHADER.m_spriteShader.DrawBox(baseX, bgCenterY, gaugeWidth + 5, gaugeMaxHalfHeight + 5, &baseColor, true);
		}
		{
			Math::Color bgColor(0.4f, 0.4f, 0.2f, 0.5f);
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			float bgCenterY = baseY + gaugeMaxHalfHeight;
			// 縦長に描画（中心点に注意して座標を計算）
			SHADER.m_spriteShader.DrawBox(baseX, bgCenterY, gaugeWidth, gaugeMaxHalfHeight, &bgColor, true);
		}
		{
			float tensionRate = m_player->GetTensionPercent();
			float currentHeight = gaugeMaxHalfHeight * tensionRate;

			// 下から上に伸びるように計算
			// 現在の高さの半分だけ基準点から上げる
			float gaugeCenterY = baseY + currentHeight;

			Math::Color gaugeColor = (tensionRate >= 1.0f) ?
				Math::Color(1.0f, 0.2f, 0.1f, 0.8f) : // 満タンで燃えるような赤
				Math::Color(1.0f, 0.7f, 0.2f, 1.0f);  // 溜まるまではオレンジ

			SHADER.m_spriteShader.DrawBox(baseX, gaugeCenterY, gaugeWidth, (int)currentHeight, &gaugeColor, true);
		}
		{
			float fireY = baseY + gaugeMaxHalfHeight * 2 + 40.0f;

			float alpha = 0.6f;

			float scale = 0.3f;
			if (m_player->GetTensionPercent() >= 1.0f)
			{
				alpha = 1.0f;
				scale = 0.3f + sinf(SCENE.GetFrameCount() * 15.0f) * 0.02f;
			}

			float offset = 2.0f; // フチの太さ

			float dx[] = { offset, -offset, 0, 0,  offset, -offset,  offset, -offset };
			float dy[] = { 0, 0, offset, -offset,  offset,  offset, -offset, -offset };

			for (int i = 0; i < 8; i++) {
				Math::Matrix edgeMat =
					Math::Matrix::CreateScale(scale) * Math::Matrix::CreateTranslation(baseX + dx[i], fireY + dy[i], 0);
				SHADER.m_spriteShader.SetMatrix(edgeMat);
				SHADER.m_spriteShader.DrawTexBlack(&m_fireTex, Math::Rectangle{ 0,0,316,508 }, alpha * 0.35f);
			}

			Math::Matrix fireMat =
				Math::Matrix::CreateScale(scale) * Math::Matrix::CreateTranslation(baseX, fireY, 0);

			SHADER.m_spriteShader.SetMatrix(fireMat);
			SHADER.m_spriteShader.DrawTex(&m_fireTex, Math::Rectangle{ 0, 0, 316, 508 }, alpha);

			float numberY = baseY + gaugeMaxHalfHeight * 2 + 15.0f;

			Math::Matrix numberMat =
				Math::Matrix::CreateScale(scale * 15.0f) * Math::Matrix::CreateTranslation(baseX, numberY, 0);
			SHADER.m_spriteShader.SetMatrix(numberMat);
			SHADER.m_spriteShader.DrawTex(&m_numberTex, Math::Rectangle{ m_player->GetHp() * 13, 0, 13, 13}, alpha);
		}
	}
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

	m_fireTex.Release();

	m_numberTex.Release();
}
