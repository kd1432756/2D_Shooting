#include "ResultScene.h"
#include"Application/Scene.h"

void ResultScene::Init()
{
	m_tex.Load("Texture/ResultScene/bg_result.png");

	m_changeFlg = false;
	m_timer = 0;

	m_hp = 0;
	m_score = 0;

	m_resultTex.Load("Texture/ResultScene/result.png");
	m_scoreTex.Load("Texture/ResultScene/score.png");
	m_numberTex.Load("Texture/ResultScene/number.png");
	m_fireTex.Load("Texture/ResultScene/fire.png");
	m_playerTex.Load("Texture/ResultScene/player.png");
	m_titleEntryTex.Load("Texture/ResultScene/entry_title.png");

	m_hp = SCENE.GetHP();
	m_score = SCENE.GetScore() * 100;
}

void ResultScene::Update()
{
	m_timer++;

	if (m_timer > 220)
	{
		if (!m_changeFlg)
		{
			if (!(GetAsyncKeyState('Z') & 0x8000)) m_changeFlg = true;
		}
		else
		{
			if ((GetAsyncKeyState('Z') & 0x8000)) SCENE.RequestSceneChange(SceneName::Title);
		}
	}
}

void ResultScene::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateScale(0.6f));
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,2304,1296 }, 1.0f);

	{
		SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
		SHADER.m_spriteShader.DrawBox(0, 0, 640, 360, &Math::Color{ 0,0,0,0.7f });
	}
	{
		Math::Matrix mat =
			Math::Matrix::CreateScale(6.0f) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(-250, 270, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_resultTex, Math::Rectangle{ 0,0,78,13 }, 1.0f);
	}

	if (m_timer <= 60)return;

	{
		float scale = 0.3f;
		if (m_hp)
		{
			scale = sinf(DirectX::XMConvertToRadians(m_timer * 90)) * 0.01f + 0.3f;
		}
		float offset = 2.0f;
		float dx[] = { offset, -offset, 0, 0,  offset, -offset,  offset, -offset };
		float dy[] = { 0, 0, offset, -offset,  offset,  offset, -offset, -offset };

		for (int i = 0; i < 8; i++) {
			Math::Matrix mat =
				Math::Matrix::CreateScale(scale) *
				Math::Matrix::CreateRotationZ(0) *
				Math::Matrix::CreateTranslation(-200 + dx[i], 120 + dx[i], 0);
			SHADER.m_spriteShader.SetMatrix(mat);
			SHADER.m_spriteShader.DrawTexBlack(&m_fireTex, Math::Rectangle{ 0,0,316, 508 }, 1.0f);
		}

		Math::Matrix mat =
			Math::Matrix::CreateScale(scale) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(-200, 120, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_fireTex, Math::Rectangle{ 0,0,316, 508 }, 1.0f);
	}

	{
		float scale = 4.5f;
		if (m_hp)
		{
			scale = sinf(DirectX::XMConvertToRadians(m_timer * 90)) * 0.15f + 4.5f;
		}
		Math::Matrix mat =
			Math::Matrix::CreateScale(scale) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(-200, 95, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_numberTex, Math::Rectangle{ m_hp * 13,13,13,13 }, 1.0f);
	}

	{
		Math::Rectangle rc;
		Math::Matrix mat;
		if (m_hp != 0)
		{
			rc = { 64 * 3, 64 * 3, 64, 64 };

			float scaleX = 5;
	
			if (m_timer % 60 < 30) scaleX *= -1;

			Math::Matrix mat =
				Math::Matrix::CreateScale(scaleX, 5, 1) *
				Math::Matrix::CreateRotationZ(0) *
				Math::Matrix::CreateTranslation(200, 95, 0);

			SHADER.m_spriteShader.SetMatrix(mat);
			SHADER.m_spriteShader.DrawTex(&m_playerTex, rc, 1.0f);
		}
		else
		{
			rc = { 64 * 5, 64 * 5, 64, 64 };

			Math::Matrix mat =
				Math::Matrix::CreateScale(5, 5, 1) *
				Math::Matrix::CreateRotationZ(0) *
				Math::Matrix::CreateTranslation(200, 95, 0);

			SHADER.m_spriteShader.SetMatrix(mat);
			SHADER.m_spriteShader.DrawTex(&m_playerTex, rc, 1.0f);
		}
		
	}

	if (m_timer <= 120)return;

	{
		Math::Matrix mat =
			Math::Matrix::CreateScale(6.0f) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(-289, -30, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_scoreTex, Math::Rectangle{ 0,0,65,13 }, 1.0f);
	}

	if (m_timer <= 180)return;

	{
		unsigned int tmp = m_score;
		int m_digits[10];
		for (int i = 10 - 1; i >= 0; --i)
		{
			// 下位の桁から抽出し配列に格納
			m_digits[i] = tmp % 10;	// 余りを求める演算子 %
			tmp /= 10;

			Math::Matrix mat =
				Math::Matrix::CreateScale(4.0f) *
				Math::Matrix::CreateRotationZ(0) *
				Math::Matrix::CreateTranslation(-289 + i * 52, -130, 0);
			SHADER.m_spriteShader.SetMatrix(mat);
			SHADER.m_spriteShader.DrawTex(&m_numberTex, Math::Rectangle{ m_digits[i] * 13,0,13,13 }, 1.0f);
		}
	}

	if (m_timer <= 220)return;

	{
		Math::Rectangle rc = { 0, 0, 90, 13 };
		Math::Matrix mat;
		rc = { 0, 0, 112, 26 };
		float scale = 4.0f + sinf(SCENE.GetFrameCount() * 0.02f) * 0.2f;
		mat =
			Math::Matrix::CreateScale(scale) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(0, -250, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_titleEntryTex, rc, scale);

	}
}

void ResultScene::Release()
{
	m_tex.Release();
	m_resultTex.Release();
	m_numberTex.Release();
	m_scoreTex.Release();
	m_fireTex.Release();
	m_playerTex.Release();
	m_titleEntryTex.Release();
}
