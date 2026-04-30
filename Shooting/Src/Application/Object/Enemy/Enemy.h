#pragma once
#include "Application/Object/BaseObject.h"

class Enemy : public BaseObject
{
public:
	Enemy() { Init(); }
	~Enemy() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

	void ChangeHP(int amount)
	{
		m_hp += amount;
		if (m_hp <= 0)
		{
			m_isAlive = false;
		}
	}

private:

};