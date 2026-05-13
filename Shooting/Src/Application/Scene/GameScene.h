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
class FireWorm;
class EnemyBullet;
class HitEffect;
class TensionUpEffect;

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
    void CreateHitEffect(Math::Vector2& pos, float size);
    void SpawnEnemy();

    KdTexture m_BGTex[4];

	bool m_isDimmingActive = false;
	float m_overlayAlpha = 0.0f;
    bool m_changeFlg = false;

    unsigned int m_enemySpawnTimer = 0;

    bool m_clearFlg = false;

    int m_score = 0;

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

    FireWorm* m_fireWorm = nullptr;
    KdTexture m_fireWormTex;

    static const int MAX_ENEMY_BULLETS = 200;
    EnemyBullet* m_enemyBullets[MAX_ENEMY_BULLETS] = { nullptr };

    KdTexture m_fireTex;
    KdTexture m_numberTex;
    KdTexture m_nextSpecialTex;

    KdTexture m_scoreTex;
    KdTexture m_scoreNumberTex;

    KdTexture m_hitBoxTex;

    KdTexture m_resultTex;
    float m_resultAlpha = 0.0f;

    KdTexture m_resultEntryTex;
    float m_resultEntryAlpha = 0.0f;
    float m_resultEntryScale = 0.0f;

    static const int MAX_HIT_EFFECT = 100;
    HitEffect* m_hitEffect[MAX_HIT_EFFECT] = { nullptr };
    KdTexture m_hitEffectTex;

    TensionUpEffect* m_tensionUpEffect = nullptr;
    KdTexture m_tensionUpEffectTex;
};