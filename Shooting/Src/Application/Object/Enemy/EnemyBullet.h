#pragma once
#include "Application/Object/BaseObject.h"

class EnemyBullet : public BaseObject
{
public:
	EnemyBullet() { Init(); }
	~EnemyBullet() { Release(); }

	void Init();
	void Update(Math::Vector2& playerPos);
	void Draw();
	void Release();

	void SetSpeed(float speed) { m_speed = speed; }
	void SetSize(float size) { m_size = size; }
	void SetWait(int wait) { m_moveWait = wait; }

	float GetSize() { return m_size; }

private:

	float m_speed;
	float m_size;

	int m_moveWait;
};