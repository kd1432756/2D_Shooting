#pragma once
#include "../../BaseObject.h"

class PlayerBullet : public BaseObject
{
public:

	PlayerBullet() { Init(); }
	~PlayerBullet() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

	void SetPosition(Math::Vector2& pos) { m_pos = pos; }
	void SetTexture(KdTexture* tex) { m_tex = tex; }
	void SetActive(bool active) { m_isActive = active; }

	bool IsActive() { return m_isActive; }

private:

	KdTexture* m_tex;
	Math::Vector2 m_pos;
	bool m_isActive = false;
	float m_speed = 15.0f;
};