#include "GameScene.h"
#include "Application/Scene.h"
#include "Application/Hit.h"
#include "Application/Object/Player/Player.h"
#include "Application/Object/Player/Bullet/PlayerBullet.h"
#include "Application/Object/Enemy/Enemy.h"
#include "Application/Object/Enemy/Slime/Slime.h"
#include "Application/Object/Enemy/SkullWolf/SkullWolf.h"
#include "Application/Object/Enemy/Phoenix/Phoenix.h"
#include "Application/Object/Enemy/Fairy/Fairy.h"
#include "Application/Object/Enemy/FireWorm/FireWorm.h"
#include "Application/Object/Enemy/EnemyBullet.h"
#include "Application/Effect/HitEffect.h"
#include "Application/Effect/TensionUpEffect.h"

void GameScene::Init()
{
	m_BGTex[0].Load("Texture/GameScene/BG00.png");
	m_BGTex[1].Load("Texture/GameScene/BG01.png");
	m_BGTex[2].Load("Texture/GameScene/BG02.png");
	m_BGTex[3].Load("Texture/GameScene/BG03.png");

	m_isDimmingActive = false;
	m_overlayAlpha = 1.0f;

	m_changeFlg = false;
	m_clearFlg = false;

	m_score = 0;

	m_enemySpawnTimer = 0;

	m_player = new Player();

	m_playerBulletTex.Load("Texture/GameScene/player_bullet.png");
	for (auto& bullet : m_playerBullets)
	{
		if (!bullet)
		{
			bullet = new PlayerBullet();
			bullet->SetTexture(&m_playerBulletTex);
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

	m_skullWolfTex.Load("Texture/GameScene/skull_wolf.png");
	for (auto& skullWolf : m_skullWolf)
	{
		if (!skullWolf)
		{
			skullWolf = new SkullWolf();
			skullWolf->SetTexture(&m_skullWolfTex);
		}
	}

	m_phoenixTex.Load("Texture/GameScene/phoenix.png");
	for (auto& phoenix : m_phoenix)
	{
		if (!phoenix)
		{
			phoenix = new Phoenix();
			phoenix->SetTexture(&m_phoenixTex);
		}
	}

	m_fairyTex.Load("Texture/GameScene/fairy.png");
	for (auto& fairy : m_fairy)
	{
		if (!fairy)
		{
			fairy = new Fairy();
			fairy->SetTexture(&m_fairyTex);
		}
	}

	m_fireWormTex.Load("Texture/GameScene/fire_worm.png");
	m_fireWorm = new FireWorm();
	m_fireWorm->SetTexture(&m_fireWormTex);

	for (auto& bullet : m_enemyBullets)
	{
		if (!bullet)
		{
			bullet = new EnemyBullet();
		}
	}

	m_fireTex.Load("Texture/GameScene/fire.png");

	m_numberTex.Load("Texture/GameScene/number.png");

	m_nextSpecialTex.Load("Texture/GameScene/nextSpecialIcon.png");

	m_scoreTex.Load("Texture/GameScene/score.png");
	m_scoreNumberTex.Load("Texture/GameScene/score_number.png");

	m_hitBoxTex.Load("Texture/GameScene/hit_box.png");

	m_resultTex.Load("Texture/GameScene/result.png");
	m_resultAlpha = 0;

	m_resultEntryTex.Load("Texture/GameScene/entry_result.png");
	m_resultEntryAlpha = 0;

	m_hitEffectTex.Load("Texture/GameScene/hit_effect.png");
	for (auto& hitEffect : m_hitEffect)
	{
		if (!hitEffect)
		{
			hitEffect = new HitEffect();
			hitEffect->SetTexture(&m_hitEffectTex);
		}
	}

	m_tensionUpEffectTex.Load("Texture/GameScene/tension_up_effect.png");
	m_tensionUpEffect = new TensionUpEffect;
	m_tensionUpEffect->SetTexture(&m_tensionUpEffectTex);
}

void GameScene::Update()
{
	if ((GetAsyncKeyState(VK_RETURN) & 0x8000)) SCENE.RequestSceneChange(SceneName::Result);

	if (m_score < SCENE.GetScore()) m_score += 1;

	if (m_clearFlg && !m_fireWorm->IsActive())
	{
		m_isDimmingActive = true;

		if (m_overlayAlpha == 0.8f)
		{
			if (m_resultAlpha < 1) m_resultAlpha += 0.02f;
			if (m_resultEntryAlpha < 1) m_resultEntryAlpha += 0.01f;

			if (!m_changeFlg)
			{
				if (!(GetAsyncKeyState('Z') & 0x8000)) m_changeFlg = true;
			}
			else
			{
				if ((GetAsyncKeyState('Z') & 0x8000))
				{
					SCENE.SetHP(m_player->GetHp());
					SCENE.RequestSceneChange(SceneName::Result);
				}
			}
		}
	}
	else
	{
		if (!m_player->IsAlive() && m_overlayAlpha == 1.0f)
		{
			if (m_resultAlpha < 1) m_resultAlpha += 0.02f;
			if (m_resultEntryAlpha < 1) m_resultEntryAlpha += 0.01f;

			if (!m_changeFlg)
			{
				if (!(GetAsyncKeyState('Z') & 0x8000)) m_changeFlg = true;
			}
			else
			{
				if ((GetAsyncKeyState('Z') & 0x8000))
				{
					SCENE.SetHP(m_player->GetHp());
					SCENE.RequestSceneChange(SceneName::Result);
				}
			}
		}

		if (GetAsyncKeyState('1') & 0x8000)
		{
			m_player->ChangeHP(1);
		}

		if (GetAsyncKeyState('2') & 0x8000)
		{
			m_player->ChangeHP(-1);
		}

		if (GetAsyncKeyState('0') & 0x8000)
		{
			m_enemySpawnTimer = 6000;
		}

		m_player->Update();

		if (m_player->GetTensionUpFlg())
		{
			m_player->SetTensionUpFlg(false);
			m_tensionUpEffect->Init();
			m_tensionUpEffect->SetActive(true);
			m_tensionUpEffect->SetPosition(m_player->GetPos());
		}

		HitCheck();

		if (m_player->GetState() == Player::State::ReadyToShoot ||
			m_player->GetState() == Player::State::Firing ||
			!m_player->IsAlive())
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
					Math::Vector2 pos = m_player->GetPos() + Math::Vector2{ 0.0f,12.0f - 12.0f };
					bullet->SetPosition(pos);
					bullet->SetAngle(0.0f);
					break;
				}
			}
		}

		if (m_fireWorm && m_fireWorm->IsActive() && !m_fireWorm->IsAlive())
		{
			m_clearFlg = true;
		}
	}

	if (!m_isDimmingActive)
	{
		SpawnEnemy();

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

		for (auto& skullWolf : m_skullWolf)
		{
			if (skullWolf && skullWolf->IsActive())
			{
				skullWolf->Update();
			}
		}

		for (auto& phoenix : m_phoenix)
		{
			if (phoenix && phoenix->IsActive())
			{
				phoenix->Update();
			}
		}

		for (auto& fairy : m_fairy)
		{
			if (fairy && fairy->IsActive())
			{
				fairy->Update(m_player->GetPos());
			}
		}

		if (m_fireWorm && m_fireWorm->IsActive())
		{
			m_fireWorm->Update(m_player->GetPos());
		}

		for (auto& bullet : m_enemyBullets)
		{
			if (bullet && bullet->IsActive())
			{
				bullet->Update(m_player->GetPos());
			}
		}

		for (auto& hitEffect : m_hitEffect)
		{
			if (hitEffect && hitEffect->IsActive())
			{
				hitEffect->Update();
			}
		}

		if (m_tensionUpEffect && m_tensionUpEffect->IsActive())
		{
			m_tensionUpEffect->Update(m_player->GetPos());
		}
	}
	else if (!m_player->IsAlive())
	{
		// 死亡時
	}
	else
	{
		if (m_player->GetState() == Player::State::Firing)
		{
			for (auto bullet : m_enemyBullets)
			{
				if (bullet && bullet->IsActive())
				{
					bullet->SetActive(false);
				}
			}
		}
	}

	if (m_isDimmingActive) {
		// 「目標の暗さ(0.7)」と「今の暗さに少し足した値」を比べて、
		// 小さい方を採用する（＝0.7を超えないようにする）
		if (m_clearFlg)
		{
			m_overlayAlpha = std::min(0.8f, m_overlayAlpha + 5.0f * 0.2f / 60.0f);
		}
		else
		{
			m_overlayAlpha = std::min(m_player->IsAlive() ? 0.7f : 1.0f, m_overlayAlpha + 5.0f * 1.0f / 60.0f);
		}
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

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1.0f / 3.0f));
	SHADER.m_spriteShader.DrawTex(&m_BGTex[3], Math::Rectangle{0,0,3840,2160}, 1.0f);

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1.0f / 3.0f));
	SHADER.m_spriteShader.DrawTex(&m_BGTex[2], Math::Rectangle{0,0,3840,2160}, 1.0f);

	for (auto& fairy : m_fairy)
	{
		if (fairy && fairy->IsActive())
		{
			fairy->Draw();
		}
	}

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1.0f / 3.0f));
	SHADER.m_spriteShader.DrawTex(&m_BGTex[1], Math::Rectangle{0,0,3840,2160}, 1.0f);

	for (auto& bullet : m_playerBullets)
	{
		if (bullet && bullet->IsActive())
		{
			bullet->Draw();
		}
	}

	for (auto& bullet : m_enemyBullets)
	{
		if (bullet && bullet->IsActive())
		{
			bullet->Draw();
		}
	}

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.DrawBox(0, 0, 1280, 720, &Math::Color(0, 0, 0, m_overlayAlpha));

	if (m_enemySpawnTimer > 6000 && m_enemySpawnTimer < 6240)
	{
		float alpha = sinf((m_enemySpawnTimer - 6000.0f) / 10.0f) * 0.2f;
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawBox(0, 0, 1280, 720, &Math::Color(1, 0, 0, alpha));
	}
