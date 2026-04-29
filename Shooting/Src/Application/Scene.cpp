#include "main.h"
#include "Scene.h"
#include "Application/Scene/TitleScene.h"
#include "Application/Scene/GameScene.h"
#include "Application/Scene/ResultScene.h"

void Scene::Draw2D()
{
	m_nowScene->Draw();

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.DrawTex(&m_transitionTex, Math::Rectangle{ 0,0,1280,720 }, m_transitionProgress * 2.0f);
}

void Scene::Update()
{
	frameCount++;

	m_nowScene->Update();

	if(m_isTransitioning)
	{
		m_transitionProgress += TRANSITION_SPEED;
		if (m_transitionProgress >= 1.0f)
		{
			m_transitionProgress = 0.0f;
			m_isTransitioning = false;
			ExecuteSceneChange();
		}
	}
}

void Scene::Init()
{
	frameCount = 0;

	m_isTransitioning = false;
	m_transitionProgress = 0.0f;

	currentScene = SceneName::Title;
	nextScene = currentScene;

	m_nowScene = new TitleScene();

	m_transitionTex.Load("Texture/transition.png");
}

void Scene::Release()
{
	delete m_nowScene;
	m_nowScene = nullptr;

	m_transitionTex.Release();
}

void Scene::ImGuiUpdate()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}

void Scene::RequestSceneChange(SceneName next)
{
	nextScene = next;
	m_isTransitioning = true;
}

void Scene::ExecuteSceneChange()
{
	if (nextScene == SceneName::None) return;

	delete m_nowScene;

	currentScene = nextScene;

	switch (currentScene)
	{
	case SceneName::Title:
		m_nowScene = new TitleScene();			
		break;
	
	case SceneName::Game:
		m_nowScene = new GameScene();
		break;
	
	case SceneName::Result:
		m_nowScene = new ResultScene();
		break;
	}

	nextScene = SceneName::None;
}