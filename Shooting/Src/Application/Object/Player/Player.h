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

	void SetMissionSuccess(bool success) { m_isMissionSuccess = success; }
	void SetMissionSuccess(bool leftSuccess, bool rightSuccess) { m_isLMissionSuccess = leftSuccess; m_isRMissionSuccess = rightSuccess; }

private:

	void ShootSpecial(float angle, PlayerBullet::BulletType type);

	bool IsFireingFinished();

	void ChangeState(State newState);

	bool CheckMissionSuccess();

	KdTexture m_tex;

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