//	SHADER.m_spriteShader.DrawBox(500, -90, 20, 200, &Math::Color(1, 0, 0, 1.0f));

	for (auto& slime : m_slime)
	{
		if (slime && slime->IsActive())
		{
			slime->Draw();
		}
	}

	for (auto& skullWolf : m_skullWolf)
	{
		if (skullWolf && skullWolf->IsActive())
		{
			skullWolf->Draw();
		}
	}

	for (auto& phoenix : m_phoenix)
	{
		if (phoenix && phoenix->IsActive())
		{
			phoenix->Draw();
		}
	}

	if (m_fireWorm && m_fireWorm->IsActive())
	{
		m_fireWorm->Draw();
	}

	m_player->Draw();

	for (auto& hitEffect : m_hitEffect)
	{
		if (hitEffect && hitEffect->IsActive())
		{
			hitEffect->Draw();
		}
	}

	if (m_tensionUpEffect && m_tensionUpEffect->IsActive())
	{
		m_tensionUpEffect->Draw();
	}

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(1.0f / 3.0f));
	SHADER.m_spriteShader.DrawTex(&m_BGTex[0], Math::Rectangle{ 0,0,3840,2160 }, 1.0f - m_overlayAlpha);

/*	{
		float baseX = 580.0f;  // 右端(640)から少し左
		float baseY = -324.0f + 100; // 下端(-360)から少し上

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
			float fireY = baseY + gaugeMaxHalfHeight * 2 + 65.0f;

			float alpha = 0.6f;

			float scale = 0.3f;

			if (m_player->GetTensionPercent() >= 1.0f && !m_isDimmingActive)
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

			float numberY = baseY + gaugeMaxHalfHeight * 2 + 40.0f;

			Math::Matrix numberMat =
				Math::Matrix::CreateScale(scale * 15.0f) * Math::Matrix::CreateTranslation(baseX, numberY, 0);
			SHADER.m_spriteShader.SetMatrix(numberMat);
			SHADER.m_spriteShader.DrawTex(&m_numberTex, Math::Rectangle{ m_player->GetHp() * 13, 0, 13, 13}, alpha);
		}
		{
			Math::Matrix nextSpecialMat =
				Math::Matrix::CreateScale(2.0f) * Math::Matrix::CreateTranslation(baseX, baseY - 60, 0);
			SHADER.m_spriteShader.SetMatrix(nextSpecialMat);
			SHADER.m_spriteShader.DrawTex(&m_nextSpecialTex, Math::Rectangle{ m_player->GetSpecialBulletType() * 48, 0, 48, 48 }, 1.0f);
		}
	}*/

	{
		// --- 1. 基準位置を「左上」に設定 ---
		float baseX = -580.0f;        // 左端(炎と数字のX座標)
		float baseY = 260.0f;         // 上端から少し下

		float gaugeHeight = 24.0f;    // ゲージの太さ
		float gaugeMaxHalfWidth = 150.0f; // ゲージの長さの半分（合計300px）

		// ゲージ自体の開始X座標（炎アイコンの少し右からスタート）
		float gaugeStartX = baseX + 50.0f;

		{
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			SHADER.m_spriteShader.DrawBox(-360, baseY + 20, 270, 85, &Math::Color{ 0,0,0,0.5f });
		}

		{
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			SHADER.m_spriteShader.DrawBox(380, baseY + 63, 280, 35, &Math::Color{ 0,0,0,0.5f });
		}

		// --- 3. 背景（一番下の黒い枠） ---
		{
			Math::Color baseColor(0.1f, 0.1f, 0.1f, 0.8f);
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			float bgCenterX = gaugeStartX + gaugeMaxHalfWidth;
			SHADER.m_spriteShader.DrawBox(bgCenterX, baseY, gaugeMaxHalfWidth + 5, gaugeHeight + 5, &baseColor, true);
		}

		// --- 4. ゲージの土台（暗い黄色） ---
		{
			Math::Color bgColor(0.4f, 0.4f, 0.2f, 0.5f);
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			float bgCenterX = gaugeStartX + gaugeMaxHalfWidth;
			SHADER.m_spriteShader.DrawBox(bgCenterX, baseY, gaugeMaxHalfWidth, gaugeHeight, &bgColor, true);
		}

		// --- 5. テンションゲージ本体（左から右へ伸びる） ---
		{
			float tensionRate = m_player->GetTensionPercent();
			float currentFullWidth = gaugeMaxHalfWidth * 2.0f * tensionRate;

			// 起点(gaugeStartX)から、現在の長さの半分だけ「右(+)」に進んだ位置がDrawBoxの中心点
			float gaugeCenterX = gaugeStartX + (currentFullWidth * 0.5f);

			Math::Color gaugeColor = (tensionRate >= 1.0f) ?
				Math::Color(1.0f, 0.2f, 0.1f, 0.8f) : // 満タンで赤
				Math::Color(1.0f, 0.7f, 0.2f, 1.0f);  // 溜まるまでオレンジ

			SHADER.m_spriteShader.DrawBox(gaugeCenterX, baseY, (int)(currentFullWidth * 0.5f), gaugeHeight, &gaugeColor, true);
		}
		// --- 2. 炎アイコンとHP数値（ゲージの左端：起点） ---
		{
			float fireX = baseX + 10;
			float iconY = baseY + 20;

			float alpha = 0.6f;
			float scale = 0.3f;

			if ((m_player->GetTensionPercent() >= 1.0f && !m_isDimmingActive) ||
				(m_player->GetHp() >= 9 && !m_isDimmingActive))
			{
				alpha = 1.0f;
				scale = 0.3f + sinf(SCENE.GetFrameCount() * 15.0f) * 0.02f;
			}

			float offset = 2.0f;
			float dx[] = { offset, -offset, 0, 0,  offset, -offset,  offset, -offset };
			float dy[] = { 0, 0, offset, -offset,  offset,  offset, -offset, -offset };

			for (int i = 0; i < 8; i++) {
				Math::Matrix edgeMat =
					Math::Matrix::CreateScale(scale) * Math::Matrix::CreateTranslation(fireX + dx[i], iconY + dy[i], 0);
				SHADER.m_spriteShader.SetMatrix(edgeMat);
				SHADER.m_spriteShader.DrawTexBlack(&m_fireTex, Math::Rectangle{ 0,0,316,508 }, alpha * 0.35f);
			}

			Math::Matrix fireMat =
				Math::Matrix::CreateScale(scale) * Math::Matrix::CreateTranslation(fireX, iconY, 0);
			SHADER.m_spriteShader.SetMatrix(fireMat);
			SHADER.m_spriteShader.DrawTex(&m_fireTex, Math::Rectangle{ 0, 0, 316, 508 }, alpha);

			Math::Matrix numberMat =
				Math::Matrix::CreateScale(scale * 15.0f) * Math::Matrix::CreateTranslation(fireX, iconY - 25.0f, 0);
			SHADER.m_spriteShader.SetMatrix(numberMat);
			SHADER.m_spriteShader.DrawTex(&m_numberTex, Math::Rectangle{ m_player->GetHp() * 13, 0, 13, 13 }, alpha);
		}
		// --- 6. 次の必殺技アイコン（ゲージの右端：終着点） ---
		{
			// ゲージの開始位置からゲージの最大幅分進み、さらに少し右に置く
			float nextSpecialX = gaugeStartX + (gaugeMaxHalfWidth * 2.0f) + 50.0f + 20.0f;

			Math::Matrix nextSpecialMat =
				Math::Matrix::CreateScale(2.0f) * Math::Matrix::CreateTranslation(nextSpecialX, baseY, 0);
			SHADER.m_spriteShader.SetMatrix(nextSpecialMat);
			SHADER.m_spriteShader.DrawTex(&m_nextSpecialTex, Math::Rectangle{ m_player->GetSpecialBulletType() * 48, 0, 48, 48 }, 1.0f);
		}
	}
	{
		Math::Matrix mat =
			Math::Matrix::CreateScale(2.5f, 4.0f, 1.0f) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(190, 260 + 60, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_scoreTex, Math::Rectangle{ 0,0,65,13 }, 1.0f);

		unsigned int tmp = m_score * 100;
		int m_digits[10];
		for (int i = 10 - 1; i >= 0; --i)
		{
			// 下位の桁から抽出し配列に格納
			m_digits[i] = tmp % 10;	// 余りを求める演算子 %
			tmp /= 10;

			Math::Matrix mat =
				Math::Matrix::CreateScale(2.5f, 4.0f, 1.0f) *
				Math::Matrix::CreateRotationZ(0) *
				Math::Matrix::CreateTranslation(320 + i * 13 * 2.5f, 260 + 60, 0);
			SHADER.m_spriteShader.SetMatrix(mat);
			SHADER.m_spriteShader.DrawTex(&m_scoreNumberTex, Math::Rectangle{ m_digits[i] * 13,0,13,13 }, 1.0f);
		}
	}

	if(m_clearFlg && !m_fireWorm->IsActive())
	{
		Math::Rectangle rc = { 0, 0, 90, 13 };
		Math::Matrix mat =
			Math::Matrix::CreateScale(6.0f) *
			Math::Matrix::CreateRotationZ(0)*
			Math::Matrix::CreateTranslation(0, 100, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_resultTex, rc, m_resultAlpha);
		
		rc = { 0, 0, 112, 26 };
		m_resultEntryScale = 4.0f + sinf(SCENE.GetFrameCount() * 0.02f) * 0.2f;
		mat =
			Math::Matrix::CreateScale(m_resultEntryScale) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(0, -220, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_resultEntryTex, rc, m_resultAlpha);
	}

	if(!m_player->IsAlive() && m_overlayAlpha == 1.0f)
	{
		Math::Rectangle rc = { 0, 13, 90, 13 };
		Math::Matrix mat =
			Math::Matrix::CreateScale(6.0f) *
			Math::Matrix::CreateRotationZ(0)*
			Math::Matrix::CreateTranslation(0, 100, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_resultTex, rc, m_resultAlpha);

		rc = { 0, 0, 112, 26 };
		m_resultEntryScale = 4.0f + sinf(SCENE.GetFrameCount() * 0.02f) * 0.2f;
		mat =
			Math::Matrix::CreateScale(m_resultEntryScale) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(0, -220, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_resultEntryTex, rc, m_resultAlpha);
	}

	/*for(auto& enemy : m_phoenix)
	{
		if (enemy->IsActive()) 
		{
			SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
			SHADER.m_spriteShader.DrawCircle(enemy->GetPos().x, enemy->GetPos().y, 24, &Math::Color(1, 0, 0, 1.0f));
		}
	}*/

	
	if (m_player->IsAlive() && !m_isDimmingActive)
	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		Math::Matrix mat =
			Math::Matrix::CreateScale(0.5f) *
			Math::Matrix::CreateTranslation(m_player->GetPos().x, m_player->GetPos().y, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_hitBoxTex, Math::Rectangle{ 0,0,32,32 }, 0.2f);
	}
	
}

