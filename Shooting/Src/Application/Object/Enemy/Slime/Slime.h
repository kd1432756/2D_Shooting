#pragma once
#include"Application/Object/Enemy/Enemy.h"

class Slime : public Enemy
{
public:
	Slime() { Init(); }
	~Slime() { Release(); }

	void Init();
	void Update();
	void Draw();
	void Release();

private:

	KdTexture* m_tex;
	Math::Vector2 m_pos;
	bool m_isActive = false;

};