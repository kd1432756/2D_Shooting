#pragma once

#include "BaseScene.h"

class TitleScene : public BaseScene
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
};