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

private:

	float m_speed = 15.0f;
};