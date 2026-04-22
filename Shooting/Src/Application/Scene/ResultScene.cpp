#include "ResultScene.h"
#include"Application/Scene.h"

void ResultScene::Init()
{
	m_tex.Load("Texture/ResultScene/bg_result.png");
}

void ResultScene::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SCENE.RequestSceneChange(SceneName::Title);
	}
}

void ResultScene::Draw()
{
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 }, 1.0f);
}

void ResultScene::Release()
{
	m_tex.Release();
}
