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

    enum class state
    {
        None,
        Start,
        HowToPlay
    };

    state m_nowState;
    bool m_isChanging = false;

    bool m_infoFlg = false;;

    float m_sinAngle = 0.0f;

    float m_scale = 20.0f;
    float m_angle = 0.0f;

    bool m_isKeyDown = false;
    bool m_isKeyDownZ = false;

    int m_nowInfo = 1;

	KdTexture m_tex;
	KdTexture m_titleTex;
	KdTexture m_startTex;
	KdTexture m_howToPlayTex;
	KdTexture m_playerTex;
	KdTexture m_bulletTex;

	KdTexture m_infoTex;
	KdTexture m_info2Tex;
};