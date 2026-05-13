#include"TitleScene.h"
#include"Application/Scene.h"

void TitleScene::Init()
{
	m_tex.Load("Texture/TitleScene/bg_title.png");
	m_titleTex.Load("Texture/TitleScene/title.png");
	m_startTex.Load("Texture/TitleScene/start.png");
	m_howToPlayTex.Load("Texture/TitleScene/how_to_play.png");
	m_playerTex.Load("Texture/TitleScene/player.png");
	m_bulletTex.Load("Texture/TitleScene/player_bullet.png");

	m_infoTex.Load("Texture/TitleScene/info.png");
	m_info2Tex.Load("Texture/TitleScene/info2.png");

	m_nowState = state::HowToPlay;
	m_isChanging = false;

	m_infoFlg = false;

	m_sinAngle = 0.0f;
	m_scale = 20.0f;
	m_angle = 0.0f;

	m_isKeyDown = false;
	m_isKeyDownZ = false;

	m_nowInfo = 1;

	//m_nowState = state::Start;
}

void TitleScene::Update()
{
	m_sinAngle += 1.0f;
	if (m_sinAngle >= 360) m_sinAngle -= 360;
	m_scale = sinf(DirectX::XMConvertToRadians(m_sinAngle)) * 2;
	m_angle = DirectX::XMConvertToRadians(sinf(DirectX::XMConvertToRadians(m_sinAngle)) * 5);

	if (!m_isChanging)
	{
		if (!m_infoFlg)
		{
			if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				if (!m_isKeyDown)
				{
					m_isKeyDown = true;
					if (m_nowState == state::HowToPlay) m_nowState = state::Start;
					else m_nowState = state::HowToPlay;
				}
			}
			else
			{
				if (m_isKeyDown) m_isKeyDown = false;
			}
		}
		else
		{
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				if (m_nowInfo == 1)m_nowInfo = 2;
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				if (m_nowInfo == 2)m_nowInfo = 1;
			}
		}

		if (GetAsyncKeyState('Z') & 0x8000)
		{
			if (m_nowState == state::Start)
			{
				SCENE.RequestSceneChange(SceneName::Game);
				m_isChanging = true;
			}
			else
			{
				if (!m_isKeyDownZ)
				{
					if (m_infoFlg)
					{
						m_infoFlg = false;
						m_isKeyDownZ = true;
					}
					else
					{
						m_infoFlg = true;
						m_isKeyDownZ = true;
					}
				}
			}
		}
		else m_isKeyDownZ = false;
	}
}

void TitleScene::Draw()
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
			Math::Matrix::CreateScale(20.0f + m_scale) *
			Math::Matrix::CreateRotationZ(m_angle) *
			Math::Matrix::CreateTranslation(-450, -100, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_playerTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
	}
	{
		Math::Matrix mat =
			Math::Matrix::CreateScale(10.0f) *
			Math::Matrix::CreateRotationZ(m_angle * -5 + DirectX::XMConvertToRadians(30.0f)) *
			Math::Matrix::CreateTranslation(500, -270, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_bulletTex, Math::Rectangle{ 0,0,16,5 }, 0.8f);
	}
	{
		Math::Matrix mat =
			Math::Matrix::CreateScale(1.0f) *
			Math::Matrix::CreateTranslation(100, 250, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_titleTex, Math::Rectangle{ 0,0,1000,120 }, 1.0f);
	}
	{
		float startScale = 1.0f;
		float howToPlayScale = 1.0f;
		Math::Matrix mat;
		if (m_nowState == state::Start)
		{
			startScale += abs(m_scale * 0.1f) + 0.2f;
		}
		else if (m_nowState == state::HowToPlay)
		{
			howToPlayScale += abs(m_scale * 0.1f) + 0.2f;
		}

		mat =
			Math::Matrix::CreateScale(howToPlayScale) *
			Math::Matrix::CreateTranslation(150, -50, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_howToPlayTex, Math::Rectangle{ 0,0,400,80 }, 1.0f);

		mat =
			Math::Matrix::CreateScale(startScale) *
			Math::Matrix::CreateTranslation(150, -200, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		SHADER.m_spriteShader.DrawTex(&m_startTex, Math::Rectangle{ 0,0,400,80 }, 1.0f);
	}
	if (m_infoFlg)
	{
		Math::Matrix mat =
			Math::Matrix::CreateScale(1.0f) *
			Math::Matrix::CreateRotationZ(0) *
			Math::Matrix::CreateTranslation(0, 0, 0);
		SHADER.m_spriteShader.SetMatrix(mat);
		if (m_nowInfo == 1)
		{
			SHADER.m_spriteShader.DrawTex(&m_infoTex, Math::Rectangle{ 0,0,1024,576 }, 0.9f);
		}
		else if (m_nowInfo == 2)
		{
			SHADER.m_spriteShader.DrawTex(&m_info2Tex, Math::Rectangle{ 0,0,1024,576 }, 0.9f);
		}
	}
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	//SHADER.m_spriteShader.DrawBox(150, -50, 200, 40);
	//SHADER.m_spriteShader.DrawBox(150, -200, 200, 40);
	//SHADER.m_spriteShader.DrawBox(100, 250, 500, 60);
}

void TitleScene::Release()
{
	m_tex.Release();
	m_titleTex.Release();
	m_startTex.Release();
	m_howToPlayTex.Release();
	m_playerTex.Release();
	m_bulletTex.Release();

	m_infoTex.Release();
	m_info2Tex.Release();
}