void GameScene::Release()
{
	m_BGTex[0].Release();
	m_BGTex[1].Release();
	m_BGTex[2].Release();
	m_BGTex[3].Release();

	if(m_player)
	{
		delete m_player;
		m_player = nullptr;
	}

	m_playerBulletTex.Release();
	for (auto& bullet : m_playerBullets)
	{
		if(bullet)
		{
			delete bullet;
			bullet = nullptr;
		}
	}

	m_slimeTex.Release();
	for (auto& slime : m_slime)
	{
		if(slime)
		{
			delete slime;
			slime = nullptr;
		}
	}

	m_skullWolfTex.Release();
	for (auto& skullWolf : m_skullWolf)
	{
		if(skullWolf)
		{
			delete skullWolf;
			skullWolf = nullptr;
		}
	}

	m_phoenixTex.Release();
	for (auto& phoenix : m_phoenix)
	{
		if(phoenix)
		{
			delete phoenix;
			phoenix = nullptr;
		}
	}

	m_fairyTex.Release();
	for (auto& fairy : m_fairy)
	{
		if(fairy)
		{
			delete fairy;
			fairy = nullptr;
		}
	}

	m_fireWormTex.Release();
	delete m_fireWorm;
	m_fireWorm = nullptr;

	for (auto& bullet : m_enemyBullets)
	{
		if (bullet)
		{
			delete bullet;
			bullet = nullptr;
		}
	}

	m_fireTex.Release();
	m_numberTex.Release();
	m_nextSpecialTex.Release();

	m_scoreTex.Release();
	m_scoreNumberTex.Release();

	m_hitBoxTex.Release();

	m_resultTex.Release();
	m_resultEntryTex.Release();

	m_hitEffectTex.Release();
	for (auto& hitEffect : m_hitEffect)
	{
		if (hitEffect)
		{
			delete hitEffect;
			hitEffect = nullptr;
		}
	}

	m_tensionUpEffectTex.Release();
	if (m_tensionUpEffect)
	{
		delete m_tensionUpEffect;
		m_tensionUpEffect = nullptr;
	}
}

