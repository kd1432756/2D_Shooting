#pragma once

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


};