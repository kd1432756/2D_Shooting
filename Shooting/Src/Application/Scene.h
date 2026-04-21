#pragma once

enum class SceneName { Title, Game, Result, None };

class TitleScene;
class GameScene;
class ResultScene;

class Scene
{
protected:

	int frameCount;

	bool m_isTransitioning;
	float m_transitionProgress;
	float TRANSITION_SPEED = 0.02f;

private:

	SceneName currentScene; // 現在のシーン
	SceneName nextScene;             // 次に切り替えるシーンの予約

	// 実際にシーンを切り替え内部処理
	void ExecuteSceneChange();

	TitleScene* m_titleScene; // タイトルシーン
	GameScene* m_gameScene; // ゲームシーン
	ResultScene* m_resultScene; // リザルトシーン

	KdTexture m_transitionTex; // トランジション用テクスチャ

public:

	// 初期設定
	void Init();

	// 解放
	void Release();

	// 更新処理
	void Update();

	// 描画処理
	void Draw2D();

	// GUI処理
	void ImGuiUpdate();

	// シーンの切り替え予約
	void RequestSceneChange(SceneName next);

private:

	Scene() {}

public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}
};

#define SCENE Scene::GetInstance()
