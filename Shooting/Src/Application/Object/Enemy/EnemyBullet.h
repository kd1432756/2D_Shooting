#pragma once
#include "Application/Object/BaseObject.h"

class EnemyBullet : public BaseObject
{
public:
	EnemyBullet() { Init(); }
	~EnemyBullet() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

	void SetSpeed(float speed) { m_speed = speed; }
	void SetSize(float size) { m_size = size; }

	float GetSize() { return m_size; }

private:

	float m_speed;
	float m_size;
};