#pragma once

#include "BaseScene.h"

class Player;
class PlayerBullet;
class Slime;

class GameScene : public BaseScene
{
public:
    GameScene() { Init(); }  // コンストラクタ
    ~GameScene() {} // デストラクタ

    void Init();
    void Update();
    void Draw();
    void Release();

private:
	const float ANGLE_RIGHT = 0.0f;

    KdTexture m_tex;

	bool m_isDimmingActive = false;
	float m_overlayAlpha = 0.0f;

	Player* m_player;

    static const int MAX_PLAYER_BULLETS = 100;
    PlayerBullet* m_playerBullets[MAX_PLAYER_BULLETS] = { nullptr };

    KdTexture m_bulletTex;

    static const int MAX_SLIMES = 10;
    Slime* m_slime[MAX_SLIMES] = { nullptr };
	KdTexture m_slimeTex;

};