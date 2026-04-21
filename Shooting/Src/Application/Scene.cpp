#include "main.h"
#include "Scene.h"
#include "Application/Scene/TitleScene.h"
#include "Application/Scene/GameScene.h"
#include "Application/Scene/ResultScene.h"

void Scene::Draw2D()
{
	switch (currentScene)
	{
	case SceneName::Title:
		m_titleScene->Draw();
		break;

	case SceneName::Game:
		m_gameScene->Draw();
		break;

	case SceneName::Result:
		m_resultScene->Draw();
		break;
	}

	SHADER.m_spriteShader.DrawTex(nullptr);
}

void Scene::Update()
{
	frameCount++;

	switch (currentScene)
	{
	case SceneName::Title:
		m_titleScene->Update();
		break;

	case SceneName::Game:
		m_gameScene->Update();
		break;

	case SceneName::Result:
		m_resultScene->Update();
		break;
	}

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

	m_titleScene = new TitleScene();
}

void Scene::Release()
{
	if (m_titleScene != nullptr)delete m_titleScene;
	if (m_gameScene != nullptr)delete m_gameScene;
	if (m_resultScene != nullptr)delete m_resultScene;
}

void Scene::ImGuiUpdate()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
		//ImGui::Text("%s", currentScene);
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

	switch (currentScene)
	{
	case SceneName::Title:
		delete m_titleScene;
		m_titleScene = nullptr;
		break;

	case SceneName::Game:
		delete m_gameScene;
		m_gameScene = nullptr;
		break;

	case SceneName::Result:
		delete m_resultScene;
		m_resultScene = nullptr;
		break;
	}

	currentScene = nextScene;

	switch (currentScene)
	{
	case SceneName::Title:
		m_titleScene = new TitleScene();			
		break;
	
	case SceneName::Game:
		m_gameScene = new GameScene();
		break;
	
	case SceneName::Result:
		m_resultScene = new ResultScene();
		break;
	}

	nextScene = SceneName::None;
}