void GameScene::HitCheck()
{
	// ========== 敵と自機 ==========

	if (!m_player->IsInvincible())
	{
		for (auto& slime : m_slime)
		{
			if (!slime->IsActive())continue;
			if (!slime->IsAlive())continue;

			if (m_hit->CheckCircle(slime->GetPos(), 16.0f, m_player->GetPos(), 8))
			{
				m_player->ChangeHP(-1);
				m_player->SetDamageTimer(55);
			}
		}
		for (auto& skullWolf : m_skullWolf)
		{
			if (!skullWolf->IsActive())continue;
			if (!skullWolf->IsAlive())continue;

			if (m_hit->CheckCircle(skullWolf->GetPos() + Math::Vector2{ 0,-24 }, 24.0f, m_player->GetPos(), 8))
			{
				m_player->ChangeHP(-1);
				m_player->SetDamageTimer(55);
			}
		}
		for (auto& phoenix : m_phoenix)
		{
			if (!phoenix->IsActive())continue;
			if (!phoenix->IsAlive())continue;

			if (m_hit->CheckCircle(phoenix->GetPos(), 24.0f, m_player->GetPos(), 8))
			{
				m_player->ChangeHP(-1);
				m_player->SetDamageTimer(55);
			}
		}
	}

	// ========== 敵の弾 ==========

	{
		if (!m_player->IsInvincible())
		{
			for (auto& bullet : m_enemyBullets)
			{
				if (!bullet->IsActive())continue;
				if (m_hit->CheckCircle(bullet->GetPos(), 16.0f * bullet->GetSize(), m_player->GetPos(), 8))
				{
					m_player->ChangeHP(-1);
					m_player->SetDamageTimer(55);
					bullet->SetActive(false);
				}
			}
		}
	}

	// ========== 通常弾 ==========

	{
		for (auto& bullet : m_playerBullets)
		{
			if (!bullet->IsActive())continue;
			for (auto& slime : m_slime)
			{
				if (!slime->IsActive())continue;
				if (!slime->IsAlive())continue;

				if (m_hit->CheckRect(bullet->GetPos(), 32, 10,
					slime->GetPos(), 64, 24))
				{
					slime->ChangeHP(-1);
					m_player->AddTension(1);
					SCENE.AddScore(1);
					CreateHitEffect(bullet->GetPos(), 1.0f);
					bullet->SetActive(false);
					if (!slime->IsAlive())
					{
						m_player->AddTension(5);
					}
				}
			}
			for (auto& skullWolf : m_skullWolf)
			{
				if (!skullWolf->IsActive())continue;
				if (!skullWolf->IsAlive())continue;

				if (m_hit->CheckRect(bullet->GetPos(), 32, 10,
					skullWolf->GetPos() + Math::Vector2{ 0,-24 }, 96, 48))
				{
					skullWolf->ChangeHP(-1);
					m_player->AddTension(1);
					SCENE.AddScore(1);
					CreateHitEffect(bullet->GetPos(), 1.0f);
					bullet->SetActive(false);
					if (!skullWolf->IsAlive())
					{
						m_player->AddTension(7);
					}
				}
			}
			for (auto& phoenix : m_phoenix)
			{
				if (!phoenix->IsActive())continue;
				if (!phoenix->IsAlive())continue;

				if (m_hit->CheckRect(bullet->GetPos(), 32, 10,
					phoenix->GetPos(), 48, 48))
				{
					phoenix->ChangeHP(-1);
					m_player->AddTension(1);
					SCENE.AddScore(1);
					CreateHitEffect(bullet->GetPos(), 1.0f);
					bullet->SetActive(false);
					if (!phoenix->IsAlive())
					{
						m_player->AddTension(10);
					}
				}
			}
			{
				if (!m_fireWorm->IsActive())continue;
				if (!m_fireWorm->IsAlive())continue;

				if (m_hit->CheckRect(bullet->GetPos(), 32, 10,
					m_fireWorm->GetPos(), 240-20, 160))
				{
					m_fireWorm->ChangeHP(-1);
					m_player->AddTension(2);
					SCENE.AddScore(1);
					CreateHitEffect(bullet->GetPos(), 1.0f);
					bullet->SetActive(false);
					if (!m_fireWorm->IsAlive())
					{

					}
				}
			}
		}
	}

	// ========== 必殺弾 ==========

	{
		auto** sBullets = m_player->GetSpecialBullets();
		for (int i = 0; i < m_player->GetSpecialBulletsMax(); ++i)
		{
			auto* bullet = sBullets[i];
			if (!bullet->IsActive()) continue;

			for (auto& slime : m_slime)
			{
				if (!slime->IsActive())continue;
				if (!slime->IsAlive())continue;
				if (bullet->GetBulletType() == PlayerBullet::BulletType::Parabola)
				{
					if (!bullet->IsHitActive())continue;
					if (m_hit->CheckCircle(bullet->GetPos(), 32, slime->GetPos(), 16))
					{
						slime->ChangeHP(-100);
						bullet->IncrementKillCount();
						SCENE.AddScore(10);
						CreateHitEffect(bullet->GetPos(), 2.0f);
					}
				}
				else
				{
					if (m_hit->CheckRect(bullet->GetPos(), 32, 10,
						slime->GetPos(), 64, 24))
					{
						slime->ChangeHP(-100);
						bullet->IncrementKillCount();
						SCENE.AddScore(10);
						CreateHitEffect(bullet->GetPos(), 2.0f);
					}
				}
			}
			for (auto& skullWolf : m_skullWolf)
			{
				if (!skullWolf->IsActive())continue;
				if (!skullWolf->IsAlive())continue;
				if (bullet->GetBulletType() == PlayerBullet::BulletType::Parabola)
				{
					if (!bullet->IsHitActive())continue;
					if (m_hit->CheckCircle(bullet->GetPos(), 32, skullWolf->GetPos() + Math::Vector2{ 0,-24 }, 24))
					{
						skullWolf->ChangeHP(-100);
						bullet->IncrementKillCount();
						SCENE.AddScore(10);
						CreateHitEffect(bullet->GetPos(), 2.0f);
					}
				}
				else
				{
					if (m_hit->CheckRect(bullet->GetPos(), 32, 10,
						skullWolf->GetPos() + Math::Vector2{ 0,-24 }, 96, 48))
					{
						skullWolf->ChangeHP(-100);
						bullet->IncrementKillCount();
						SCENE.AddScore(10);
						CreateHitEffect(bullet->GetPos(), 2.0f);
					}
				}
			}
			for (auto& phoenix : m_phoenix)
			{
				if (!phoenix->IsActive())continue;
				if (!phoenix->IsAlive())continue;
				if (bullet->GetBulletType() == PlayerBullet::BulletType::Parabola)
				{
					if (!bullet->IsHitActive())continue;
					if (m_hit->CheckCircle(bullet->GetPos(), 32, phoenix->GetPos(), 24))
					{
						phoenix->ChangeHP(-100);
						bullet->IncrementKillCount();
						SCENE.AddScore(10);
						CreateHitEffect(bullet->GetPos(), 2.0f);
					}
				}
				else
				{
					if (m_hit->CheckRect(bullet->GetPos(), 32, 10,
						phoenix->GetPos(), 48, 48))
					{
						phoenix->ChangeHP(-100);
						bullet->IncrementKillCount();
						SCENE.AddScore(10);
						CreateHitEffect(bullet->GetPos(), 2.0f);
					}
				}
			}
			if (!m_fireWorm->IsActive())continue;
			if (!m_fireWorm->IsAlive())continue;
			if (bullet->GetBulletType() == PlayerBullet::BulletType::Parabola)
			{
				if (!bullet->IsHitActive())continue;
				if (m_hit->CheckCircle(bullet->GetPos(), 32, m_fireWorm->GetPos(), 120))
				{
					m_fireWorm->ChangeHP(-50);
					bullet->IncrementKillCount();
					SCENE.AddScore(50);
					CreateHitEffect(bullet->GetPos(), 3.0f);
				}
			}
			else
			{
				if (m_hit->CheckRect(bullet->GetPos(), 32, 10,
					m_fireWorm->GetPos(), 240, 160))
				{
					m_fireWorm->ChangeHP(-5);
					sBullets[0]->IncrementKillCount();
					sBullets[1]->IncrementKillCount();
					SCENE.AddScore(5);
					CreateHitEffect(bullet->GetPos(), 1.5f);
				}
			}
		}
	}
}

