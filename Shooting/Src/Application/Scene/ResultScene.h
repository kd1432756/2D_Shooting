#pragma once

class ResultScene
{
public:
    ResultScene() { Init(); }  // コンストラクタ
    ~ResultScene() {} // デストラクタ

    void Init();
    void Update();
    void Draw();
	void Release();

private:


};