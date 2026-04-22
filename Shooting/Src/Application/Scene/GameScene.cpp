#include "GameScene.h"
#include"Application/Scene.h"

void GameScene::Init()
{
	m_tex.Load("Texture/GameScene/bg_game.png");
}

void GameScene::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SCENE.RequestSceneChange(SceneName::Result);
	}
}

void GameScene::Draw()
{
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 }, 1.0f);
}

void GameScene::Release()
{
	m_tex.Release();
}
