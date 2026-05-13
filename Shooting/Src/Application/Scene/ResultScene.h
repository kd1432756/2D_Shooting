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

    bool m_changeFlg = false;

	KdTexture m_tex;

    unsigned int m_timer;

    int m_hp;
    unsigned int m_score;

    KdTexture m_resultTex;
    KdTexture m_numberTex;
    KdTexture m_scoreTex;
    KdTexture m_fireTex;
    KdTexture m_playerTex;
    KdTexture m_titleEntryTex;
};