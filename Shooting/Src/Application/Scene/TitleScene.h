#pragma once

class TitleScene
{
public:
    TitleScene() { Init(); }  // コンストラクタ
    ~TitleScene() { Release(); } // デストラクタ

    void Init();
    void Update();
    void Draw();
    void Release();

private:
	KdTexture m_tex;
	Math::Matrix m_pos;
};