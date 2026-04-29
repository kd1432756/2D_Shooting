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

	bool IsAlive() { return m_isAlive; }

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

	void ChangeHP(int amount);

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

	bool m_isAlive;

	AnimState m_animState = AnimState::Idle;
	int m_animIndex = 0;
	float m_animTimer = 0.0f;
	float m_animSpeed = 0.1f;
	const int IDLE_FRAMES = 2;
	const int RUN_FRAMES = 8;
	const int ATTACK_FRAMES = 6;
	const int HIGH_ATTACK_FRAMES = 6;
	const int LOW_ATTACK_FRAMES = 6;
	const int DEATH_FRAMES = 10;
	int m_maxFrames = IDLE_FRAMES;

	bool  m_isAnimShotDone = false;    // 現在のアニメ中で既に弾を出したか
	bool  m_isSpecialReserved = false; // 必殺技の発射待ちフラグ

	Math::Vector2 m_vec = {};
	float m_speed = 3.0f;
	State m_state = State::Normal;
	float m_cooldownTimer = 0.0f;

	static const int MAX_SPECIALBULLETS = 2;
	PlayerBullet* m_specialBullets[MAX_SPECIALBULLETS] = { nullptr };
	SpecialBulletType m_specialBulletType = SpecialBulletType::TypeA;
	KdTexture m_bulletTex;

	const float SPREAD_ANGLE = 15.0f;
	bool m_isMissionSuccess = false;
	bool m_isLMissionSuccess = false;
	bool m_isRMissionSuccess = false;

	bool m_isShotRequested = false;
	float m_shootCooldown = 0.5f;
	float m_shootTimer = 0.0f;
};