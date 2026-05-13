#pragma once

#include "BaseEffect.h"

class TensionUpEffect : public BaseEffect
{
public:

	TensionUpEffect() {}
	virtual ~TensionUpEffect() {}

	void Init();
	void Update(Math::Vector2& pos);
	void Draw();
	void Release();

private:

};