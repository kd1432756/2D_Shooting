#pragma once
#include"Application/Object/Enemy/Enemy.h"

class SkullWolf : public Enemy
{
public:
	SkullWolf() { Init(); }
	~SkullWolf() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

private:

	int m_moveCount = 0;

};