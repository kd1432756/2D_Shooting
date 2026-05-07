#pragma once
#include "Application/Object/BaseObject.h"
#include "Bullet/PlayerBullet.h"

class Player : public BaseObject
{
public:

	enum class State
	{
		Normal,
		ReadyToShoot,
		Firing,
		Cooldown
	};

	enum class SpecialBulletType
	{
		TypeA,
		TypeB,
		TypeC
	};

	Player() { Init(); }
	~Player() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

	State GetState() const { return m_state; }

	bool IsShotRequested()
	{
		if(m_isShotRequested)
		{
			m_isShotRequested = false;
			return true;
		}
		return false;
	}

	void AddTension(float amount) {
		m_tensionGauge = std::min(MAX_TENSION, m_tensionGauge + amount);
	}

	float GetTensionPercent() { return m_tensionGauge / MAX_TENSION; }

	void ChangeHP(int amount);

	int GetHp() { return m_hp; }

	PlayerBullet** GetSpecialBullets() { return m_specialBullets; }

	int GetSpecialBulletsMax() { return MAX_SPECIALBULLETS; }

	int GetSpecialBulletType() { return (int)m_specialBulletType; }

private:

	enum class AnimState
	{
		Idle,
		Run,
		Attack,
		HighAttack,
		LowAttack,
		Death
	};

	void AnimUpdate();
	void ChangeAnimState(AnimState animState);

	void ShootSpecial(float angle, PlayerBullet::BulletType type);

	bool IsFireingFinished();

	void ChangeState(State newState);

	KdTexture m_tex;

	AnimState m_animState = AnimState::Idle;
	int m_animIndex = 0;
	float m_animTimer = 0.0f;
	float m_animSpeed = 0.1f;
	const int IDLE_FRAMES = 6;
	const int RUN_FRAMES = 8;
	const int ATTACK_FRAMES = 6;
	const int HIGH_ATTACK_FRAMES = 6;
	const int LOW_ATTACK_FRAMES = 6;
	const int DEATH_FRAMES = 10;
	int m_maxFrames = IDLE_FRAMES;

	bool  m_isAnimShotDone = false;    // 現在のアニメ中で既に弾を出したか
	bool  m_isSpecialReserved = false; // 必殺技の発射待ちフラグ

	Math::Vector2 m_vec = {};
	float m_speed = 5.0f;
	State m_state = State::Normal;
	float m_cooldownTimer = 0.0f;

	float m_tensionGauge = 0.0f;
	const float MAX_TENSION = 100.0f;

	static const int MAX_SPECIALBULLETS = 2;
	PlayerBullet* m_specialBullets[MAX_SPECIALBULLETS] = { nullptr };
	SpecialBulletType m_specialBulletType = SpecialBulletType::TypeA;
	KdTexture m_playerBulletTex;

	const float SPREAD_ANGLE = 15.0f;
	bool m_isMissionSuccess = false;
	bool m_isLMissionSuccess = false;
	bool m_isRMissionSuccess = false;

	bool m_isShotRequested = false;
	float m_shootCooldown = 0.5f;
	float m_shootTimer = 0.0f;
	
	float m_cutInTimer = 0.0f;     // カットインの経過時間
	const float CUTIN_DURATION = 1.0f; // カットインを表示する合計時間（秒）
};