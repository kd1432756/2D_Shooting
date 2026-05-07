#pragma once
#include"Application/Object/Enemy/Enemy.h"

class Fairy : public Enemy
{
public:
	Fairy() { Init(); }
	~Fairy() { Release(); }

	void Init();
	void Update(Math::Vector2 plauyerPos);
	void Draw();
	void Release();

private:

	bool m_isShotReady = false;
	bool m_hasShoted = false;

};