void GameScene::SpawnEnemy()
{
	m_enemySpawnTimer++;
	
	if (m_enemySpawnTimer < 6000)
	{
		if (!(m_enemySpawnTimer % 120))
		{
			float posY = (float)(rand() % 400 - 200) - 90.0f;
			for (auto& slime : m_slime)
			{
				if (slime && !slime->IsActive())
				{
					slime->Init();
					slime->SetActive(true);
					slime->SetAlive(true);
					slime->SetHP(2);
					Math::Vector2 pos = { 700, posY };
					slime->SetPosition(pos);
					slime->SetEnemyBulletPool(m_enemyBullets, MAX_ENEMY_BULLETS);
					break;
				}
			}
		}
		if (!(m_enemySpawnTimer % 300))
		{
			if (m_enemySpawnTimer > 300)
			{
				float posY = (float)(rand() % 400 - 200) - 90.0f + 24.0f;
				for (auto& skullWolf : m_skullWolf)
				{
					if (skullWolf && !skullWolf->IsActive())
					{
						skullWolf->Init();
						skullWolf->SetActive(true);
						skullWolf->SetAlive(true);
						skullWolf->SetHP(3);
						Math::Vector2 pos = { 700, posY };
						skullWolf->SetPosition(pos);
						skullWolf->SetEnemyBulletPool(m_enemyBullets, MAX_ENEMY_BULLETS);
						break;
					}
				}
			}
		}
		if (!(m_enemySpawnTimer % 480))
		{
			if (m_enemySpawnTimer > 480)
			{
				float posY = (float)(rand() % 360 - 180) - 90.0f;
				for (auto& phoenix : m_phoenix)
				{
					if (phoenix && !phoenix->IsActive())
					{
						phoenix->Init();
						phoenix->SetActive(true);
						phoenix->SetAlive(true);
						phoenix->SetHP(4);
						Math::Vector2 pos = { 700, posY };
						phoenix->SetPosition(pos);
						phoenix->SetEnemyBulletPool(m_enemyBullets, MAX_ENEMY_BULLETS);
						break;
					}
				}
			}
		}
		if (!((m_enemySpawnTimer % 900)))
		{
			float posY = 260;
			for (auto& fairy : m_fairy)
			{
				if (fairy && !fairy->IsActive())
				{
					fairy->Init();
					fairy->SetActive(true);
					fairy->SetAlive(true);
					fairy->SetHP(1);
					Math::Vector2 pos = { 700, posY };
					fairy->SetPosition(pos);
					fairy->SetEnemyBulletPool(m_enemyBullets, MAX_ENEMY_BULLETS);
					break;
				}
			}
		}
	}
	else
	{
		if (m_fireWorm && !m_fireWorm->IsActive() && !m_fireWorm->IsAlive())
		{
			m_fireWorm->Init();
			m_fireWorm->SetActive(true);
			m_fireWorm->SetAlive(true);
			m_fireWorm->SetHP(350);
			Math::Vector2 pos = { 850, -50 };
			m_fireWorm->SetPosition(pos);
			m_fireWorm->SetEnemyBulletPool(m_enemyBullets, MAX_ENEMY_BULLETS);
		}
	}
}

void GameScene::CreateHitEffect(Math::Vector2& _pos, float _size)
{
	for (auto& hitEffect : m_hitEffect)
	{
		if (hitEffect && !hitEffect->IsActive())
		{
			Math::Vector2 pos = _pos + Math::Vector2{ 16, 0 };
			hitEffect->Init();
			hitEffect->SetActive(true);
			hitEffect->SetPosition(pos);
			hitEffect->SetSize(_size);
			break;
		}
	}
}
