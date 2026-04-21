#include"TitleScene.h"
#include"Application/Scene.h"

void TitleScene::Init()
{
	m_tex.Load("Texture/TitleScene/bg_title.png");
}

void TitleScene::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SCENE.RequestSceneChange(SceneName::Game);
	}
}

void TitleScene::Draw()
{
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 }, 1.0f);
}

void TitleScene::Release()
{
	m_tex.Release();
}
