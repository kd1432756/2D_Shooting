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

private:

	KdTexture* m_tex;
	Math::Vector2 m_pos;
	bool m_isActive = false;
};