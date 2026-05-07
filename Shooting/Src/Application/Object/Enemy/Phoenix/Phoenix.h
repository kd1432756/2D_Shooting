#pragma once
#include"Application/Object/Enemy/Enemy.h"

class Phoenix : public Enemy
{
public:
	Phoenix() { Init(); }
	~Phoenix() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

private:

	bool m_isMove = false;
	int m_moveCount = 0;
};