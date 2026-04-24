#pragma once

#include "BaseScene.h"

class ResultScene : public BaseScene
{
public:
    ResultScene() { Init(); }  // コンストラクタ
    ~ResultScene() {} // デストラクタ

    void Init();
    void Update();
    void Draw();
	void Release();

private:
	KdTexture m_tex;

};