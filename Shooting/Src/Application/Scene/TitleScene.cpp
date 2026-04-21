#include"TitleScene.h"
#include"Application/Scene.h"

void TitleScene::Init()
{
	m_tex.Load("Texture/TitleScene/titleBG.png");
}

void TitleScene::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SCENE.RequestSceneChange(SceneName::Game);
	}

	m_pos = Math::Matrix::CreateTranslation(0, 0, 0);
}

void TitleScene::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_pos);
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 }, 1.0f);
}

void TitleScene::Release()
{
	m_tex.Release();
}
