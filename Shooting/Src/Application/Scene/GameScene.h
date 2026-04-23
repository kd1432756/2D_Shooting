#pragma once

class Player;

class GameScene
{
public:
    GameScene() { Init(); }  // コンストラクタ
    ~GameScene() {} // デストラクタ

    void Init();
    void Update();
    void Draw();
    void Release();

private:
    KdTexture m_tex;

	Player* m_player;
};