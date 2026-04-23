#include "GameScene.h"
#include"Application/Scene.h"
#include "Application/Object/Player/Player.h"

void GameScene::Init()
{
	m_tex.Load("Texture/GameScene/bg_game.png");
	m_player = new Player();
}

void GameScene::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SCENE.RequestSceneChange(SceneName::Result);
	}
	m_player->Update();
}

void GameScene::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.DrawTex(&m_tex, Math::Rectangle{ 0,0,1280,720 }, 1.0f);
	m_player->Draw();
}

void GameScene::Release()
{
	m_tex.Release();
	if(m_player)
	{
		delete m_player;
		m_player = nullptr;
	}
}
