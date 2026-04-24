#include "GameScene.h"
#include "Application/Scene.h"
#include "Application/Object/Player/Player.h"
#include "Application/Object/Enemy/Slime/Slime.h"

void GameScene::Init()
{
	m_tex.Load("Texture/GameScene/bg_game.png");

	m_isDimmingActive = false;
	m_overlayAlpha = 0.0f;

	m_player = new Player();

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
			if (!slime->IsActive())
			{
				slime->SetActive(true);
				Math::Vector2 pos = { 500, 0 };
				slime->SetPosition(pos);
				break;
			}
		}
	}

	if (Player::State::ReadyToShoot == m_player->GetState() || Player::State::Firing == m_player->GetState())
	{
		m_isDimmingActive = true;
	}
	else
	{
		m_isDimmingActive = false;
	}

	if (m_isDimmingActive) {
		// 「目標の暗さ(0.7)」と「今の暗さに少し足した値」を比べて、
		// 小さい方を採用する（＝0.7を超えないようにする）
		m_overlayAlpha = std::min(0.7f, m_overlayAlpha + 5.0f * 1.0f / 60.0f);
	}
	else {
		// 「0.0」と「今の暗さから少し引いた値」を比べて、
		// 大きい方を採用する（＝0.0を下回らないようにする）
		m_overlayAlpha = std::max(0.0f, m_overlayAlpha - 5.0f * 1.0f / 60.0f);
	}

	m_player->Update();

	for (auto& slime : m_slime)
	{
		if (slime->IsActive())
		{ 
			slime->Update();
		}
	}
}

void GameScene::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 }, 1.0f);

	SHADER.m_spriteShader.DrawBox(0, 0, 1280, 720, &Math::Color(0, 0, 0, m_overlayAlpha));

	for (auto& slime : m_slime)
	{
		if (slime->IsActive())
		{
			slime->Draw();
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

	for (auto& slime : m_slime)
	{
		if(slime)
		{
			delete slime;
			slime = nullptr;
		}
	}
}
