#pragma once

#include "BaseScene.h"

class Hit;
class Player;
class PlayerBullet;
class Enemy;
class Slime;
class SkullWolf;
class Phoenix;
class Fairy;
class EnemyBullet;

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

    void HitCheck();

    void SpawnSlime(float posY);

    KdTexture m_BGTex[4];

	bool m_isDimmingActive = false;
	float m_overlayAlpha = 0.0f;

    unsigned int m_enemySpawnTimer = 0;

    Hit* m_hit = nullptr;

    Player* m_player = nullptr;
    Enemy* m_enemy = nullptr;

    static const int MAX_PLAYER_BULLETS = 100;
    PlayerBullet* m_playerBullets[MAX_PLAYER_BULLETS] = { nullptr };

    KdTexture m_playerBulletTex;

    static const int MAX_SLIMES = 30;
    Slime* m_slime[MAX_SLIMES] = { nullptr };
	KdTexture m_slimeTex;

    static const int MAX_SKULL_WOLF = 20;
    SkullWolf* m_skullWolf[MAX_SKULL_WOLF] = { nullptr };
	KdTexture m_skullWolfTex;

    static const int MAX_PHOENIX = 20;
    Phoenix* m_phoenix[MAX_PHOENIX] = { nullptr };
	KdTexture m_phoenixTex;

    static const int MAX_FAIRY = 20;
    Fairy* m_fairy[MAX_FAIRY] = { nullptr };
	KdTexture m_fairyTex;

    static const int MAX_ENEMY_BULLETS = 100;
    EnemyBullet* m_enemyBullets[MAX_ENEMY_BULLETS] = { nullptr };

    KdTexture m_fireTex;
    KdTexture m_numberTex;
    KdTexture m_nextSpecialTex;
};